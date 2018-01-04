#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include <stdio.h>
#include <avr/pgmspace.h>


#include "FreeRTOS.h"
#include "queue.h"
#include "cmdline.h"

#include "task.h"

#include "../../Lib/include/tlvProt.h"
#include "main.h"


extern xQueueHandle         xMainRec;



typedef struct mainMsg
{
  uint16_t duration;
  uint8_t command;
  uint8_t data[4];
} mainMsg_t;


/**
 * Proces odpowiedzialny za obsługę głównej pętli programu
 * @param *Ptr
 */
void vTaskMain(void *Ptr);




#endif /* MAIN_TASK_H */
