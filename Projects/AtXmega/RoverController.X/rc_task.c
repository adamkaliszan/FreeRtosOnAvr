#include "rc_task.h"

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "../../../Lib/include/rc_ibus.h"

#include "hardwareConfig.h"
#include "hardware.h"


extern xQueueHandle xSIM900Rec;
extern xQueueHandle xSIM900Tx;



void vTaskRc(void *ptr)
{
    (void) ptr;
    uint8_t tmp;
    for (;;)
    {
        if (xQueueReceive(xSIM900Rec, &tmp, 50) == pdFALSE)
        {
            pwrOff4v3();
            vTaskDelay(500);
            pwrOn4v3rpi();
            continue;
        }
        if (!ibus_processDta(tmp))
            continue;
        
        ibus_processMessage();
        
        int16_t fwL = rcChannels[2] + rcChannels[3];
        int16_t fwR = rcChannels[2] - rcChannels[3];
        
        if (fwL > 127) fwL = 127;
        if (fwL < -127) fwL = -127;

        if (fwR > 127) fwR = 127;
        if (fwR < -127) fwR = -127;
        
        if ((fwL >= 0) && (fwR >= 0)) forwardB(fwL, fwR);
        if ((fwL <  0) && (fwR <  0)) backwordB(-fwL, -fwR);

        if ((fwL >= 0) && (fwR <  0)) rotateRightB(fwL, -fwR);
        if ((fwL <  0) && (fwR >= 0)) rotateLeftB(-fwL, fwR);

    }
}
