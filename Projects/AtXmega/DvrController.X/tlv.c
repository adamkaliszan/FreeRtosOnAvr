#include "tlv.h"

static void tlvPingFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);

const tlvCommand_t tlvCmdList[] PROGMEM =
{
  {PING,          tlvPingFunction},
  {0,             NULL}
};


static void tlvPingFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  (void) myTlvMsg;
  sendTlvMsgDta(myTlvMsg, myTlvMsg->data, tlvInt->errStr);
  //tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
}
