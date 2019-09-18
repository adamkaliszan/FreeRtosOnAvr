#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/crc16.h>



#include "FreeRTOS.h"
#include "croutine.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

#include "memory_x.h"
#include "queueStream.h"
#include "cli_task.h"
#include "tlv_task.h"
#include "main_task.h"
#include "serial.h"

#include "hardwareConfig.h"
#include "softwareConfig.h"

#include "hardware.h"


#include "cmdline.h"
#include "vty.h"
#include "tlv.h"




#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 1

#define SYSTEM_NAME "FreeRtos+"
#define S_VERSION "0.11"

extern const TlvCommand_t tlvCmdList[];

void my_init_clock (void) __attribute__ ((naked))  __attribute__ ((section (".init0")));

#endif
