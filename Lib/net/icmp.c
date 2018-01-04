/*! \file icmp.c \brief ICMP Protocol Library. */
//*****************************************************************************
//
// File Name	: 'icmp.c'
// Title		: ICMP (Internet Control Message Protocol) Protocol Library
// Author		: Pascal Stang
// Created		: 9/10/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************


#include "icmp.h"


//extern void nicSend(unsigned int len, unsigned char* packet);

// global variables


// functions
void icmpInit(void)
{
  icmpDebug = NULL;
  icmpDebugLevel = 0;
}

void icmpIpIn(void)
{
  // check ICMP type
  switch(nicState.layer4.icmp->type)
  {
    case ICMP_TYPE_ECHOREQUEST:
      icmpEchoRequest();       // echo request
      break;
    default:
#if ICMP_DEBUG
      if (icmpDebug != NULL)
      {
        if (icmpDebugLevel > 0)
          fprintf_P(icmpDebug, PSTR("Unknown ICMP typeReceived ICMP request: "));
      }
#endif
      break;
  }
}

void icmpEchoRequest(void)
{
#if ICMP_DEBUG
  if (icmpDebug != NULL)
  {
    if (icmpDebugLevel > 1)
      fprintf_P(icmpDebug, PSTR("Received ICMP request: "));
  }
#endif
  uint32_t tempIp;

  // change type to reply
  nicState.layer4.icmp->type = ICMP_TYPE_ECHOREPLY;
  // recalculate checksum
  nicState.layer4.icmp->icmpchksum = 0;
  nicState.layer4.icmp->icmpchksum = netChecksum((uint8_t*)(nicState.layer4.icmp), htons(nicState.layer3.ip->len)-IP_HEADER_LEN);
  // return to sender
  tempIp = nicState.layer3.ip->destipaddr;
  nicState.layer3.ip->destipaddr = nicState.layer3.ip->srcipaddr;
  nicState.layer3.ip->srcipaddr = tempIp;
  // add ethernet routing
  arpIpOut(0);

  // debugging
  if (icmpDebug != NULL)
    icmpPrintHeader(icmpDebug, nicState.layer3.ip, nicState.layer4.icmp);
  //debugPrintHexTable(htons(packet->ip.len), (uint8_t*)packet);

  // send it (packet->ip.len+ETH_HEADER_LEN
  nicSend(htons(nicState.layer3.ip->len) + ETH_HEADER_LEN);
#if ICMP_DEBUG
  if (icmpDebug != NULL)
  {
    if (icmpDebugLevel > 1)
      fprintf_P(icmpDebug, PSTR("Sending ICMP PONG\r\n"));
  }
#endif
}

#if ICMP_DEBUG
void setIcmpDebug(FILE *stream, uint8_t level)
{
  icmpDebug = stream;
  icmpDebugLevel = level;
}
#endif

void icmpPrintHeader(FILE *stream, struct netIpHeader *ipPacket, struct netIcmpHeader *icmpPacket)
{
  fprintf_P(stream, PSTR("ICMP Packet:\r\n"));
// print source IP address
  fprintf_P(stream, PSTR("SrcIpAddr: "));  netPrintIPAddr(stream, ipPacket->srcipaddr);  fprintf_P(stream, PSTR("\r\n"));
// print dest IP address
  fprintf_P(stream, PSTR("DstIpAddr: "));  netPrintIPAddr(stream, ipPacket->destipaddr); fprintf_P(stream, PSTR("\r\n"));
// print type
  fprintf_P(stream, PSTR("Type: 0x%x    "), icmpPacket->type);
//switch(icmpPacket->type)
//{
//  case ICMP_TYPE_ECHOREQUEST:
//    fprintf_P(stream, PSTR("E REQ"));
//    break;
//  case ICMP_TYPE_ECHOREPLY:
//    fprintf_P(stream, PSTR("E REP"));
//    break;
//  default:
//    fprintf_P(stream, ("???"));
//    break;
//}
//  fprintf_P(stream, PSTR("\r\n"));
// print code
  fprintf_P(stream, PSTR("Code   : 0x%x\r\n"), icmpPacket->icode);
}

