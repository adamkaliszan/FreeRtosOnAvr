/**
 * @file ip.c
 * @brief IP (Internet Protocol) Library.
 */
//*****************************************************************************
//
// File Name	: 'ip.c'
// Title		: IP (Internet Protocol) Library
// Author		: Pascal Stang
// Created		: 8/30/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 2
//
//*****************************************************************************

#include "ip.h"

static uint32_t myip_eep    __attribute__((section (".eeprom"))) = ((uint32_t)MY_IP4   << 24) + ((uint32_t)MY_IP3   <<16) + ((uint32_t)MY_IP2   <<8) + MY_IP1;
static uint32_t mask_eep    __attribute__((section (".eeprom"))) = ((uint32_t)MY_MASK4 << 24) + ((uint32_t)MY_MASK3 <<16) + ((uint32_t)MY_MASK2 <<8) + MY_MASK1;
static uint32_t defGw_eep   __attribute__((section (".eeprom"))) = ((uint32_t)MY_GW4   << 24) + ((uint32_t)MY_GW3   <<16) + ((uint32_t)MY_GW2   <<8) + MY_GW1;

void ipInit()
{

}

void ipLoadConfig(void)
{
  IpMyConfig.ip      = eeprom_read_dword(&myip_eep);
  IpMyConfig.netmask = eeprom_read_dword(&mask_eep);
  IpMyConfig.gateway = eeprom_read_dword(&defGw_eep);
#if IP_DEBUG
  IpMyConfig.dbgLevel = 0;
  IpMyConfig.dbgStream = NULL;
#endif
}

void ipSaveConfig(void)
{
  eeprom_update_dword(&myip_eep,  IpMyConfig.ip);
  eeprom_update_dword(&mask_eep,  IpMyConfig.netmask);
  eeprom_update_dword(&defGw_eep, IpMyConfig.gateway);
}

inline void netstackIPv4Process(void)
{
// check IP addressing, stop processing if not for me and not a broadcast
  if( (nicState.layer3.ip->destipaddr != ipGetConfig()->ip) &&                                   //Różne adresy IP
      (nicState.layer3.ip->destipaddr != (ipGetConfig()->ip | (~ipGetConfig()->netmask))) &&     //Nie jest to adres rozgłoszeniowy sieci
      (nicState.layer3.ip->destipaddr != 0xFFFFFFFF))                                            //Nie jest to brodcast
    return;

// handle ICMP packet
  if(nicState.layer3.ip->proto == IP_PROTO_ICMP)
  {
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
    {
      if (IpMyConfig.dbgLevel > 0)
        fprintf_P(IpMyConfig.dbgStream, PSTR("NET Rx: ICMP/IP packet\r\n"));
      if (IpMyConfig.dbgLevel > 2)
        icmpPrintHeader(IpMyConfig.dbgStream, nicState.layer3.ip, nicState.layer4.icmp);
    }
#endif /*IP_DEBUG*/
    icmpIpIn();
    return;
  }
  if( nicState.layer3.ip->proto == IP_PROTO_UDP )
  {
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
    {
      if (IpMyConfig.dbgLevel > 0)
        fprintf_P(IpMyConfig.dbgStream, PSTR("NET Rx: UDP/IP packet\r\n"));
    }
#endif /*IP_DEBUG*/
    netstackUDPIPProcess();
    return;
  }
#ifdef TCP_H
  if( nicState.layer3.ip->proto == IP_PROTO_TCP )
  {
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
    {
      if (IpMyConfig.dbgLevel > 0)
        fprintf_P(IpMyConfig.dbgStream, PSTR("NET Rx: TCP/IP packet\r\n"));
    }
#endif /*IP_DEBUG*/
    netstackTCPIPProcess();
    return;
  }
#endif
#if IP_DEBUG
  if (IpMyConfig.dbgStream != NULL)
  {
    if (IpMyConfig.dbgLevel > 0)
      fprintf_P(IpMyConfig.dbgStream, PSTR("NET Rx: Unknown IP packet\r\n"));
  }
#endif /*IP_DEBUG*/
}

