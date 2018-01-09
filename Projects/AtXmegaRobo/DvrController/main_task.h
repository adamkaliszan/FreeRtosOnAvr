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


/**
 * Proces odpowiedzialny za obsługę głównej pętli programu
 * @param *Ptr
 */
void vTaskMain(void *Ptr);




#endif /* MAIN_TASK_H */
