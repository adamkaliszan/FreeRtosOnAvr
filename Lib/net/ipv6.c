/**
 * Deklaracje glownych funkcji ipv6
 */

#include "ipv6.h"
#include "../../freeRtos/Lib/net/include/ipv6.h"

void ipv6Init(void)
{
  //Ipv6MyConfig.ip = xmalloc(sizeof(uip_ip6addr_t));
  //Ipv6MyConfig.prefix= 64;
  //Ipv6MyConfig.gateway = xmalloc(sizeof(uip_ip6addr_t));
}

void netstackIPv6Process(void)
{
  //uip_ip6addr_t *ipv6addr;
  #if IP_DEBUG
  fprintf_P(debugStream, PSTR("*IPv6 packet in. Length: %d\r\n"), plen);
  netPrintIpv6Header(debugStream);
  //netPrintIpv6HeaderRAW(debugStream);
  netPrintIpv6PayloadRAW(debugStream);
  ipv6PrintConfig(debugStream, uip_netif_physical_if);
  #endif /*IP_DEBUG*/  

  if(checkPacketLen() == 1)
  {
    
        
     // if(!uip_netif_is_addr_my_unicast(&UIP_IP_BUF->destipaddr) &&
     // !uip_netif_is_addr_my_solicited(&UIP_IP_BUF->destipaddr) &&
     // !uip_is_addr_linklocal_allnodes_mcast(&UIP_IP_BUF->destipaddr)){
     // UIP_STAT(++uip_stat.ip.drop);
     // goto drop;
  

      if(UIP_ICMP_BUF->type==ICMP6_ECHO_REQUEST)
      {
	//reply
	uip_icmp6_echo_request_input();
	swithEthAddresses();
	nicSend(plen);
      }
      if(UIP_ICMP_BUF->type==ICMP6_NS)
      {
	//reply ICMP6_NA
	fprintf_P(debugStream, PSTR("NS, length=%d, \r\n"), plen);    
	for (uint8_t i=0; i < plen; i++)
	{
	  if (i!=0 && i%16==0) fprintf_P(debugStream, PSTR("\r\n"));
	  fprintf_P(debugStream, PSTR("%02x:"), nicState.layer2.buf[i]);
	}
	fprintf_P(debugStream, PSTR("\r\n"));
	uip_nd6_io_ns_input(plen);
	swithEthAddresses();
	fprintf_P(debugStream, PSTR("Packet out, length=%d, \r\n"), plen);    
	for (uint8_t i=0; i < plen; i++)
	{
	  if (i!=0 && i%16==0) fprintf_P(debugStream, PSTR("\r\n"));
	  fprintf_P(debugStream, PSTR("%02x:"), nicState.layer2.buf[i]);
	}
	fprintf_P(debugStream, PSTR("\r\n"));
	nicSend(86);
      }
      if(UIP_IP_BUF->proto==IP_PROTO_TCP)
      {
	fprintf_P(debugStream, PSTR("TCP packet in, \r\n"));    
	netstackTCPIPProcess6();
      }
  }
  else 
  {
    #if IP_DEBUG
    fprintf_P(debugStream, PSTR("ip: packet shorter than reported in IP header.\n\r"));
    #endif /*IP_DEBUG*/  
  }
   
}

void swithEthAddresses(void)
{
   /* fprintf_P(debugStream, PSTR("Packet out, length=%d, \r\n"), plen);    
    for (uint8_t i=0; i < plen; i++)
    {
      if (i!=0 && i%16==0) fprintf_P(debugStream, PSTR("\r\n"));
      fprintf_P(debugStream, PSTR("%02x:"), nicState.layer2.buf[i]);
    }
    fprintf_P(debugStream, PSTR("\r\n"));
    */
    memcpy(&nicState.layer2.ethHeader->dest, &nicState.layer2.ethHeader->src, 6);
    memcpy(&nicState.layer2.ethHeader->src, &nicState.mac, 6);
    nicState.layer2.ethHeader->type = 0xdd86;
    /*
    fprintf_P(debugStream, PSTR("Packet out, length=%d, \r\n"), plen);    
    for (uint8_t i=0; i < plen; i++)
    {
      if (i!=0 && i%16==0) fprintf_P(debugStream, PSTR("\r\n"));
      fprintf_P(debugStream, PSTR("%02x:"), nicState.layer2.buf[i]);
    }
    fprintf_P(debugStream, PSTR("\r\n"));
    */
}

void ipv6DebugInit(FILE* inDebugStream)
{
  debugStream = inDebugStream;
}

uint8_t checkPacketLen()
{
  // 1 - packet ok, 0 - wrong length
  if(plen <= (UIP_IP_BUF->len[0] << 8) + UIP_IP_BUF->len[1])
    return 0;
  else
    return 1;
  
}

void netPrintIpv6HeaderRAW(FILE *stream)
{
  fprintf_P(stream, PSTR("*IPv6 Header RAW:\r\n"));    
  fprintf_P(stream, PSTR("\t"));    
  for (uint8_t i=0; i < UIP_IPv6H_LEN; i++)
  {
    if (i!=0 && i%16==0) 
    {
      fprintf_P(stream, PSTR("\r\n"));
      fprintf_P(stream, PSTR("\t"));
    }
    fprintf_P(stream, PSTR("%02x:"), nicState.layer3.buf[i]);
  }
  fprintf_P(stream, PSTR("\r\n")); 
}

