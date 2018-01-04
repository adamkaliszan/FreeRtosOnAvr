#ifndef TLV_PROT
#define TLV_PROT

#include <stdint.h>
#include <util/crc16.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>


#include "FreeRTOS.h"
#include "hardwareConfig.h"

#define TLV_SYNC 0x55

#ifndef TLV_BUF_LEN
#error "Define TLV_BUF_LEN in hardwareConfig.h file\nRecomended value 16-64"
#endif //TLV_BUF_LEN
#if TLV_BUF_LEN < 16
#error "TLV bufer is too small"
#endif // TLV_BUF_LEN
#if TLV_BUF_LEN > 64
#warning "TLV bufer is more than 64 bytes"
#endif // TLV_BUF_LEN


struct tlvInterpreter;

typedef struct tlvInterpreter tlvInterpreter_t;

typedef struct tlvMsg
{
  uint8_t sync;     // = TLV_SYNC
  uint8_t address;  // Device address
  uint8_t type;
  uint8_t dtaLen;
  uint8_t crcLo;
  uint8_t crcHi;
  uint8_t data[0];
} tlvMsg_t;


typedef struct tlvCommand
{
  uint8_t   type;
  void (*fun)(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
} tlvCommand_t;

struct tlvInterpreter
{
  uint8_t            buffer[TLV_BUF_LEN];
  uint8_t            bufIdx;
  const tlvCommand_t *commands;
  uint8_t            noOfCmds;
  FILE               *ioStr;
  FILE               *errStr;
};

void tlvIinitializeInterpreter(tlvInterpreter_t *tlvInt, FILE *ioStr, FILE *errStr, const tlvCommand_t *commands);

void tlvCalculateCrc(tlvMsg_t *message);
void tlvCalculateCrcSepDta(tlvMsg_t *message, const uint8_t dta[]);

uint8_t tlvCheckCrc(tlvMsg_t *message);

void tlvProcessDta(tlvInterpreter_t *tlvInt, uint8_t dta);
void sendTlvMsg(tlvMsg_t *message, FILE *ostream);
void sendTlvMsgDta(tlvMsg_t *message, const uint8_t const *msgDta, FILE *ostream);


#endif
