#ifndef CLI_TASK_H
#define CLI_TASK_H

#include <stdio.h>
#include <avr/pgmspace.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "cmdline.h"

extern xQueueHandle         xRs1Rec;
extern xQueueHandle         xRs1Tx;

/**
 * Proces odpowiedzialny za obsługę VTY
 * Kod zoptymalizowany do szczególnego przypadku
 * @param *cliStatePtr wskaźnik do struktury przechowującej stan interpretera poleceń
 */
void vTaskVtyRs1(void *cliStatePtr);


/**
 * Proces odpowiedzialny za obsługę VTY
 * Kod uniwersalny dla dowolnego strumienai FILE
 * @param *cliStatePtr wskaźnik do struktury przechowującej stan interpretera poleceń
 */
void vTaskVTYsocket(void *cliStatePtr);


#endif /* CLI_TASK_H */
