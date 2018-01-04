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

#include "tcp.h"


static struct TcpIpSocket* findConnectedSocket(void);

/**
 * @param *sck socket
 */
static inline void tcpAcceptConn(struct TcpIpSocket *sck);

inline void socketInit(void)
{
  sockets = xmalloc(NUMBER_OF_SOCKETS * sizeof(struct TcpIpSocket));
  memset(sockets, 0, NUMBER_OF_SOCKETS * sizeof(struct TcpIpSocket));

  uint8_t            i;
  uint8_t            *ptr = (uint8_t *)RTOS_TCP_BUF_BASE_ADDR;

  struct TcpIpSocket *sck = sockets;
  for (i=0; i < NUMBER_OF_SOCKETS; i++)
  {
//    sck->Rx = xQueueCreateExternal(255, 1, (void *)(ptr));
    ptr+=256;
//    sck->Tx = xQueueCreateExternal(255, 1, (void *)(ptr));
    ptr+=256;

    sck->localPort     = (i<16) ? htons(MYTELNETPOERT + i) : (MYTELNETPOERT + 16);
    sck->seqNoLastSent = HTONL(0xFF112233);
    sck->state         = LISTEN;
    sck++;
  }
}

struct TcpIpSocket* findConnectedSocket(void)
{
  struct TcpIpSocket *result = sockets;
  uint8_t i;
  for (i=0; i<NUMBER_OF_SOCKETS; i++)
  {
    if ( ((result->state != LISTEN) && (result->state != CLOSED))
      && (result->RemoteIpAddr == nicState.layer3.ip->srcipaddr) && (result->localPort == nicState.layer4.tcp->destport) && (result->remotePort == nicState.layer4.tcp->srcport))
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
    if ((result->state == LISTEN) && (result->localPort == nicState.layer4.tcp->destport))
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
      fprintf_P(tcpDebugStream, PSTR("Can't find TCP socket with localPort %d\r\n"), htons(nicState.layer4.tcp->destport));
#endif
  return NULL;
}

static inline void tcpAcceptConn(struct TcpIpSocket *sck)
{
  sck->state        = SYN_RECEIVED;
  sck->remotePort   = nicState.layer4.tcp->srcport;
  sck->RemoteIpAddr = nicState.layer3.ip->srcipaddr;
}

inline uint8_t processTcpPacket(void)
{
  struct TcpIpSocket *socket = findConnectedSocket();

  if (socket == NULL)
    return 1;


  socket->seqNoLastReceived = htonl(nicState.layer4.tcp->seqno);

  if (socket->state == LISTEN)
  {
    if (nicState.layer4.tcp->flags & TCP_FLAGS_SYN)
    {
//      uint16_t len = nicState.layer4.tcp->tcpoffset>>4;
//      len *=4;
#if TCP_DEBUG
      if (tcpDebugStream != NULL)
        if (tcpDebugLevel > 2)
          fprintf_P(tcpDebugStream, PSTR("Opening TCP connection socket state change LISTEN->SYN_RECEIVED\r\n"));
#endif
      tcpAcceptConn(socket);
      //Preparing response
      nicState.layer4.tcp->srcport   = socket->localPort;
      nicState.layer4.tcp->destport  = socket->remotePort;
      nicState.layer4.tcp->seqno     = htonl(socket->seqNoLastSent);
      nicState.layer4.tcp->ackno     = htonl(socket->seqNoLastReceived+1);
      nicState.layer4.tcp->tcpoffset = 5<<4;
      nicState.layer4.tcp->flags     = TCP_FLAGS_ACK+TCP_FLAGS_SYN;
      nicState.layer4.tcp->wnd       = htons(100);
      nicState.layer4.tcp->tcpchksum = 0;
      nicState.layer4.tcp->urgp      = 0;
      calculateTcpChecksun(TCP_HEADER_LEN);

      socket->seqNoLastSent++;
      ipSend(socket->RemoteIpAddr, IP_PROTO_TCP, TCP_HEADER_LEN);
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
    if (nicState.layer4.tcp->flags & TCP_FLAGS_ACK)
    {
      socket->state    = ESTABILISHED;
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
    if (nicState.layer4.tcp->flags & TCP_FLAGS_FIN)  //ESTABILISHED -> CLOSE_WAIT -> closed
    {
      socket->timer              = timer100Hz;
      nicState.layer4.tcp->flags = TCP_FLAGS_ACK;

      uint8_t dataFromBufLen = 0;
      uint8_t *dataPtr = (uint8_t *)(nicState.layer4.tcp+1);
      while (xQueueReceive(socket->Tx, dataPtr, 0) == pdTRUE)
      {
        dataFromBufLen++;
        dataPtr++;
      }
      ipSend(socket->RemoteIpAddr, IP_PROTO_TCP, TCP_HEADER_LEN + dataFromBufLen);
      socket->state    = CLOSE_WAIT;


      nicState.layer4.tcp->flags = TCP_FLAGS_FIN;
      ipSend(socket->RemoteIpAddr, IP_PROTO_TCP, TCP_HEADER_LEN);
      socket->state    = LAST_ACK;
    }
    return 0;
  }

  //Read data and put into the queue

  return 0;
}

void calculateTcpChecksun(uint16_t tcpLen)
{
  nicState.layer4.tcp->tcpchksum = 0;
  nicState.layer4.tcp->tcpchksum = netChecksum((uint8_t *)nicState.layer4.tcp, tcpLen); //TODO finish it
}



uint8_t sendTcpBuffer(uint8_t socketNo)
{
  (void) socketNo;
  //struct TcpIpSocket *sck = &sockets[socketNo];
  return 0;
}

void netstackTCPIPProcess(void)
{
  if (nicState.layer4.tcp->destport == htons(80))
  {
#if TCP_DEBUG
    if (tcpDebugStream != NULL)
      fprintf_P(tcpDebugStream, PSTR("HTTP not implemented\r\n"));
#endif
    ;
  }
  else
  {
    processTcpPacket();
  }
}

#if TCP_DEBUG
void setTcpDebug(FILE *stream, uint8_t level)
{
  tcpDebugStream = stream;
  tcpDebugLevel = level;
}
#endif /* TCP_DEBUG */

void flushTcpQueues()
{
  uint8_t sckNo = 0;
  struct TcpIpSocket *sck = sockets;
  for (sckNo = 0; sckNo < NUMBER_OF_SOCKETS; sckNo++)
  {

    sck++;
  }
}


inline void httpProcess()
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
