/**
 * @file          net.c
 * @author        Pascal Stang, Adam Kaliszan
 * @brief         Network support library.
 * @version       0.1
 * Created        30.08.2004
 * Revised        25.11.2010
 * Editor Tabs    2
 */

#include "net.h"


uint16_t ntohs(uint16_t val)
{
  return (val<<8) | (val>>8);
}

uint16_t htons(uint16_t val)
{
  return (val<<8) | (val>>8);
}

uint32_t htonl(uint32_t val)
{
  return (htons(val>>16) | (uint32_t)htons(val&0x0000FFFF)<<16);
}

uint32_t ntohl(uint32_t val)
{
  return (ntohs(val>>16) | (uint32_t)ntohs(val&0x0000FFFF)<<16);
}


uint16_t netChecksum(uint8_t *data, uint16_t len)
{
  register uint32_t sum = 0;

  for (;;)
  {
    if (len < 2)
      break;
//sum += *((uint16_t *)data)++;
    sum += *((uint16_t *)data);
    data+=2;
    len -= 2;
  }
  if (len)
    sum += *(uint8_t *) data;

  while ((len = (uint16_t) (sum >> 16)) != 0)
    sum = (uint16_t) sum + len;

  return (uint16_t) sum ^ 0xFFFF;
}

void netPrintEthAddr(FILE *stream, struct netEthAddr* ethaddr)
{
  fprintf_P(stream, PSTR("%02x:%02x:%02x:%02x:%02x:%02x"), ethaddr->addr[0], ethaddr->addr[1], ethaddr->addr[2], ethaddr->addr[3], ethaddr->addr[4], ethaddr->addr[5]);
}

void netPrintIPAddr(FILE *stream, uint32_t ipaddr)
{
  fprintf_P(stream, PSTR("%d.%d.%d.%d"), ((unsigned char*)&ipaddr)[0], ((unsigned char*)&ipaddr)[1], ((unsigned char*)&ipaddr)[2], ((unsigned char*)&ipaddr)[3]);
}

void netPrintEthHeader(FILE *stream, struct netEthHeader* eth_hdr)
{
  fprintf_P(stream, PSTR("Eth Packet Type: 0x%x"), eth_hdr->type);
  fprintf_P(stream, PSTR(" SRC:"));
  netPrintEthAddr(stream, &eth_hdr->src);
  fprintf_P(stream, PSTR("->DST:"));
  netPrintEthAddr(stream, &eth_hdr->dest);
}

void netPrintIpHeader(FILE *stream, struct netIpHeader* ipheader)
{
  fprintf_P(stream, PSTR("IP Header\r\n"));
  fprintf_P(stream, PSTR("Ver     : %d\r\n"), (ipheader->vhl)>>4);
  fprintf_P(stream, PSTR("Length  : %d\r\n"), htons(ipheader->len));
  if(ipheader->proto == IP_PROTO_ICMP)
    fprintf_P(stream, PSTR("Protocol: ICMP\r\n"));
  else if(ipheader->proto == IP_PROTO_TCP)
    fprintf_P(stream, PSTR("Protocol: TCP\r\n"));
  else if(ipheader->proto == IP_PROTO_UDP)
    fprintf_P(stream, PSTR("Protocol: UDP\r\n"));
  else
    fprintf_P(stream, PSTR("Protocol: %d\r\n"), ipheader->proto);

  fprintf_P(stream, PSTR("SourceIP: ")); netPrintIPAddr(stream, htonl(ipheader->srcipaddr));   fprintf_P(stream, PSTR("\r\n"));
  fprintf_P(stream, PSTR("Dest  IP: ")); netPrintIPAddr(stream, htonl(ipheader->destipaddr));  fprintf_P(stream, PSTR("\r\n"));
}

void netPrintTcpHeader(FILE *stream, struct netTcpHeader* tcpheader)
{
  fprintf_P(stream, PSTR("TCP Header\r\n"));
  fprintf_P(stream, PSTR("Src Port: %d\r\n"), htons(tcpheader->srcport));
  fprintf_P(stream, PSTR("Dst Port: %d\r\n"), htons(tcpheader->destport));
  fprintf_P(stream, PSTR("Seq Num : 0x%x"), htonl(tcpheader->seqno));
  fprintf_P(stream, PSTR("Ack Num : 0x%x\r\n"), htonl(tcpheader->ackno));
  fprintf_P(stream, PSTR("Flags   : "));
  if(tcpheader->flags & TCP_FLAGS_FIN)
    fprintf_P(stream, PSTR("FIN "));
  if(tcpheader->flags & TCP_FLAGS_SYN)
    fprintf_P(stream, PSTR("SYN "));
  if(tcpheader->flags & TCP_FLAGS_RST)
    fprintf_P(stream, PSTR("RST "));
  if(tcpheader->flags & TCP_FLAGS_PSH)
    fprintf_P(stream, PSTR("PSH "));
  if(tcpheader->flags & TCP_FLAGS_ACK)
    fprintf_P(stream, PSTR("ACK "));
  if(tcpheader->flags & TCP_FLAGS_URG)
    fprintf_P(stream, PSTR("URG "));
  fprintf_P(stream, PSTR("\r\n"));
}


