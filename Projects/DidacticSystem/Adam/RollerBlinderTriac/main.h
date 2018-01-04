#ifndef MAIN_H
#define MAIN_H

#define X2 1


#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include <string.h>
#include <avr/eeprom.h> 
#include "FreeRTOS.h"
#include "croutine.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
#include "hardware.h"
#include "automat.h"
#include "../../freeRtos/Lib/include/protocol1.h"

#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 100
#define mainRESET_COUNT_ADDRESS			( ( void * ) 0x50 )

#define DO_INCR_RST 0

#define STR_V1 '0'
#define STR_V2 '.'
#define STR_V3 '0'
#define STR_V4 '0'
#define STR_V5 '0'

uint8_t settings;

#endif
