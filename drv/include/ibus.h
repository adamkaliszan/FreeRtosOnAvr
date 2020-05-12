#if !defined(IBUS_H)
#define IBUS_H

#include "softwareConfig.h"

#if USE_IBUS

#include <stdint.h>
#include <stdio.h>

#include "FreeRTOS.h"



void vTaskIbus(void *Ptr);


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

typedef enum 
{
    IBPS_DTA_LEN,
    IBPS_MSG_TYPE,
    IBPS_DTA,
    IBPS_CRC_LO,
    IBPS_CRC_HI
} IBUS_PROT_STATE_t;

typedef struct IbusState 
{
    xQueueHandle inputBuffer;
    xQueueHandle outputBuffer;
    
    FILE *debug;
    uint8_t channels[IBUS_NO_OF_CHAN];
    
    struct
    {
        IBUS_PROT_STATE_t state;

        uint16_t checksum;
        int8_t msgLen;
        uint8_t msgType;        
        
        uint8_t dtaIdx;
        uint8_t crcLo;        

        uint8_t data[30];

        uint16_t noOfCrcErr;        
    } protocol;
} IbusState_t;

void ibusReset(IbusState_t *state);

void ibusExecute(IbusState_t *state);

uint8_t ibusProcess(IbusState_t* state, uint8_t ch);

#endif /* USE_IBUS */

#endif
