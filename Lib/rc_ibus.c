#include "rc_ibus.h"

#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#define BS 32
static struct 
{
    uint8_t type;
    uint8_t size;
    union
    {
        uint8_t data[28];
        uint16_t channels[14];
    };    
} message;

static uint16_t _noOfNewFrames = 0;
static uint16_t _noOfProcFrames = 0;
static uint16_t _noOfCrcErrors = 0;


uint16_t rc_getNoOfCrcErrors(void)
{
    return _noOfCrcErrors;
}

uint16_t rc_getNoOfNewFrames(void)
{
    return _noOfNewFrames;
}

uint16_t rc_getNoOfProcFrames(void)
{
    return _noOfProcFrames;
}

uint16_t rc_getChan(uint8_t idx)
{
    return message.channels[idx];
}


static enum
{
    RC_ST_SIZE,
    RC_ST_MSG_TYPE,
    RC_ST_DATA,
    RC_ST_CRC_HI,
    RC_ST_CRC_LO
} _state = RC_ST_SIZE;

void ibus_newFrame()
{
    if (_state != RC_ST_SIZE)
        _noOfNewFrames++;    
    _state = RC_ST_SIZE;
}

int8_t rcChannels[8];


void ibus_processMessage(void)
{
    uint8_t i;
    if (message.type == 0x40)
    {
        for (i=0; i<8; i++)
            rcChannels[i] = (1500 - message.channels[i]) / 4;
    }
}

uint8_t ibus_processDta(uint8_t dta)
{
    uint8_t result = 0;
    static uint8_t dtaIdx;
    static uint16_t sum;
    
  //  return result;
    switch (_state)
    {
    case RC_ST_SIZE:
        if (dta != 0x20)
            break;
        
        message.size = dta;
        _state = RC_ST_MSG_TYPE;
        sum = 0xFFFF-dta;
        break;
        
    case RC_ST_MSG_TYPE:
        if (dta == 0x020)
            break;
        
        if (dta != 0x40)
        {
            _state = RC_ST_SIZE;
            break;
        }
        
        message.type = dta;
        _state = RC_ST_DATA;
        sum-= dta;
        dtaIdx = 0;  
        break;

    case RC_ST_DATA:
        if (dtaIdx < sizeof(message.data))
            message.data[dtaIdx] = dta;
        dtaIdx++;
        sum-= dta;
        if (dtaIdx + 4 == message.size)
            _state = RC_ST_CRC_LO;    
        break;

    case RC_ST_CRC_LO:
        sum-=dta;
        _state = RC_ST_CRC_HI;
        break;
        
    case RC_ST_CRC_HI:
        _state = RC_ST_SIZE;
        _noOfNewFrames++;
        if (((sum & 0xFF) == 0) && (sum>>8 == dta))
        {
            _noOfProcFrames++;
            result = 1;
        }
        else
        {
            _noOfCrcErrors++;            
        }
        break;
    }
    return result;
}
