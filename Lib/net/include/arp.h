/**
 * @file     arp.h
 * @version  0.2
 * @brief    ARP Protocol Library.
 * @ingroup  network
 * @defgroup arp ARP Protocol Library (arp.c)
 * @code #include "net/arp.h" @endcode
 * @author    Pascal Stang, Adam Kaliszan
 * Created    : 9/7/2004
 * Revised    : 7/3/2005
 * Target MCU : Atmel AVR series
 * Editor Tabs: 4
 *
 * @par      Description
 *           To send anything over ethernet (or most any other physical network)
 *           a packet must be addressed to a physical network node address, often
 *           called a MAC/hardware/ethernet address.  This MAC address identifies
 *           a specific interface (like the ethernet card in your computer) on the
 *           network.
 *           ARP (Address Resolution Protocol) assists in mapping IP addresses to
 *           the MAC addresses required to actually get data to its destination.
 *           In other words, an IP address is not enough to send information over
 *           ethernet.  You need the MAC address of the network interface/card that
 *           "owns" that IP address.  ARP maintains a table mapping IP addresses to
 *           MAC addresses.  This table can be filled by both listening to
 *           traffic on the network, as well as making specific ARP requests if
 *           an IP<->MAC mapping is not in the table.
 *
 * @note     This code is currently below version 1.0, and therefore is considered
 *           to be lacking in some functionality or documentation, or may not be fully
 *           tested.  Nonetheless, you can expect most functions to work.
 *
 *           This code is distributed under the GNU Public License
 *           which can be found at http://www.gnu.org/licenses/gpl.txt
 */

#ifndef ARP_H
#define ARP_H

#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "softwareConfig.h"
#include "net.h"
#include "nic.h"
#include "ip.h"


#ifndef ARP_TABLE_SIZE
#warning "ARP_TABLE_SIZE not defined in softwareConfig.h file"
#define ARP_TABLE_SIZE	8
#endif

#ifndef ARP_CACHE_TIME_TO_LIVE
#warning "ARP_CACHE_TIME_TO_LIVE not defined in softwareConfig.h file"
#define ARP_CACHE_TIME_TO_LIVE	100
#endif



extern struct ipConfig   IpMyConfig;
extern nicState_t        nicState;
#if ARP_DEBUG
FILE   *arpDebug;
uint8_t arpDebugLevel;
void setArpDebug(FILE *stream, uint8_t level);
#endif /*ARP_DEBUG*/


//@{
/**
 * Initialize ARP system.
 * Clears ARP table and prepares it for use. This is typically done
 * once at program initialization.
 */
void arpInit(void);

/**
 * Processes incoming ARP packets.
 * This function is to be called when an ARP type packet has arrived
 * over the network.  If the packet type is an ARP request for us,
 * an ARP reply will be generated and sent.
 */
void arpArpIn(void);

/**
 * Process incoming IP packets to harvest IP<->MAC relationships.
 * This function should be called when IP packets are received over the
 * network.  It does nothing more than harvest the IP<->MAC address
 * relationships from the ethernet and IP header of the packet.  The
 * packet is not changed nor processed.  Nothing is sent on the network.
 * Use of this command is not required, but it is a good way to
 * automatically fill the ARP table with information about nodes that are
 * active on the network.

 * @warning On very busy or heavily populated netorks, this can quickly
 * fill the ARP table with unnecessary entries, and/or cause some CPU load.
*/
void arpIpIn(void);

/**
 * Process outgoing IP packet to fill in ethernet header information.
 *
 * To be sent on a network, an IP packet must have the correct ethernet
 * header information appended to the front.  This function will fill
 * in this information.
 *
 * A physical destination IP address argument is needed to support
 * sending to a gateway (i.e. when a packet is destined for a node that
 * is not on this network, IP addressing is as usual, but we phyiscally
 * send the packet to the gateway's ethernet address/interface).
 *
 * @warning Technically, if an IP<->MAC address mapping is not in the
 * ARP table, then the IP packet should be held while an ARP request is
 * made, and the reply received.  However, in single-threaded ram-limited
 * embedded systems, such a holdup is unacceptable.  This function instead
 * sends the packet as an ethernet broadcast if a mapping cannot be found.

 * \todo Send the packet broadcast AND send an ARP request, if a mapping
 * is not found.
*/
void arpIpOut(uint32_t phyDstIp);

/**
 * Periodic ARP cache maintenance.
 * This function is to be called once per second and will slowly
 * expire old ARP cache entries.
 */
void arpTimer(void);

/**
 * Check if this IP address is present in the ARP cache. Internal function.
 * If IP address is found, function returns index of entry.  If not found,
 * returns -1.
 */
int arpMatchIp(uint32_t ipaddr);

#ifdef ARP_DEBUG

/**
 * Enable or disable ARP debug
 * @param *stream - output stream
 */
void setArpDebug(FILE *stream, uint8_t level);

/**
 * Print diagnotic information about ARP packet.
 * @param *stream - output stream
 * @param *packet - arp packet
 */
void arpPrintHeader(FILE *stream, struct netArpHeader* packet);

/**
 * Print diagnotic information about ARP cache.
 * @param *stream - output stream
 */
void arpPrintTable(FILE *stream);

#endif /*ARP_DEBUG*/


#endif
//@}
