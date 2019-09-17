#ifndef VTY_H
#define VTY_H

#include "main.h"
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/crc16.h>

#include "enc28j60.h"
#include "memory_x.h"
#include "configuration.h"
#include "nic.h"
#include "ip.h"
#include "net.h"
#include "arp.h"
#include "cmdline.h"
#include "udp.h"

#include "spi_task.h"


// Znaki kontrolne w protokole Xmodem
#define SOH                     0x01
#define STX                     0x02
#define EOT                     0x04
#define ACK                     0x06
#define NAK                     0x15
#define CAN                     0x18
#define CTRLZ                   0x1A

// xmodem timeout/retry parameters
#define XMODEM_RETRY_LIMIT      16

// error return codes
#define XMODEM_ERROR_REMOTECANCEL       1
#define XMODEM_ERROR_OUTOFSYNC          2
#define XMODEM_ERROR_RETRYEXCEED        3

#define XMODEM_BUFFER_SIZE		128


extern nicState_t  nicState;
extern UdpSocket_t *udpSocket;

void VtyInit(cmdState_t *state, FILE *stream);

void printErrorInfo(cmdState_t *state);

void printStatus(FILE *stream);

extern volatile uint8_t temperature;
extern volatile uint8_t voltage;




extern struct Enc28j60_config Enc28j60_global;

extern uint8_t NoOfSerialPorts;
extern uint8_t NoOfSpiSlaves;

enum errorType
{
  AllOK  = 0,
  noFile = 1,
  xModemFrameStartTimeout = 2,
  xModemByteSendTimeout = 3,
  xModemWrongFrameNo = 4,
  xModemFrameFrameNoCorrectionNotMatch = 5,
  xModemFrameCrc = 6,
  xModemRemoteSideCan = 7,
  xModemUnknownResponse = 8,
  noRemoteDevice = 9,
  bootloaderNotResponding = 10,
  cantOpenFile = 11
};

typedef enum errorType errorType_t;

#endif
