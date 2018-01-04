/**
 * @file     nic.h
 * @version  0.2
 * @brief    Network Interface Card (NIC) software definition.
 * @ingroup  network
 * @author   Pascal Stang, Adam Kaliszan
 * @defgroup nic Network Interface Card (NIC) software definition (nic.h)
 * @code #include "net/nic.h" @endcode
 * @par Description
 *      This is the software interface standard for network interface hardware
 *      as used by AVRlib.  Drivers for network hardware must implement these
 *      functions to allow upper network layers to initialize the interface,
 *      and send and receive net traffic.
 *
 * Editor Tabs  : 4
 * Target MCU   : Atmel AVR series
 * Created      : 22.08.2004
 * Revised      : 28.11.2010
 *
 * This code is distributed under the GNU Public License
 * which can be found at http://www.gnu.org/licenses/gpl.txt
 */
//@{

#ifndef NIC_H
#define NIC_H

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>

#include "net.h"
#include <avr/pgmspace.h>
#include "hardwareConfig.h"
#include "softwareConfig.h"

typedef struct
{
  uint16_t                bufferSize;          /// rozmiar tablicy pamięci z buforem
  struct netEthAddr       mac;

  union
  {
    uint8_t               *buf;
    struct netEthHeader   *ethHeader;
  } layer2;

  union
  {
    struct netArpHeader  *arp;
    struct netIpHeader   *ip;
    #if IPV6_SUPPORT
    struct uip_ipv6_hdr	 *ipv6;
    uint8_t              *buf; //Pointer on layer3
    #endif
  } layer3;

  union
  {
    struct netIcmpHeader *icmp;
    struct netTcpHeader  *tcp;
    struct netUdpHeader  *udp;
    //#if IPV6_SUPPORT
    //struct uip_icmp6_hdr *icmpv6; //in ipv6 "header length" before icmp_header may varry - there may be extended header
    //#endif
  } layer4;
}  nicState_t;

nicState_t       nicState;

/**
 * Create mac buffer and next call hardware specyfic function to initialize NIC
 * @note For some hardware, this command will take a non-negligible amount of time (1-2 seconds).
 */
void nicInit(void);

/**
 * Initialize network interface hardware.
 * Hardware specyfic function
 * Reset and bring up network interface hardware. This function should leave
 * the network interface ready to handle \c nicSend() and \c nicPoll() requests.
 * @note For some hardware, this command will take a non-negligible amount of time (1-2 seconds).
 */
void nicMacInit(void)                     __attribute__ ((weak));


/** Send packet on network interface.
 * Function accepts the length (in bytes) of the data to be sent, and a pointer
 * to the data.  This send command may assume an ethernet-like 802.3 header is at the
 * beginning of the packet, and contains the packet addressing information.
 * See net.h documentation for ethernet header format.
 * @param length - data length
 */
void nicSend(uint16_t len)                __attribute__ ((weak));

/**
 * Check network interface.
 * Upper network layers may assume that an ethernet-like 802.3 header is at the beginning
 * of the packet, and contains the packet addressing information, without the preamble
 * See net.h documentation for ethernet header format
 * received data are stored in global buffer <b>nicBuffer</b>
 * @return number of received bytes
 */
unsigned int nicPoll(void)               __attribute__ ((weak));

/**
 * Gets mac address of the network interface.
 * This function can return a MAC address read from the NIC hardware, if available.
 * If the hardware does not provide a MAC address, a software-defined address may be
 * returned.  It may be acceptable to return an address that is less than 48-bits.
 * @param [out] macaddr* - pointer to the mac address.
 */
void nicGetMacAddress(uint8_t* macaddr)  __attribute__ ((weak));

/**
 * Set the 48-bit hardware node (MAC) address of this network interface.
 * This function may not be supported on all hardware.
 * @param [in] macaddr* - pointer to the mac address.
 */
void nicSetMacAddress(uint8_t* macaddr)  __attribute__ ((weak));

/**
 * Print network interface hardware registers.
 * Prints a formatted list of names and values of NIC registers for debugging purposes.
 * @param stream - input stream
 */
void nicRegDump(FILE *stream)            __attribute__ ((weak));

/**
 * Save nic parameters like MAC
 */
void nicSaveConfig(void);

void nicLoadConfig(void);

#endif
//@}
