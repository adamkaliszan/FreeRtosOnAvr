/**
 * @file        tcp.h
 * @author      Adam Kaliszan
 * @brief       Enc28j60 TCP socket
 * @ingroup     network
 * @version     0.1
 * Created:     13.10.2010
 * Revised:     .12.2010
 * Editor Tabs: 2
 *
 * @defgroup    netstack Network Stack
 * @code #include "net/Enc28j60socket.h" @endcode
 * @par Description
 *  This library supports TCP Sockets.
 *
 * @note This is NOT a full-blown TCP/IP stack. It merely handles lower
 *  level stack functions so that TCP packets can be sent and received easily.
 *  End-to-end TCP functionality may be added in a future version.
 *  Until then, I can recommend using other embedded TCP/IP stacks like Adam Dunkel's uIP.
 *
 */
//@{

#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <string.h>
#include <avr/io.h>

#include "hardwareConfig.h"
#include "softwareConfig.h"
#include "memory_x.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"

#include "net.h"
#include "ip.h"

#if TCP_DEBUG
FILE *tcpDebugStream;
uint8_t tcpDebugLevel;

extern uint8_t timer100Hz;
extern nicState_t nicState;

/**
 * Enables TCP protocol debuging
 * @param *stream - pointer to output debug stream
 * @param level   - debug intensity 0 - debug sidabled, 1 - basic short info, 2 - short info about all ewents, 3 - basic detailed info + short info about all ewents, 4 - detailed info about everything
 */
void setTcpDebug(FILE *stream, uint8_t level);
#endif

typedef enum
{
  CLOSED,
  LISTEN,
  SYN_RECEIVED,
  ESTABILISHED,
  CLOSE_WAIT,
  LAST_ACK,
  SYN_SENT,
  FIN_WAIT1,
  FIN_WAIT2,
  CLOSING,
  TIMED_WAIT
} socket_state_t;

struct TcpIpSocket
{
  socket_state_t state;
  uint32_t       RemoteIpAddr;           /// Stored in network order
#if IPV6_SUPPORT
  uip_ip6addr_t   RemoteIpAddr6;
#endif /*IPV6_SUPPORT*/

  uint16_t       localPort;              /// Stored in network order
  uint16_t       remotePort;             /// Stored in network order

  uint32_t       seqNoLastReceived;      /// Sequence number of last received packet
  uint32_t       seqNoLastSent;          /// Sequence number of last sent packet
  uint16_t       noOfNotAckBytes;        /// Number of received bytes without ack

  uint16_t       windowSize;
  uint8_t        timer;

  xQueueHandle   Rx;
  xQueueHandle   Tx;

  //struct packetBackup packetBackupBuffer[4];
  uint8_t        packetBackupBufferNotConfirmedPacketIdx;
  uint8_t        packetBackupBufferWriteIdx;
};

struct TcpIpSocket *sockets;

/**
 * Initialize socket structs
 */
void socketInit(void);


/**
 * put received data into the matching socket,
 * if theare is matching sockets, creates new connection
 * @param packet - IP packet
 * @return 0 - OK, 1 - socket is ocupied by another connection
 */
uint8_t processTcpPacket(void);

/**
 * Reads number of bytes that are queued in Tx buffer
 * @param socketNo - globals socket descriptor
 * @return number of bytes in TCP buffer
 */
uint8_t getTxBufferState(uint8_t socketNo);


/**
 * Sends data from Tx buffer
 * @param socketNo - socket number
 * @return 0 - all OK
 */
uint8_t sendTcpBuffer(uint8_t socketNo);

/**
 * Calculates TCP checksum according to data in ENC Tx buffer
 * @param TCP data length
 */
void calculateTcpChecksun(uint16_t tcpLen);

/**
 * Close TCP socket
 * @param socketNo - socket number to be closed
 * @return 0 - all OK
 */
uint8_t closeSocket(uint8_t socketNo);


void netstackTCPIPProcess(void);

/**
 * Flush all UDP queues
 */
void flushTcpQueues(void);

void httpProcess(void);

#endif /*TCP_H*/
//@}
