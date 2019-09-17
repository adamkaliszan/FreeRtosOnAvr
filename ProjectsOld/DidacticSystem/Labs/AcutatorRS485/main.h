#ifndef MAIN_H
#define MAIN_H


#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include <string.h>
#include "FreeRTOS.h"
#include "croutine.h"
#include "queue.h"
#include "task.h"

#include "hardware.h"
#include "serial.h"

#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 100
#define mainRESET_COUNT_ADDRESS        ( ( void * ) 0x50 )


/**
 * Bufory, do komunikacji z korutynami.
 * Same korutyny mają ograniczony kontakt ze sprzętem.
 * Wyjątkiem jest wysyłanie danych. Po umieszczeniu ich w buforze nadawczym,
 * należy włączyć obsługę przerwania "miejsce w sprzętowym buforze nadawczym".
 */
xQueueHandle xRxedChars;       /// Bufor, który przechowuje odebrane znaki z magistrali RS 485
xQueueHandle xCharsForTx;      /// Bufor, który przechowuje znaki do wysłania przez magistralę RS 485


#endif
