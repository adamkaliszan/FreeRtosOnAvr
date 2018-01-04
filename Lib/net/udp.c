#include "udp.h"

void udpLoadConfig()
{
  udpSocket->dstIp      = eeprom_read_dword(&udpIpDst_eep);
  udpSocket->dstPortDef = eeprom_read_word(&udpPortDstEep);
  udpSocket->srcPort    = eeprom_read_word(&udpPortSrcEep);
}

void udpInit_0(void)
{
#if UDP_DEBUG
  udpDbgStream         = NULL;
  udpDbgLevel          = 0;
#endif
  udpSocket = xmalloc(sizeof(UdpSocket_t));

  udpSocket->Rx         = xQueueCreateExternal(255, 1, (void *)(RTOS_UDP_RX_BUF_ADDR));
  udpSocket->Tx         = xQueueCreateExternal(255, 1, (void *)(RTOS_UDP_TX_BUF_ADDR));
}

#if UDP_DEBUG
void setUdpDebug(FILE *stream, uint8_t level)
{
  udpDbgStream = stream;
  udpDbgLevel = level;
}
#endif

inline void udpSend(uint16_t len)
{
// make pointer to UDP header
  nicState.layer4.udp->srcport  = udpSocket->srcPort;
  nicState.layer4.udp->destport = (udpSocket->dstPortDef == 0)? udpSocket->dstPort : udpSocket->dstPortDef;          //data in udpSocket are stored in network order

  nicState.layer4.udp->udplen = htons(len + UDP_HEADER_LEN);
  nicState.layer4.udp->udpchksum = 0;

#if UDP_DEBUG
  if (udpDbgStream != NULL)
    if (udpDbgLevel > 1)
      fprintf_P(udpDbgStream, PSTR("Sending UDP packet (data length %d)\r\n"), len);
#endif
  ipSend(udpSocket->dstIp, IP_PROTO_UDP, len + UDP_HEADER_LEN);

  if(udpDbgStream != NULL)
    fprintf_P(udpDbgStream, PSTR("UDP tx %d bytes\r\n"), len);
}

inline void netstackUDPIPProcess(void)
{
  uint16_t len = (uint16_t) htons(nicState.layer4.udp->udplen);
  uint8_t i;

  #if UDP_DEBUG
  if(udpDbgStream != NULL)
    if (udpDbgLevel > 5)
      fprintf_P(udpDbgStream, PSTR("Proc. UDP packet (data length %d)"), len-UDP_HEADER_LEN);
#endif

  if ((udpSocket->srcPort != nicState.layer4.udp->destport) ||
     ((udpSocket->dstPortDef != HTONS(0)) && (udpSocket->dstPort == nicState.layer4.udp->srcport)))
  {
#if UDP_DEBUG
      if(udpDbgStream != NULL)
        if (udpDbgLevel > 5)
          fprintf_P(udpDbgStream, PSTR("Skipping, wrong ports %d %d\r\n"), nicState.layer4.udp->destport, nicState.layer4.udp->srcport );
#endif
     return;
  }

  if (udpSocket->dstPortDef == HTONS(0))
    udpSocket->dstPort = nicState.layer4.udp->srcport;
  uint8_t *tmp = (uint8_t *)(nicState.layer4.udp) + UDP_HEADER_LEN;
#if UDP_DEBUG
      if(udpDbgStream != NULL)
        if (udpDbgLevel > 4)
          fprintf_P(udpDbgStream, PSTR("Received UDP data:"));
#endif

  for (i=UDP_HEADER_LEN; i<len; i++)
  {
#if UDP_DEBUG
    if(udpDbgStream != NULL)
      if (udpDbgLevel > 4)
        fprintf_P(udpDbgStream, PSTR(" 0x%2x"), *tmp);
#else
    xQueueSend(udpSocket->Rx, tmp, 0);
#endif
#if UDP_DEBUG
      if (xQueueSend(udpSocket->Rx, tmp, 10) == 0)
        if(udpDbgStream != NULL)
          if (udpDbgLevel > 0)
            fprintf_P(udpDbgStream, PSTR("UDP TX buffer busy\r\n"));
#endif
      tmp++;
    }
#if UDP_DEBUG
    if(udpDbgStream != NULL)
    {
       if (udpDbgLevel > 4)
        fprintf_P(udpDbgStream, PSTR("\r\n"));
       else if (udpDbgLevel > 0)
        fprintf_P(udpDbgStream, PSTR("Received UDP packet (len %d)\r\n"), len);

    }
#endif
}

inline void flushUdpQueues(void)
{
  if (uxQueueMessagesWaiting(udpSocket->Tx) > 0)
  {
    uint16_t len = 0;
    uint8_t *data = (uint8_t *)(nicState.layer4.udp) + UDP_HEADER_LEN;
    while (xQueueReceive(udpSocket->Tx, data, 0) == pdTRUE)
    {
      data++;
      len++;
    }
    udpSend(len);
  }
}

void udpSaveConfig(void)
{
  eeprom_update_dword(&udpIpDst_eep, udpSocket->dstIp);
  eeprom_update_word(&udpPortDstEep, udpSocket->dstPortDef);
  eeprom_update_word(&udpPortSrcEep, udpSocket->srcPort);
}

void udpPrintStatus(FILE *stream)
{
  fprintf_P(stream, PSTR("UDP config:"));
  fprintf_P(stream, PSTR("\r\n  IP          : ")); netPrintIPAddr(stream, udpSocket->dstIp);
  fprintf_P(stream, PSTR("\r\n  src port    : %d\r\n  dst port    : "), htons(udpSocket->srcPort));
  if (udpSocket->dstPortDef == HTONS(0))
    fprintf_P(stream, PSTR("ANY\r\n"));
  else
    fprintf_P(stream, PSTR("%d\r\n"), htons(udpSocket->dstPortDef));
}
