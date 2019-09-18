#include "tlv.h"

static void tlvForwardFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg);
static void tlvBackwordFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg);
static void tlvRotateLeftFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg);
static void tlvRotateRightFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg);
static void tlvStopFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg);
static void tlvPingFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg);

const TlvCommand_t tlvCmdList[] PROGMEM =
{
  {FORWARD,       tlvForwardFunction},
  {BACKWORD,      tlvBackwordFunction},
  {ROTATE_LEFT,   tlvRotateLeftFunction},
  {ROTATE_RIGHT,  tlvRotateRightFunction},
  {STOP,          tlvStopFunction},
  {PING,          tlvPingFunction},
  {0,             NULL}
};

static void tlvForwardFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  TlvMsgMove_t *msg = (TlvMsgMove_t *) myTlvMsg;
  forwardB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void tlvBackwordFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  TlvMsgMove_t *msg = (TlvMsgMove_t *) myTlvMsg;
  backwordB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void tlvRotateLeftFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  TlvMsgMove_t *msg = (TlvMsgMove_t *) myTlvMsg;
  rotateLeftB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void tlvRotateRightFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  TlvMsgMove_t *msg = (TlvMsgMove_t *) myTlvMsg;
  rotateRightB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void tlvStopFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  (void) myTlvMsg;
  //tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  offHbridge();
}

static void tlvPingFunction(TlvInterpreter_t *tlvInt, TlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  (void) myTlvMsg;
  sendTlvMsgDta(myTlvMsg, myTlvMsg->data, tlvInt->errStr);
  //tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
}
