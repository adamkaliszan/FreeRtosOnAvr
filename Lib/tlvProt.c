#include "tlvProt.h"

void tlvIinitializeInterpreter(tlvInterpreter_t *tlvInt, FILE *ioStr, FILE *errStr, const tlvCommand_t *commands)
{
  tlvCommand_t tmpCmd;
  memset(tlvInt, 0, sizeof(struct tlvInterpreter));

  tlvInt->ioStr  = ioStr;
  tlvInt->errStr = errStr;
  tlvInt->commands = commands;

  tlvInt->noOfCmds = -1;
  do
  {
    memcpy_P(&tmpCmd, commands, sizeof(tlvCommand_t));
    tlvInt->noOfCmds++;
    commands++;
  }
  while(tmpCmd.type != 0);
}

void tlvCalculateCrc(tlvMsg_t *message)
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

void tlvCalculateCrcSepDta(tlvMsg_t *message, const uint8_t dta[])
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

uint8_t tlvCheckCrc(tlvMsg_t *message)
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

void tlvProcessDta(tlvInterpreter_t *tlvInt, uint8_t dta)
{
  uint8_t i, j;
  struct tlvMsg *myRecMsg = (struct tlvMsg *)tlvInt->buffer;

  if (tlvInt->bufIdx >= TLV_BUF_LEN)
  {
    fprintf_P(tlvInt->errStr, PSTR("# TLV buffer overflow"));
    tlvInt->bufIdx = 0;
  }

  if (tlvInt->bufIdx == 0 && dta != TLV_SYNC)
    return;

  tlvInt->buffer[tlvInt->bufIdx] = dta;
  tlvInt->bufIdx++;

  if (tlvInt->bufIdx < sizeof(struct tlvMsg))
    return;

  if (tlvInt->bufIdx < myRecMsg->dtaLen + sizeof(struct tlvMsg))
    return;

  if (tlvCheckCrc(myRecMsg) == 0)
  {
    fprintf_P(tlvInt->errStr, PSTR("# CRC mismatch: buffer idx %d\r\n"), tlvInt->bufIdx);
    fprintf_P(tlvInt->errStr, PSTR("\taddress  : %x\r\n"), myRecMsg->address);
    fprintf_P(tlvInt->errStr, PSTR("\tmsg type : %x\r\n"), myRecMsg->type);
    fprintf_P(tlvInt->errStr, PSTR("\tcrc lo   : %x\r\n"), myRecMsg->crcLo);
    fprintf_P(tlvInt->errStr, PSTR("\tcrc hi   : %x\r\n"), myRecMsg->crcHi);
    fprintf_P(tlvInt->errStr, PSTR("\tdta len  : %x\r\n"), myRecMsg->dtaLen);

    fprintf_P(tlvInt->errStr, PSTR("\tdata:"));
    for(i=sizeof(struct tlvMsg); i<tlvInt->bufIdx; i++)
      fprintf_P(tlvInt->errStr, PSTR(" %2x"), tlvInt->buffer[i]);
    fprintf_P(tlvInt->errStr, PSTR("\r\n"));

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

  tlvCommand_t tmp;                                                     // We need to create this object. We can't directly
  for (i=0; i<tlvInt->noOfCmds; i++)
  {
      memcpy_P(&tmp, &tlvInt->commands[i], sizeof(tlvCommand_t));
      if (myRecMsg->type == tmp.type)
      {
        tmp.fun(tlvInt, myRecMsg);
        break;
      }
  }
  if (i == tlvInt->noOfCmds)
    fprintf_P(tlvInt->errStr, PSTR("! Unknown command: %d\r\n"), myRecMsg->type);
  else
    fprintf_P(tlvInt->errStr, PSTR("TLV command %x was executed\r\n"), myRecMsg->type);

  tlvInt->bufIdx = 0;
}

void sendTlvMsg(tlvMsg_t *message, FILE *ostream)
{
  int i, len;
  len = sizeof(struct tlvMsg) + message->dtaLen;
  uint8_t *ptr = (uint8_t *) message;

  for (i=0; i<len; i++)
  {
    fputc(*ptr, ostream);
    ptr++;
  }
}

void sendTlvMsgDta(tlvMsg_t *message, const uint8_t const *msgDta, FILE *ostream)
{
  int i, len;
  len = sizeof(struct tlvMsg);
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

