#include <string.h>

#include "tlvProt.h"

void tlvIinitializeInterpreter(TlvInterpreter_t *tlvInt, FILE *ioStr, FILE *errStr, const TlvCommand_t *commands)
{
  TlvCommand_t tmpCmd;
  memset(tlvInt, 0, sizeof(struct TlvInterpreter));

  tlvInt->ioStr  = ioStr;
  tlvInt->errStr = errStr;
  tlvInt->commands = commands;

  tlvInt->noOfCmds = -1;
  do
  {
#if USE_XC8
    memcpy(&tmpCmd, commands, sizeof(TlvCommand_t));
#else
    memcpy_P(&tmpCmd, commands, sizeof(TlvCommand_t));
#endif
    tlvInt->noOfCmds++;
    commands++;
  }
  while(tmpCmd.type != 0);
}

void tlvCalculateCrc(TlvMsg_t *message)
{
  uint16_t crc;
  crc = _crc16_update(0, message->address);
  crc = _crc16_update(0, message->type);
  crc = _crc16_update(0, message->dtaLen);

  uint8_t i;
  for (i=0; i<message->dtaLen; i++)
    crc = _crc16_update(0, message->data[i]);

  message->crcLo = (uint8_t) crc;
  message->crcHi = (uint8_t) crc>>8;
}

void tlvCalculateCrcSepDta(TlvMsg_t *message, const uint8_t dta[])
{
  uint16_t crc;
  crc = _crc16_update(0, message->address);
  crc = _crc16_update(0, message->type);
  crc = _crc16_update(0, message->dtaLen);

  uint8_t i;
  for (i=0; i<message->dtaLen; i++)
    crc = _crc16_update(0, dta[i]);

  message->crcLo = (uint8_t) crc;
  message->crcHi = (uint8_t) crc>>8;
}

uint8_t tlvCheckCrc(TlvMsg_t *message)
{
  uint16_t crc;
  crc = _crc16_update(0, message->address);
  crc = _crc16_update(0, message->type);
  crc = _crc16_update(0, message->dtaLen);

  uint8_t i;
  for (i=0; i<message->dtaLen; i++)
    crc = _crc16_update(0, message->data[i]);

  uint8_t crcLo = (uint8_t) crc;
  uint8_t crcHi = (uint8_t) crc>>8;

  if (message->crcLo != crcLo)
    return 0;

  if (message->crcHi != crcHi)
    return 0;

  return 1;
}

