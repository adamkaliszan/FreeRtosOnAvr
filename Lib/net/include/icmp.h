/**
 * @file      icmp.h 
 * @version   0.1
 * @author    Pascal Stang, Adam Kaliszan
 * @brief     ICMP Protocol Library.
 * @ingroup   network
 * @defgroup  icmp ICMP Protocol Library (icmp.c)
 * @code #include "net/icmp.h" @endcode
 * @par       Description
 *        ICMP (Internet Control Message Protocol) has many functions on the
 *        internet, including the handling of ECHO (ping) requests, relaying
 *        network route status, passing connection status messages, etc.
 *
 *        This library currently handles only ICMP ECHO requests (ping), but
 *        may be expanded to include other useful ICMP operations as needed.
 *
 * Created       : 10.09.2004
 * Revised       : 28.11.2010
 * Target MCU    : Atmel AVR series
 * Editor Tabs   : 2
 *
 *
 * This code is distributed under the GNU Public License
 * which can be found at http://www.gnu.org/licenses/gpl.txt
 */

//@{

#ifndef ICMP_H
#define ICMP_H

#include "net.h"
#include "nic.h"
#include "arp.h"


#include <stdio.h>
#include <avr/pgmspace.h>

#if ICMP_DEBUG
FILE *icmpDebug;
uint8_t icmpDebugLevel;

/**
 * Enable or disable icmp
 * @param *stream - output stream
 */
void setIcmpDebug(FILE *stream, uint8_t level);

#endif /*ICMP_DEBUG*/

/**
 * Initialize ICMP protocol library.
 */
void icmpInit(void);

/**
 * Incoming IP packets of protocol ICMP should be passed to this function.
 */
void icmpIpIn(void);

/**
 * Forms and sends a reply in response to an ICMP ECHO request.
 */
void icmpEchoRequest(void);

/**
 * Print ICMP packet information.
 */
void icmpPrintHeader(FILE *stream, struct netIpHeader *ipPacket, struct netIcmpHeader *icmpPacket);

#endif
//@}
