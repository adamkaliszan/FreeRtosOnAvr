#include "tlv.h"

static void tlvForwardFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void tlvBackwordFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void tlvRotateLeftFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void tlvRotateRightFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void tlvStopFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);
static void tlvPingFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg);

const tlvCommand_t tlvCmdList[] PROGMEM =
{
  {FORWARD,       tlvForwardFunction},
  {BACKWORD,      tlvBackwordFunction},
  {ROTATE_LEFT,   tlvRotateLeftFunction},
  {ROTATE_RIGHT,  tlvRotateRightFunction},
  {STOP,          tlvStopFunction},
  {PING,          tlvPingFunction},
  {0,             NULL}
};

static void tlvForwardFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  forwardB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void tlvBackwordFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  backwordB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void tlvRotateLeftFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  rotateLeftB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void tlvRotateRightFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  rotateRightB(msg->data.pwmLeft, msg->data.pwmRight);
}

static void tlvStopFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  (void) myTlvMsg;
  //tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
  offHbridge();
}

static void tlvPingFunction(tlvInterpreter_t *tlvInt, tlvMsg_t *myTlvMsg)
{
  (void) tlvInt;
  (void) myTlvMsg;
  sendTlvMsgDta(myTlvMsg, myTlvMsg->data, tlvInt->errStr);
  //tlvMsgMove_t *msg = (tlvMsgMove_t *) myTlvMsg;
}
