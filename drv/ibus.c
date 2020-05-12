#include <stdint.h>

#include "softwareConfig.h"

#if USE_IBUS

#include "FreeRTOS.h"
#include "queue.h"
#include "ibus.h"

/*
  The data is serial data, 115200, 8N1.
  Messages arrive every 7 milliseconds, and are read constantly until a
  few tenths of a second after the transmitter is switched off.

  Packet format:
  20 40 CH0 CH1 CH2 CH3 CH4 CH5 CH6 CH7 CH8 CH9 CH10 CH11 CH12 CH13 SUM
  Channels are stored in little endian byte order.  Unused channels read
  5DC (first byte DC, second byte 05).

  Channel 0: Right horizontal: 3E8 -> 7D0
  Channel 1: Right vertical:   3E8 -> 7CF
  Channel 2: Left vertical:    3E8 -> 7D0
  Channel 3: Left horizontal:  3E8 -> 7CD
  Channel 4: Left pot:         3E8 -> 7D0
  Channel 5: Right pot:        3E8 -> 7D0

  The checksum starts at 0xFFFF, then subtract each byte except the 
  checksum bytes.
*/

void vTaskIbus(void *ptr)
{
    IbusState_t *state = (IbusState_t *) ptr;
    uint8_t recDta;
    uint8_t doNotSkip;
    for (;;)
    {
        doNotSkip = xQueueReceive(state->inputBuffer, &recDta, 1);
        if (doNotSkip == pdFALSE) 
        {
            ibusReset(state);
            continue;
        }
      
        
        if (ibusProcess(state, recDta))
            ibusExecute(state);
    }
    
}

void ibusReset(IbusState_t *state)
{
    state->protocol.state = IBPS_DTA_LEN;
    state->protocol.checksum = 0xFF;
    state->protocol.dtaIdx = 0;
}

void ibusExecute(IbusState_t *state)
{
    
}

uint8_t ibusProcess(IbusState_t* state, uint8_t ch) 
{
    uint8_t doExecute = 0;
    
    switch (state->protocol.state) 
    {
    case IBPS_DTA_LEN:
        state->protocol.msgLen = ch;
        state->protocol.msgLen--;
        state->protocol.checksum-= ch;
        state->protocol.state = IBPS_MSG_TYPE;
        break;
        
    case IBPS_MSG_TYPE:
        state->protocol.msgLen--;
        state->protocol.checksum-= ch;
        state->protocol.msgType = ch;
        
        if (state->protocol.msgLen > 2)
            state->protocol.state = IBPS_DTA;
        else
            state->protocol.state = IBPS_CRC_LO;

    case IBPS_DTA:
        state->protocol.data[state->protocol.dtaIdx++] = ch;
        state->protocol.msgLen--;
        state->protocol.checksum-= ch;
        if (state->protocol.msgLen == 2)
            state->protocol.state = IBPS_CRC_LO;
            
            
        break;
        
    case IBPS_CRC_LO:
        state->protocol.state = IBPS_CRC_HI;
        

    case IBPS_CRC_HI:
        if ((ch<<8)+state->protocol.crcLo != state->protocol.checksum)
        {
            state->protocol.noOfCrcErr++;
        }
        else
        {
            doExecute = 1;
        }
        ibusReset(state);
        break;
    }
    return doExecute;
}


#endif