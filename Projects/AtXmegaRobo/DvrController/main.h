#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/crc16.h>

#include "memory_x.h"
//#include "Rs485_prot.h"

#include "../../../FreeRtosCore/Source/include/FreeRTOS.h"
#include "../../../FreeRtosCore/Source/include/croutine.h"
#include "../../../FreeRtosCore/Source/include/queue.h"
#include "../../../FreeRtosCore/Source/include/task.h"
#include "../../../FreeRtosCore/Source/include/semphr.h"

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

extern const tlvCommand_t tlvCmdList[];

void my_init_clock (void) __attribute__ ((naked))  __attribute__ ((section (".init0")));

#endif
