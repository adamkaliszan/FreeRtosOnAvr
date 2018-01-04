#ifndef ENC_TASK_H
#define ENC_TASK_H 1

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "ip_arp_udp_tcp.h"
#include "enc28j60.h"
#include "avr_compat.h"
//#include "net.h"
#include "hardwareConfig.h"

#define MYWWWPORT    80
#define MYTELNETPORT 23
#define MYUDPPORT    1200

uint8_t mymac[6];
uint8_t myip[4];
uint8_t mask;

uint8_t verify_password(char *str);

//! takes a string of the form password/commandNumber and analyse it
//! return values: -1 invalid password, otherwise command number
//!                -2 no command given but password valid
//!                -3 valid password, no command and no trailing "/"
int8_t analyse_get_url(char *str);

//! answer HTTP/1.0 301 Moved Permanently\r\nLocation: password/\r\n\r\n
//! to redirect to the url ending in a slash
uint16_t moved_perm(uint8_t *buf);

//! prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf,uint8_t on_off);

//! Enc28j60 task
void encTask(void *pvParameters);

//! initialize enc28j60
void enc28j60chipInit(void);

extern struct Enc28j60_config Enc28j60_global;
#endif