void tlvProcessDta(TlvInterpreter_t *tlvInt, uint8_t dta)
{
  uint8_t i, j;
  struct TlvMsg *myRecMsg = (struct TlvMsg *)(tlvInt->buffer);

  if (tlvInt->bufIdx >= TLV_BUF_LEN)
  {
#ifdef USE_XC8
    fprintf(tlvInt->errStr, "# TLV buffer overflow");      
#else
    fprintf_P(tlvInt->errStr, PSTR("# TLV buffer overflow"));
#endif
    tlvInt->bufIdx = 0;
  }

  if (tlvInt->bufIdx == 0 && dta != TLV_SYNC)
    return;

  tlvInt->buffer[tlvInt->bufIdx] = dta;
  tlvInt->bufIdx++;

  if (tlvInt->bufIdx < sizeof(struct TlvMsg))
    return;

  if (tlvInt->bufIdx < myRecMsg->dtaLen + sizeof(struct TlvMsg))
    return;

  if (tlvCheckCrc(myRecMsg) == 0)
  {
#ifdef USE_XC8
    fprintf(tlvInt->errStr, "# CRC mismatch: buffer idx %d\r\n", tlvInt->bufIdx);
    fprintf(tlvInt->errStr, "\taddress  : %x\r\n", myRecMsg->address);
    fprintf(tlvInt->errStr, "\tmsg type : %x\r\n", myRecMsg->type);
    fprintf(tlvInt->errStr, "\tcrc lo   : %x\r\n", myRecMsg->crcLo);
    fprintf(tlvInt->errStr, "\tcrc hi   : %x\r\n", myRecMsg->crcHi);
    fprintf(tlvInt->errStr, "\tdta len  : %x\r\n", myRecMsg->dtaLen);

    fprintf(tlvInt->errStr, "\tdata:");
    for(i=sizeof(struct TlvMsg); i<tlvInt->bufIdx; i++)
      fprintf(tlvInt->errStr, " %2x", tlvInt->buffer[i]);
    fprintf(tlvInt->errStr, "\r\n");
#else
    fprintf_P(tlvInt->errStr, PSTR("# CRC mismatch: buffer idx %d\r\n"), tlvInt->bufIdx);
    fprintf_P(tlvInt->errStr, PSTR("\taddress  : %x\r\n"), myRecMsg->address);
    fprintf_P(tlvInt->errStr, PSTR("\tmsg type : %x\r\n"), myRecMsg->type);
    fprintf_P(tlvInt->errStr, PSTR("\tcrc lo   : %x\r\n"), myRecMsg->crcLo);
    fprintf_P(tlvInt->errStr, PSTR("\tcrc hi   : %x\r\n"), myRecMsg->crcHi);
    fprintf_P(tlvInt->errStr, PSTR("\tdta len  : %x\r\n"), myRecMsg->dtaLen);

    fprintf_P(tlvInt->errStr, PSTR("\tdata:"));
    for(i=sizeof(struct TlvMsg); i<tlvInt->bufIdx; i++)
      fprintf_P(tlvInt->errStr, PSTR(" %2x"), tlvInt->buffer[i]);
    fprintf_P(tlvInt->errStr, PSTR("\r\n"));
#endif
    for (i=1; i<tlvInt->bufIdx; i++)
    {
      if (tlvInt->buffer[i] == TLV_SYNC)
      {
        for (j=0; i+j < tlvInt->bufIdx; j++)
          tlvInt->buffer[j] = tlvInt->buffer[i+j];

        tlvInt->bufIdx -= i;
        return;
      }
    }
    tlvInt->bufIdx = 0;
    return;
  }

  TlvCommand_t tmp;                                                     // We need to create this object. We can't directly
  for (i=0; i<tlvInt->noOfCmds; i++)
  {
#if USE_XC8
      memcpy(&tmp, &tlvInt->commands[i], sizeof(TlvCommand_t));
#else
      memcpy_P(&tmp, &tlvInt->commands[i], sizeof(TlvCommand_t));
#endif
      if (myRecMsg->type == tmp.type)
      {
        tmp.fun(tlvInt, myRecMsg);
        break;
      }
  }
#if USE_XC8
  if (i == tlvInt->noOfCmds)
    fprintf(tlvInt->errStr, "! Unknown command: %d\r\n", myRecMsg->type);
  else
    fprintf(tlvInt->errStr, "TLV command %x was executed\r\n", myRecMsg->type);  
#else
  if (i == tlvInt->noOfCmds)
    fprintf_P(tlvInt->errStr, PSTR("! Unknown command: %d\r\n"), myRecMsg->type);
  else
    fprintf_P(tlvInt->errStr, PSTR("TLV command %x was executed\r\n"), myRecMsg->type);
#endif
  tlvInt->bufIdx = 0;
}

void sendTlvMsg(TlvMsg_t *message, FILE *ostream)
{
  int i, len;
  len = sizeof(struct TlvMsg) + message->dtaLen;
  uint8_t *ptr = (uint8_t *) message;

  for (i=0; i<len; i++)
  {
    fputc(*ptr, ostream);
    ptr++;
  }
}

void sendTlvMsgDta(TlvMsg_t *message, const uint8_t const *msgDta, FILE *ostream)
{
  int i, len;
  len = sizeof(struct TlvMsg);
  uint8_t const *ptr = (uint8_t *) message;

  for (i=0; i<len; i++)
  {
    fputc(*ptr, ostream);
    ptr++;
  }

  ptr = msgDta;
  len = message->dtaLen;
  for (i=0; i<len; i++)
  {
    fputc(*ptr, ostream);
    ptr++;
  }
}

