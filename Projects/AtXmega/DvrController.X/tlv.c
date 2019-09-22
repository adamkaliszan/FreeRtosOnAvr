#include "tlv.h"

static void tlvPingFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg);

const TlvCommand_t tlvCmdList[] PROGMEM =
{
  {PING,          tlvPingFunction},
  {0,             NULL}
};


static void tlvPingFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  (void) myTlvMsg;
  sendTlvMsgDta(myTlvMsg, myTlvMsg->data, tlvInt->errStr);
  //tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
}
