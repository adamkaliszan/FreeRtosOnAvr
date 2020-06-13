#include "rc_task.h"

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "../../../Lib/include/rc_ibus.h"

#include "hardwareConfig.h"


extern xQueueHandle xSIM900Rec;
extern xQueueHandle xSIM900Tx;



void vTaskRc(void *ptr)
{
    (void) ptr;
    uint8_t tmp;
    for (;;)
    {
        if (xQueueReceive(xSIM900Rec, &tmp, 1) == pdFALSE)
        {
            ibus_newFrame();
            continue;
        }
        if (!ibus_processDta(tmp))
            continue;
        
        ibus_processMessage();
    }
}
