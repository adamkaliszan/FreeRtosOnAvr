/**
 * @file        tcp.h //tcp6.h
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
 *  This library supports TCP Sockets (over ipv6). 
 *  
 * @note This is NOT a full-blown TCP/IP stack. It merely handles lower
 *  level stack functions so that TCP packets can be sent and received easily.
 *  End-to-end TCP functionality may be added in a future version.
 *  Until then, I can recommend using other embedded TCP/IP stacks like Adam Dunkel's uIP.
 *
 */
//@{

#ifndef SOCKET_V6_H
#define SOCKET_V6_H

#include <stdio.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <string.h>
#include <avr/io.h>


#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"

#include "ipv6.h"
#include "../../freeRtos/Lib/net/include/ipv6.h"
#include "tcp.h"

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
void setTcpDebug6(FILE *stream, uint8_t level);
#endif

/**
 * Initialize socket structs
 */
void socketInit6(void);


/**
 * put received data into the matching socket,
 * if theare is matching sockets, creates new connection
 * @param packet - IP packet
 * @return 0 - OK, 1 - socket is ocupied by another connection
 */
uint8_t processTcpPacket6(void);

/**
 * Reads number of bytes that are queued in Tx buffer
 * @param socketNo - globals socket descriptor
 * @return number of bytes in TCP buffer
 */
uint8_t getTxBufferState6(uint8_t socketNo);


/**
 * Sends data from Tx buffer
 * @param socketNo - socket number
 * @return 0 - all OK
 */
uint8_t sendTcpBuffer6(uint8_t socketNo);

/**
 * Calculates TCP checksum according to data in ENC Tx buffer
 * @param TCP data length
 */
void calculateTcpChecksum6(uint16_t tcpLen);

/**
 * Close TCP socket
 * @param socketNo - socket number to be closed
 * @return 0 - all OK
 */
uint8_t closeSocket(uint8_t socketNo);


void netstackTCPIPProcess6(void);

/**
 * Flush all UDP queues
 */
void flushTcpQueues6(void);

void httpProcess6(void);

#endif /*SOCKET_V6_H*/
//@}
