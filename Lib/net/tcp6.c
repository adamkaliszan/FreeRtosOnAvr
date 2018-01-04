/**
 * @file        tcp.c
 * @author      Adam Kaliszan
 * @brief       TCP socket
 * @ingroup     network
 * @defgroup    netstack Network Stack
 * @version     0.2
 * Created:     13.10.2010
 * Revised:     05.10.2010
 * Editor Tabs: 2
 *
 */

#include "tcp6.h"
#include "include/ipv6.h"


static struct TcpIpSocket* findConnectedSocket(void);

/**
 * @param *sck socket
 */
static inline void tcpAcceptConn6(struct TcpIpSocket *sck);

inline void socketInit6(void)
{
  sockets = xmalloc(NUMBER_OF_SOCKETS * sizeof(struct TcpIpSocket));
  memset(sockets, 0, NUMBER_OF_SOCKETS * sizeof(struct TcpIpSocket));
  
  uint8_t            i;
  uint8_t            *ptr = (uint8_t *)RTOS_TCP_BUF_BASE_ADDR;

  struct TcpIpSocket *sck = sockets;
  for (i=0; i < NUMBER_OF_SOCKETS; i++)
  {
    sck->Rx = xQueueCreateExternal(255, 1, (void *)(ptr));
    ptr+=256;
    sck->Tx = xQueueCreateExternal(255, 1, (void *)(ptr));
    ptr+=256;

    sck->localPort     = (i<16) ? htons(MYTELNETPOERT + i) : (MYTELNETPOERT + 16);
    sck->seqNoLastSent = HTONL(0xFF112233); 
    sck->state         = LISTEN;   
    sck++;
  }
}

struct TcpIpSocket* findConnectedSocket6(void)
{
  struct TcpIpSocket *result = sockets;
  //UIP_TCP_BUF->
  uint8_t i;
  for (i=0; i<NUMBER_OF_SOCKETS; i++)
  {
    if ( ((result->state != LISTEN) && (result->state != CLOSED)) 
      && memcmp(&result->RemoteIpAddr6,&nicState.layer3.ipv6->srcipaddr, 128) && (result->localPort == UIP_TCP_BUF->destport) && (result->remotePort == UIP_TCP_BUF->srcport))
    {
#if TCP_DEBUG
      if (tcpDebugStream != NULL)
        if (tcpDebugLevel > 2)
          fprintf_P(tcpDebugStream, PSTR("Found TCP socket state %d\r\n"), result->state);
#endif      
      return result;
    }
    result++;
  }
  
  result = sockets;
  for (i=0; i<NUMBER_OF_SOCKETS; i++)
  {
    if ((result->state == LISTEN) && (result->localPort == UIP_TCP_BUF->destport))
    {
#if TCP_DEBUG
      if (tcpDebugStream != NULL)
        if (tcpDebugLevel > 2)
          fprintf_P(tcpDebugStream, PSTR("Found TCP socket no %d state LISTEN\r\n"), i);
#endif      
      return &sockets[i];
    }
    result++;
  }
#if TCP_DEBUG
  if (tcpDebugStream != NULL)
    if (tcpDebugLevel > 2)
      fprintf_P(tcpDebugStream, PSTR("Can't find TCP socket with localPort %d\r\n"), htons(UIP_TCP_BUF->destport));
#endif
  return NULL;
}

static inline void tcpAcceptConn6(struct TcpIpSocket *sck)
{
  //UIP_TCP_BUF->
  sck->state        = SYN_RECEIVED;
            fprintf_P(tcpDebugStream, PSTR("TCP PORTS src %04x ; dest %04x\r\n"), UIP_TCP_BUF->srcport, UIP_TCP_BUF->destport);
  uint16_t tempPort = UIP_TCP_BUF->destport;
  sck->remotePort   = UIP_TCP_BUF->srcport;
  sck->localPort = tempPort;
  //memcpy(&sck->RemoteIpAddr6, &nicState.layer3.ip->srcipaddr, 128)
  sck->RemoteIpAddr6 = nicState.layer3.ipv6->srcipaddr;  
}

