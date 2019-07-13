#ifndef TLV_TASK_H
#define TLV_TASK_H

#include <stdio.h>
#include <avr/pgmspace.h>


#include "FreeRTOS.h"
#include "queue.h"
#include "cmdline.h"

#include "task.h"

#include "../../Lib/include/tlvProt.h"
#include "main.h"




extern xSemaphoreHandle HC21semaphore;

extern FILE hc12Stream;

extern xQueueHandle         xTlvRec;
extern xQueueHandle         xTlvTx;


extern uint8_t  confHC12mode;  //1 - 3
extern uint16_t confHC12baud; //
extern uint8_t  confHC12channel;
extern uint8_t  confHC12power; //1-8


/**
 * Proces odpowiedzialny za obsługę protokołu TLV
 * Kod zoptymalizowany do szczególnego przypadku
 * @param *TlvPtr wskaźnik do struktury przechowującej stan sesji protokołu TLV
 */
void vTaskTLV(void *tlvIntPtr);





#endif /* TLV_TASK_H */
