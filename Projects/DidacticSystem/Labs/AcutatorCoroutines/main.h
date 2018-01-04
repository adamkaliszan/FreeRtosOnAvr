#ifndef MAIN_H
#define MAIN_H


#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include <string.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "croutine.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"

#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 100
#define mainRESET_COUNT_ADDRESS			( ( void * ) 0x50 )

#define STR_V1 '0'
#define STR_V2 '.'
#define STR_V3 '0'
#define STR_V4 '0'
#define STR_V5 '0'

#endif
