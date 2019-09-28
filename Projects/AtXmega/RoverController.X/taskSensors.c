#include <string.h>

#include "taskSensors.h"
#include "task.h"

#include "drvPAL/i2cPAL.h"
#include "hardware.h"
#include "hardwareConfig.h"


extern HardwarePAL_t hardwarePAL;

const uint8_t baroCmd[] = {0x00, 0x01};
#define BARO_RESP_LEN 3


void vTaskSensors(void *Ptr)
{
    struct
    {
        union
        {
            struct
            {
                uint8_t data[8];
                uint8_t length;
            } cmd;
        };
        
    } tmp;
    
    for (;;)
    {
        tmp.cmd.length = sizeof(baroCmd);
        memcpy(tmp.cmd.data, baroCmd, tmp.cmd.length);
        if (TWI_MasterWriteRead(&hardwarePAL.twiSensors, BARO_ADDR, tmp.cmd.data, tmp.cmd.length, BARO_RESP_LEN))
        {
            while(PORTD.IN)
                vTaskDelay(10); //TODO Adam use async IO
            
            
        }
        
        vTaskDelay(20);
        
        
    }
    
    
}