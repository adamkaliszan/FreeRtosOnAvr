#ifndef ENC_TASK_H
#define ENC_TASK_H 1

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "enc28j60.h"
#include "net.h"
#include "nic.h"
#include "ip.h"
#include "arp.h"
#include "udp.h"
#include "tcp.h"
#include "icmp.h"
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"


//TODO add new task: http server
typedef enum 
{
  URLramDysk,
  URLsdDysk,
  URLstatus,
  URLerror
} urlSource_t;


extern struct lockerSensor *lockSensors;

uint8_t verify_password(char *str);

/**
 * takes a string of url address and process it
 * @param str - part of URL string
 * @param [out] - filename or command
 * @return http source (SD, RamDysk, status)
 */
urlSource_t analyse_get_url (const char *str, char *fname);

/**
 * Enc28j60 task
 */
void encTask(void *pvParameters);

extern nicState_t nicState;
#endif
