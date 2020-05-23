#include <string.h>

#include "taskSensors.h"
#include "task.h"

#include "twi.h"
#include "hardware.h"
#include "hardwareConfig.h"


extern HardwarePAL_t hardwarePAL;

const uint8_t baroCmd[] = {0x00, 0x01};
#define BARO_RESP_LEN 3


void vTaskSensors(void *Ptr)
{   
    for (;;)
    {
//        tmp.cmd.length = sizeof(baroCmd);
//        memcpy(tmp.cmd.data, baroCmd, tmp.cmd.length);
//        if (TwiMaster_ReadAndWrite(&hardwarePAL.twiSensors, BARO_ADDR, tmp.cmd.data, tmp.cmd.length, BARO_RESP_LEN))
//        {
//            //while(PORTD.IN)
//                vTaskDelay(10); //TODO Adam use async IO
//        }
        vTaskDelay(20);
    }
}