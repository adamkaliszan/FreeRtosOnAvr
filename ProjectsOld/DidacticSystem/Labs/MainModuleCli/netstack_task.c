/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Ethernet remote device and sensor
 * UDP and HTTP interface
        url looks like this http://baseurl/password/command
        or http://baseurl/password/
 *
 * Chip type           : Atmega88 or Atmega168 or Atmega328 with ENC28J60
 * Note: there is a version number in the text. Search for tuxgraphics
 *********************************************/
#include "netstack_task.h"


/*uint16_t printHTMLstatus(char *buf, uint16_t pos, uint16_t maxPos)
{*/
/*  char *tmpPtr;

  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ( "<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>"));
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ( "<h3>Status</h3>"));
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<p>"SYSTEM_NAME" ver <b>"S_VERSION"</b> build: "__DATE__", "__TIME__"</p>"));

  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<p><table border=1>"));

  tmpPtr = getBufPosToWrite(buf, pos);
  pos +=sprintf_P(tmpPtr, PSTR("<tr><td>Temperatura</td><td>%d C</td></tr>"), temperature);
  tmpPtr = getBufPosToWrite(buf, pos);
  pos +=sprintf_P(tmpPtr, PSTR("<tr><td>Napięcie na magistrali</td><td>%d V</td></tr>"), voltage);

  uint8_t tmp = ramDyskLiczbaWolnychKlastrow();
  tmpPtr = getBufPosToWrite(buf, pos);
  pos +=sprintf_P(tmpPtr, PSTR("<tr><td>Liczba wolnych klastrów</td><td>%d / %d</td></tr>"), tmp, L_KLASTROW);

  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("</table></p>"));

  tmpPtr = getBufPosToWrite(buf, pos);

  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR("<h3>Czujniki rygli</h3>"));
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<p><table border=1>"));
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<tr><td>Czujnik nr</td><td>Położenie rygla</td><td>Odczyt z przetwornika AC</td><td>Wart graniczna</td></tr>"));
  for (tmp = 0; tmp < 4; tmp++)
  {
    if (lockSensors[tmp].enabled)
    {
      tmpPtr = getBufPosToWrite(buf, pos);
      pos +=sprintf_P(tmpPtr, PSTR("<tr align=\"center\"><td>%d</td>"), tmp+1);
      if (lockSensors[tmp].locked)
        pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<td>zamknięty</td>"));
      else
        pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("<td>otwarty</td>"));

      tmpPtr = getBufPosToWrite(buf, pos);
      pos +=sprintf_P(tmpPtr, PSTR("<td>%d</td><td>%d</td>"), lockSensors[tmp].acVal, lockSensors[tmp].threshold);
      pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("</tr>"));
    }
  }
  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR ("</table></p>"));

  pos=fill_tcp_data_p(Enc28j60_global.buf, pos, PSTR("<h3>Moduły wykonawcze</h3>"));
  return pos;*/
//   return 0;
// }


void encTask ( void *pvParameters )
{
  FILE *netstackDebug = (FILE *) pvParameters;
  uint16_t plen;

  nicInit();
  ipInit();
  arpInit();
  icmpInit();


  //TODO    init_ip_arp_udp_tcp (mymac, ipGetConfig()->ip, MYWWWPORT);


  for ( ; ; )
  {
    vTaskDelay ( 0 );         //Zastąpić oczekiwaniem na zwolnienie semafora. Semafor zostaje zwolniony po odebrzeniu przerwania od ENC

    // get the next new packet:
    plen = nicPoll();
    /*plen will ne unequal to zero if there is a valid
    * packet (without crc error) */
    if ( plen==0 )
    {
      flushUdpQueues();
      flushTcpQueues();
      //flush HTTP long file queue
      continue;
    }

    if(nicState.layer2.ethHeader->type == htons(ETHTYPE_IP))             // process an IP packet
    {
      arpIpIn();
      netstackIPv4Process();
    }
    else if(nicState.layer2.ethHeader->type == htons(ETHTYPE_ARP))       // process an ARP packet
    {
      arpArpIn();
    }
    else
    {
      if (netstackDebug != NULL)
      {
        fprintf_P(netstackDebug, PSTR("Unknown packet\r\n"));
      }
    }

    continue;
  }
}

#if 0
//   uint16_t dat_p;
//   uint8_t cmd_pos=0;
//   int8_t cmd;
//   uint8_t payloadlen=0;
//   char str[30];
//   char cmdval;


    // arp is broadcast if unknown but a host may also
    // verify the mac address by sending it to
    // a unicast address.
    if ( eth_type_is_arp_and_my_ip(Enc28j60_global.buf, plen))
    {
      make_arp_answer_from_request(Enc28j60_global.buf);
      continue;
    }

    // IP - check if ip packets are for us:
    if ( eth_type_is_ip_and_my_ip ( Enc28j60_global.buf,plen ) ==0 )
      continue;

    // PING
    if ( Enc28j60_global.buf[IP_PROTO_P]==IP_PROTO_ICMP && Enc28j60_global.buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V )
    {
      // a ping packet, let's send pong
      make_echo_reply_from_request (Enc28j60_global.buf, plen);
      continue;
    }

    }
    if ( Enc28j60_global.buf[IP_PROTO_P]==IP_PROTO_TCP && Enc28j60_global.buf[TCP_DST_PORT_H_P]==0 && Enc28j60_global.buf[TCP_DST_PORT_H_P]==MYTELNETPOERT_H
      && (Enc28j60_global.buf[TCP_DST_PORT_L_P]>=MYTELNETPOERT_L || Enc28j60_global.buf[TCP_DST_PORT_L_P]<=MYTELNETPOERT_L + 20))
    {
      processIpPacket(Enc28j60_global.buf);
      continue;
    }
#endif