#if IP_DEBUG
void setIpDebug(FILE *stream, uint8_t level)
{
  IpMyConfig.dbgStream = stream;
  IpMyConfig.dbgLevel = level;
  if (level == 0)
    IpMyConfig.dbgStream = NULL;
}
#endif

void ipSetConfig(uint32_t myIp, uint32_t netmask, uint32_t gatewayIp)
{
  // set local addressing
  IpMyConfig.ip = myIp;
  IpMyConfig.netmask = netmask;
  IpMyConfig.gateway = gatewayIp;
}

void ipSetConfigIp(uint32_t myIp)
{
  // set local addressing
  IpMyConfig.ip = myIp;
}

void ipSetConfigMask(uint32_t netmask)
{
  IpMyConfig.netmask = netmask;
}

void ipSetConfigGw(uint32_t gatewayIp)
{
  IpMyConfig.gateway = gatewayIp;
}


struct ipConfig* ipGetConfig(void)
{
  return &IpMyConfig;
}

void ipSend(uint32_t dstIp, uint8_t protocol, uint16_t len)
{
// make pointer to ethernet/IP header
#if IP_DEBUG
  if (IpMyConfig.dbgStream != NULL)
  {
    if (IpMyConfig.dbgLevel > 2)
      fprintf_P(IpMyConfig.dbgStream, "Sending Ip packet\r\n");
  }
#endif

// adjust length to add IP header
  len += IP_HEADER_LEN;

// fill IP header
  nicState.layer3.ip->destipaddr = dstIp;
  nicState.layer3.ip->srcipaddr  = IpMyConfig.ip;
  nicState.layer3.ip->proto      = protocol;
  nicState.layer3.ip->len        = htons(len);
  nicState.layer3.ip->vhl        = 0x45;
  nicState.layer3.ip->tos        = 0;
  nicState.layer3.ip->ipid       = 0;
  nicState.layer3.ip->ipoffset   = 0;
  nicState.layer3.ip->ttl        = IP_TIME_TO_LIVE;
  nicState.layer3.ip->ipchksum   = 0;

// calculate and apply IP checksum
// DO THIS ONLY AFTER ALL CHANGES HAVE BEEN MADE TO IP HEADER
  nicState.layer3.ip->ipchksum   = netChecksum((uint8_t *)(nicState.layer3.ip), IP_HEADER_LEN);

// add ethernet routing
// check if we need to send to gateway
  if( (dstIp & IpMyConfig.netmask) == (IpMyConfig.ip & IpMyConfig.netmask) )
  {
    arpIpOut(0);  // local send
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
      fprintf_P(IpMyConfig.dbgStream, PSTR("Sending IP packet on local net\r\n"));
#endif
  }
  else
  {
    arpIpOut(IpMyConfig.gateway);  // gateway send
#if IP_DEBUG
    if (IpMyConfig.dbgStream != NULL)
      fprintf_P(IpMyConfig.dbgStream, PSTR("Sending IP packet to gateway\r\n"));
#endif
  }

// adjust length to add ethernet header
  len += ETH_HEADER_LEN;

#if IP_DEBUG
  if (IpMyConfig.dbgStream != NULL)
  {
    if (IpMyConfig.dbgLevel > 3)
    {
      fprintf_P(IpMyConfig.dbgStream, PSTR("debugPrintHexTable(ETH_HEADER_LEN, &data[0]);"));
      fprintf_P(IpMyConfig.dbgStream, PSTR("debugPrintHexTable(len-ETH_HEADER_LEN, &data[ETH_HEADER_LEN]);"));
    }
  }
#endif
// send it
  nicSend(len);
}

void ipPrintConfig(FILE *stream, struct ipConfig* config)
{
  fprintf_P(stream, PSTR("IP Addr : ")); netPrintIPAddr(stream, config->ip);        fprintf_P(stream, PSTR("\r\n"));
  fprintf_P(stream, PSTR("Netmask : ")); netPrintIPAddr(stream, config->netmask);   fprintf_P(stream, PSTR("\r\n"));
  fprintf_P(stream, PSTR("Gateway : ")); netPrintIPAddr(stream, config->gateway);   fprintf_P(stream, PSTR("\r\n"));
}