inline uint8_t processTcpPacket6(void)
{
  ///struct TcpIpSocket *socket = findConnectedSocket6();
  //UIP_TCP_BUF->
  
  ///if (socket == NULL)
    ///return 1;
  
  struct TcpIpSocket *socket = &sockets[0];
   
  socket->seqNoLastReceived = htonl( (struct netTcpHeader *)UIP_TCP_BUF->seqno);
  
  if (socket->state == LISTEN)
  {
    if (UIP_TCP_BUF->flags & TCP_FLAGS_SYN)
    {
//      uint16_t len = UIP_TCP_BUF->tcpoffset>>4;
//      len *=4;
#if TCP_DEBUG
      if (tcpDebugStream != NULL)
        if (tcpDebugLevel > 2)
          fprintf_P(tcpDebugStream, PSTR("Opening TCP connection socket state change LISTEN->SYN_RECEIVED\r\n"));
#endif
      tcpAcceptConn6(socket);
      //Preparing response
      UIP_TCP_BUF->srcport   = socket->localPort;
      UIP_TCP_BUF->destport  = socket->remotePort;
      UIP_TCP_BUF->seqno     = htonl(socket->seqNoLastSent);
      UIP_TCP_BUF->ackno     = htonl(socket->seqNoLastReceived+1);
      UIP_TCP_BUF->tcpoffset = 5<<4;
      UIP_TCP_BUF->flags     = TCP_FLAGS_ACK+TCP_FLAGS_SYN;
      UIP_TCP_BUF->wnd       = htons(100);
      UIP_TCP_BUF->tcpchksum = 0;
      UIP_TCP_BUF->urgp      = 0;
      calculateTcpChecksum6(TCP_HEADER_LEN);

      socket->seqNoLastSent++;
      fprintf_P(tcpDebugStream, PSTR("Sending SYN_ACK\r\n"));
      ipv6Send(IP_PROTO_TCP, TCP_HEADER_LEN);
      fprintf_P(tcpDebugStream, PSTR("Sent SYN_ACK\r\n"));
    }
#if TCP_DEBUG
    else
    {
      if (tcpDebugStream != NULL)
        if (tcpDebugLevel > 1)
          fprintf_P(tcpDebugStream, PSTR("Opening TCP connection ERROR: syn flag wasn't set\r\n"));
    }
#endif    
    return 0;
  }
  
  if (socket->state == SYN_RECEIVED)
  {
    fprintf_P(tcpDebugStream, PSTR("SYN_RECEIVED\r\n"));
    if (UIP_TCP_BUF->flags & TCP_FLAGS_ACK)
    {
      socket->state    = ESTABILISHED;
      fprintf_P(tcpDebugStream, PSTR("ESTABILISHED\r\n"));
#if TCP_DEBUG
    if (tcpDebugStream != NULL)
      if (tcpDebugLevel > 2)
        fprintf_P(tcpDebugStream, PSTR("Opening TCP connection socket state change SYN_RECEIVED->ESTABILISHED\r\n"));
#endif        

    }
    else
    {
      socket->state = LISTEN;
#if TCP_DEBUG
      if (tcpDebugStream != NULL)
        if (tcpDebugLevel > 1)
          fprintf_P(tcpDebugStream, PSTR("Opening TCP connection ERROR: ack flag wasn't set\r\n"));
#endif    
    }
    return 0;
  }
  
  
  if (socket->state == ESTABILISHED)
  {
    if (UIP_TCP_BUF->flags & TCP_FLAGS_FIN)  //ESTABILISHED -> CLOSE_WAIT -> closed
    {
      socket->timer              = timer100Hz;
      UIP_TCP_BUF->flags = TCP_FLAGS_ACK;
      
      uint8_t dataFromBufLen;
      uint8_t *dataPtr = (uint8_t *)(UIP_TCP_BUF+1);
//      while (xQueueReceive(socket->Tx, dataPtr, 0) == pdTRUE)
//      {
//        dataFromBufLen++;
//        dataPtr++;
//      }
      calculateTcpChecksum6(TCP_HEADER_LEN);
      ipv6Send(IP_PROTO_TCP, TCP_HEADER_LEN + dataFromBufLen);
      socket->state    = CLOSE_WAIT;
      
      
      UIP_TCP_BUF->flags = TCP_FLAGS_FIN;
      calculateTcpChecksum6(TCP_HEADER_LEN);
      ipv6Send(IP_PROTO_TCP, TCP_HEADER_LEN);
      socket->state    = LAST_ACK;
    }
    return 0;
  }

  //Read data and put into the queue
  
  return 0;
}

void calculateTcpChecksum6(uint16_t tcpLen)
{
  UIP_TCP_BUF->tcpchksum = 0;
  UIP_TCP_BUF->tcpchksum = netChecksum(UIP_TCP_BUF, tcpLen); //TODO finish it
}



uint8_t sendTcBuffer6(uint8_t socketNo)
{
  struct TcpIpSocket *sck = &sockets[socketNo];
  return 0;
}

