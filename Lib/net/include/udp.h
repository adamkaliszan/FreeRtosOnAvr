#ifndef UDP_H
#define UDP_H

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "ip.h"
#include "nic.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "softwareConfig.h"
#include "hardwareConfig.h"
#include "memory_x.h"
#include <stdio.h>
#include <avr/pgmspace.h>

#if UDP_DEBUG
#endif

extern uint32_t udpIpDst_eep   __attribute__((section (".eeprom")));
extern uint16_t udpPortDstEep  __attribute__((section (".eeprom")));
extern uint16_t udpPortSrcEep  __attribute__((section (".eeprom")));

extern struct ipConfig  IpMyConfig;
extern nicState_t       nicState;

typedef struct
{
  uint16_t dstPortDef;           //stored in network order
  uint16_t dstPort;              //stored in network order
  uint16_t srcPort;
  uint32_t dstIp;

  xQueueHandle Rx;
  xQueueHandle Tx;
} UdpSocket_t;

UdpSocket_t *udpSocket;//[NUMBER_OF_UDP_SOCK];

#if UDP_DEBUG
FILE *udpDbgStream;
uint8_t udpDbgLevel;

/**
 * Enable or disable debug stream
 * @param *stream - output stream. Do not use network stream. NULL value disable debug stream
 * @param level   - debug detail's level
 */
void setUdpDebug(FILE *stream, uint8_t level);
#endif


void udpLoadConfig(void);

/**
 * Initialize UDP protocol.
 * Creates UDP socket
 * Invoke this function on the very begining
 */
void udpInit_0(void);


/**
 * Send a UDP/IP packet.
 * Packet data has to be written in Ethernet buffer.
 * Function fills the IP addresses and ports according to UDP settings
 * @param len - udp packet length (with header)
 */
void udpSend(uint16_t len);


void netstackUDPIPProcess(void);

void flushUdpQueues(void);

void udpSaveConfig(void);
void udpPrintStatus(FILE *stream);

#endif
