/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Ethernet remote device and sensor
 * UDP and HTTP interface
        url looks like this http://baseurl/password/command
        or http://baseurl/password/
 *
 * Chip type           : Atmega88 or Atmega168 or Atmega328 with ENC28J60
 * Note: there is a version number in the text. Search for tuxgraphics
 *********************************************/
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "main.h"
#include "memory_x.h"
#include "Rs485_prot.h"

#ifndef SENSORS_TASK_H
#define SENSORS_TASK_H




#define NOT_DETECTED          0x01
#define BOOTLOADER_MODE       0x02
#define NEW_STATE             0x04
#define RESERVED              0x08
#define ROLLER1_UP            0x10
#define ROLLER1_DOWN          0x20
#define ROLLER2_UP            0x40
#define ROLLER2_DOWN          0x80


extern volatile uint8_t temperature;
extern volatile uint8_t voltage;




void sensorsTaskInit(void);

void sensorsTask(void *pvParameters);

#endif
