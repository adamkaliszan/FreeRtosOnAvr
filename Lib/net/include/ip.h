/**
 * @file      ip.h
 * @version   0.2
 * @brief     IP (Internet Protocol) Library.
 * @author    Pascal Stang, Adam Kaliszan
 * @ingroup network
 * @defgroup ip IP (Internet Protocol) Library (ip.c)
 * @code #include "net/ip.h" \endcode
 * @par Description
 *              The IP (Internet Protocol) library provide support for sending IP and
 *              IP-related packets.  It's not clear if additional features are needed
 *              or will be added, or even if this is the proper way to facilitate IP
 *              packet operations.
 * Created      : 30.08.2004
 * Revised      : 28/11.2010
 * Target MCU	: Atmel AVR series
 * Editor Tabs	: 2
 *
 *              This code is distributed under the GNU Public License
 *              which can be found at http://www.gnu.org/licenses/gpl.txt
 */
#ifndef IP_H
#define IP_H

#include <inttypes.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "softwareConfig.h"
#include "net.h"
#include "icmp.h"
#include "nic.h"
#include "arp.h"
#include "udp.h"


//@{
/**
 * IP addressing/configuration structure
 */
struct ipConfig
{
  uint32_t ip;                  ///< IP address
  uint32_t netmask;             ///< netmask
  uint32_t gateway;             ///< gateway IP address

#if IP_DEBUG
  FILE*    dbgStream;           ///debug stream
  uint8_t  dbgLevel;
#endif
};

struct ipConfig   IpMyConfig;     ///< Local IP address/config structure
extern nicState_t nicState;

#define IP_TIME_TO_LIVE		128		///< default Time-To-Live (TTL) value to use in IP headers


#if IP_DEBUG
/**
 * Enable or disable debug stream
 * @param *stream - output stream. Do not use network stream. NULL value disable debug stream
 * @param level   - level of sending details (0-3)
 */
void setIpDebug(FILE *stream, uint8_t level);
#endif


void ipInit(void);
/**
 * Read Ip config
 * Initializes ARP
 */
void ipLoadConfig(void);

/**
 * Save Ip config
 */
void ipSaveConfig(void);


/**
 * Process Ip v4 packet
 */
void netstackIPv4Process(void);



/**
 * Set our IP address and routing information.
 * The myIp value will be used in the source field of IP packets.
 * Use this function to set and reset the system IP address.
 * @param myIp      - local IP address
 * @param netmask   - 32 bit network mask
 * @param gatewayIp - default gateway
 */
void ipSetConfig(uint32_t myIp, uint32_t netmask, uint32_t gatewayIp);

/**
 * @param myIp   - ip Address
 */
void ipSetConfigIp(uint32_t myIp);

/**
 * @param netmask - 32 bit network mask
 */
void ipSetConfigMask(uint32_t netmask);

/**
 * @param gatewayIp - default gateway
 */
void ipSetConfigGw(uint32_t gatewayIp);

/**
 * Get our local IP configuration.
 * @return pointer to current IP address/configuration.
 */
struct ipConfig* ipGetConfig(void);

/**
 * Print IP configuration
 * @param straem - input stream
 * @param *config - pointer to IP config struct
 */
void ipPrintConfig(FILE *stream, struct ipConfig* config);


/**
 * Send an IP packet.
 */
void ipSend(uint32_t dstIp, uint8_t protocol, uint16_t len);


#endif
//@}