void netstackTCPIPProcess6(void)
{
  if (UIP_TCP_BUF->destport == htons(80))
  {
#if TCP_DEBUG
    if (tcpDebugStream != NULL)
      fprintf_P(tcpDebugStream, PSTR("HTTP not implemented\r\n"));
#endif
    ;
  }
  else
  {
    processTcpPacket6();
  }
}

#if TCP_DEBUG
void setTcpDebug6(FILE *stream, uint8_t level)
{
  tcpDebugStream = stream;
  tcpDebugLevel = level;
}
#endif /* TCP_DEBUG */

void flushTcpQueues6()
{
  uint8_t sckNo = 0;
  struct TcpIpSocket *sck = sockets;
  for (sckNo = 0; sckNo < NUMBER_OF_SOCKETS; sckNo++)
  {
    
    sck++;
  }
}


inline void httpProcess6()
{
#if 0
    // TCP WWW tcp port www start, compare only the lower byte
    if ( Enc28j60_global.buf[IP_PROTO_P]==IP_PROTO_TCP && Enc28j60_global.buf[TCP_DST_PORT_H_P]==0 && Enc28j60_global.buf[TCP_DST_PORT_L_P]==MYWWWPORT )
    {
      if ( Enc28j60_global.buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V )
      {
        make_tcp_synack_from_syn (Enc28j60_global.buf );
        // make_tcp_synack_from_syn does already send the syn,ack
        continue;
      }
      if (Enc28j60_global.buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
      {
        init_len_info (Enc28j60_global.buf ); // init some data structures
        // we can possibly have no data, just ack:
        dat_p=get_tcp_data_pointer();
        if ( dat_p==0 )
        {
          if (Enc28j60_global.buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V )
          {
            // finack, answer with ack
            make_tcp_ack_from_any (Enc28j60_global.buf );
          }
          // just an ack with no data, wait for next packet
          continue;
        }
        if ( strncmp ( "GET ", ( char * )(&Enc28j60_global.buf[dat_p]), 4) !=0 )
        {
          // head, post and other methods:
          //
          // for possible status codes see:
          // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
          plen=fill_tcp_data_p (Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>" ) );
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
          continue;
        }
        if ( strncmp ( "/ ", ( char * ) & (Enc28j60_global.buf[dat_p+4] ),2 ) ==0)
        {
          plen=fill_tcp_data_p(Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" ) );
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "<p>Usage: http://host_or_ip/filename or http://host_or_ip/sd/filename</p>\n" ) );
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
          continue;
        }

        cmd = analyse_get_url (( char * )(&Enc28j60_global.buf[dat_p+5]), filename);

        if (cmd == URLramDysk)
        {
          plen=fill_tcp_data_p(Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" ) );
  
          //Open the filen
          struct ramPlikFd fd;
          if (ramDyskOtworzPlik(filename, &fd) != 0)
          {
            plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "<p>Nie mozna otworzyc pliku o nazwie: " ) );
            plen=fill_tcp_data(Enc28j60_global.buf, plen, filename);
            plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( " umieszczonego w ram dysku</p>\n" ) );
            make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
            make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
            continue;
          }
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "<p>Zawartosc pliku " ) );
          plen=fill_tcp_data(Enc28j60_global.buf, plen, filename);
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( " zapisanego w ram dysku:<br>" ) );

          char c;
          while (plen < Enc28j60_global.bufferSize - 30)
          {
            if (ramDyskCzytajBajtZPliku(&fd , &c) != 0)
              break;
            plen = add_tcp_byte(Enc28j60_global.buf, plen, c);
          }
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "</p>\n"));
          ramDyskZamknijPlik(&fd);
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data

          continue;
        }

        if (cmd == URLsdDysk)
        {
          plen=fill_tcp_data_p(Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" ) );
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( "<p>Do zaimpelentowania wyswietlenie pliku o nazwie " ) );
          plen=fill_tcp_data(Enc28j60_global.buf, plen, filename);
          plen=fill_tcp_data_p(Enc28j60_global.buf, plen, PSTR ( " z karty SD</p>\n" ) );
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
          continue;
        }
        if (cmd == URLstatus)
        {
          plen=fill_tcp_data_p(Enc28j60_global.buf, 0, PSTR ( "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" ) );
          plen=printHTMLstatus(Enc28j60_global.buf, plen, Enc28j60_global.bufferSize);  
          make_tcp_ack_from_any(Enc28j60_global.buf);          // send ack for http get
          make_tcp_ack_with_data(Enc28j60_global.buf, plen);   // send data
          continue;  
        }
      }

#endif
}