void netPrintIpv6Header(FILE *stream)
{
    fprintf_P(stream, PSTR("*IPv6 Header:\r\n"));    
    fprintf_P(stream, PSTR("\tVersion: \t\t"));
    fprintf_P(stream, PSTR("\t%d \n\r"), (nicState.layer3.ipv6->vtc >> 4));
    fprintf_P(stream, PSTR("\tTraffic Class: \t\t"));
    fprintf_P(stream, PSTR("\t%d \n\r"), nicState.layer3.ipv6->vtc & 0x0F + (nicState.layer3.ipv6->tcflow >> 4));
    fprintf_P(stream, PSTR("\tFlow Label: \t\t"));
    fprintf_P(stream, PSTR("\t%d \n\r"), nicState.layer3.ipv6->tcflow & 0x0F + (nicState.layer3.ipv6->flow << 4));
    fprintf_P(stream, PSTR("\tPayload Length: \t"));
    fprintf_P(stream, PSTR("\t%d \n\r"), nicState.layer3.ipv6->len[1] + (nicState.layer3.ipv6->len[0] << 8));
    fprintf_P(stream, PSTR("\tSource address: \t"));
    PRINT6ADDR(stream, &nicState.layer3.ipv6->srcipaddr);
    fprintf_P(stream, PSTR("\tDestination address: \t"));
    PRINT6ADDR(stream, &nicState.layer3.ipv6->destipaddr);
}

void netPrintIpv6PayloadRAW(FILE *stream)
{
  uint16_t payloadLen=nicState.layer3.ipv6->len[1] + (nicState.layer3.ipv6->len[0] << 8);
  fprintf_P(stream, PSTR("*IPv6 Payload RAW:\r\n"));
  fprintf_P(stream, PSTR("\t"));    
  for (uint8_t i=0; i <  payloadLen; i++)
  {
    if (i!=0 && i%16==0) 
    {
      fprintf_P(stream, PSTR("\r\n"));
      fprintf_P(stream, PSTR("\t"));
    }
    fprintf_P(stream, PSTR("%02x:"), nicState.layer3.buf[i + UIP_IPv6H_LEN]);
  }
  fprintf_P(stream, PSTR("\r\n")); 
}

void ipv6PrintConfig(FILE *stream, struct uip_netif* config)
{
  //Currently not all settings are printed
  fprintf_P(stream, PSTR("*Interface IPv6 config:\n\r"));
  uint8_t i;
  fprintf_P(stream, PSTR("\tAddresses:\n\r"));
  for(i = 0; i < UIP_CONF_NETIF_MAX_ADDRESSES; i ++)
    if(config->addresses[i].state != NOT_USED)
    {
      fprintf_P(stream, PSTR("\tAddress %d:\t\t"), i);
      PRINT6ADDR(stream, &config->addresses[i].ipaddr);    
    }
  fprintf_P(stream, PSTR("\tSolicited node mcast address:"));
  PRINT6ADDR(stream, &config->solicited_node_mcastaddr);    
}

void ipv6Send(uint8_t protocol, uint16_t len)
{
// make pointer to ethernet/IP header
//#if IP_DEBUG
  //if (debugStream != NULL)
  {  
    //if (IpMyConfig.dbgLevel > 2)
      fprintf_P(debugStream, PSTR("Sending Ip packet\r\n"));
  }
//#endif
  
  //Exchange eth addresses
  swithEthAddresses();

  // adjust length to add IP header
  //len += IP_HEADER_LEN;

  // fill IP header
  
  uip_ip6addr_t *tmp_ipaddr;
  tmp_ipaddr = xmalloc(sizeof(uip_ip6addr_t));
  memcpy(tmp_ipaddr, &UIP_IP_BUF->srcipaddr, sizeof(uip_ip6addr_t));
  memcpy(&UIP_IP_BUF->srcipaddr, &UIP_IP_BUF->destipaddr, sizeof(uip_ip6addr_t));
  memcpy(&UIP_IP_BUF->destipaddr, tmp_ipaddr, sizeof(uip_ip6addr_t));
    
  
  UIP_IP_BUF->vtc = 0x60;
  UIP_IP_BUF->tcflow = 0;
  UIP_IP_BUF->flow = 0;
  UIP_IP_BUF->len[0] = (len & 0xFF00); /* more than 255 */
  UIP_IP_BUF->len[1] = (len & 0x00FF);
  UIP_IP_BUF->proto = protocol;
  UIP_IP_BUF->ttl = IP_TIME_TO_LIVE;
  
  
  ///UIP_TCP_BUF->tcpchksum = 0;
  ///UIP_TCP_BUF->tcpchksum = 0;

// adjust length to add ethernet and IPv6 header
  len += ETH_HEADER_LEN + UIP_IPH_LEN;

// send it
  nicSend(len);
}

//void ipv6Send(uint32_t dstIpv6, uint8_t protocol, uint16_t len);
//void netPrintIpv6Header(FILE *stream, struct netIpv6Header* ipheader);
//void ipSetConfigIpv6(uint32_t myIp);