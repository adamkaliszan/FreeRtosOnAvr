#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/crc16.h>

#include "memory_x.h"
#include "Rs485_prot.h"

#include "FreeRTOS.h"
#include "croutine.h"
#include "queue.h"
#include "task.h"
#include "queueStream.h"
#include "cli_task.h"
#include "serial.h"

#include "hardwareConfig.h"
#include "softwareConfig.h"

#include "hardware.h"
#include "spi.h"
#include "mpc23s17.h"
#include "mcp3008.h"
#include "ds1305.h"
#include "enc28j60.h"

#include "ramdysk.h"
#include "ff.h"
#include "sd_diskio.h"
#include "rtc.h"

#include "sensors_task.h"
#include "netstack_task.h"


#include "cmdline.h"
#include "vty.h"

#include "../../Lib/net/include/arp.h"
#include "ip.h"
#include "arp.h"
#include "udp.h"
#include "tcp.h"



#define mainCHECK_TASK_PRIORITY 1
#define mainCHECK_PERIOD 1

#define SYSTEM_NAME "FreeRtos+"
#define S_VERSION "0.31"




volatile timeDecoded_t czasRtc;

void initExternalMem(void) __attribute__ ((naked)) __attribute__ ((section (".init4")));

extern UdpSocket_t *udpSocket;

#endif
