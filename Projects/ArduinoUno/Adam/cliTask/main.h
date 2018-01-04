#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/crc16.h>


#include "FreeRTOS.h"
#include "croutine.h"
#include "queue.h"
#include "task.h"
#include "queueStream.h"
#include "cli_task.h"
#include "loop_task.h"
#include "serial.h"

#include "hardwareConfig.h"
#include "softwareConfig.h"

#include "hardware.h"

#include "cmdline.h"
#include "memory_x.h"
#include "vty.h"


#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 1

#define SYSTEM_NAME "FreeRtos+"
#define S_VERSION "0.31"


#endif
