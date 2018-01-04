/**
 * @file          net.c
 * @author        Pascal Stang, Adam Kaliszan
 * @brief         Network support library.
 * @version       0.2
 * Created        30.08.2004
 * Revised        25.11.2010
 * Editor Tabs    2

 * @ingroup network
 * @defgroup net Network support library (net.c)
 * @code #include "net/net.h" \endcode
 * @par Description
 *  This is a general network support library including a multitude of
 *  structure definitions for various types of network packets, functions
 *  and macros for switching byte order, and an RFC-compliant function
 *  for calculating checksums.
 *
 * This code is distributed under the GNU Public License
 * which can be found at http://www.gnu.org/licenses/gpl.txt
 */


#ifndef NET_H
#define NET_H

#include <inttypes.h>
#include <stdio.h>
#include <avr/pgmspace.h>
// #include "ipv6Conf.h"
#include "softwareConfig.h"

// ******* ETH *******
#define ETH_HEADER_LEN	14
// values of certain bytes:
#define ETHTYPE_ARP_H_V 0x08
#define ETHTYPE_ARP_L_V 0x06
#define ETHTYPE_IP_H_V  0x08
#define ETHTYPE_IP_L_V  0x00
// byte positions in the ethernet frame:
//
// Ethernet type field (2bytes):
#define ETH_TYPE_H_P 12
#define ETH_TYPE_L_P 13
//
#define ETH_DST_MAC 0
#define ETH_SRC_MAC 6


// ******* ARP *******
#define ETH_ARP_OPCODE_REPLY_H_V 0x0
#define ETH_ARP_OPCODE_REPLY_L_V 0x02
//
#define ETHTYPE_ARP_L_V 0x06
// arp.dst.ip
#define ETH_ARP_DST_IP_P 0x26
// arp.opcode
#define ETH_ARP_OPCODE_H_P 0x14
#define ETH_ARP_OPCODE_L_P 0x15
// arp.src.mac
#define ETH_ARP_SRC_MAC_P 0x16
#define ETH_ARP_SRC_IP_P 0x1c
#define ETH_ARP_DST_MAC_P 0x20
#define ETH_ARP_DST_IP_P 0x26


// ******* ICMP *******
#define ICMP_TYPE_ECHOREPLY_V 0
#define ICMP_TYPE_ECHOREQUEST_V 8
//
#define ICMP_TYPE_P 0x22
#define ICMP_CHECKSUM_P 0x24

// ******* UDP *******
#define UDP_HEADER_LEN	8
//
#define UDP_SRC_PORT_H_P 0x22
#define UDP_SRC_PORT_L_P 0x23
#define UDP_DST_PORT_H_P 0x24
#define UDP_DST_PORT_L_P 0x25
//
#define UDP_LEN_H_P 0x26
#define UDP_LEN_L_P 0x27
#define UDP_CHECKSUM_H_P 0x28
#define UDP_CHECKSUM_L_P 0x29
#define UDP_DATA_P 0x2a

#define GNUC_PACKED

//@{
/**
 * Representation of a 48-bit Ethernet address.
 */
//@}
struct netEthAddr
{
  uint8_t addr[6];
} GNUC_PACKED;

/// The Ethernet header
struct netEthHeader
{
  struct netEthAddr dest;
  struct netEthAddr src;
  uint16_t type;
} GNUC_PACKED;


#define ETHTYPE_ARP     0x0806
#define ETHTYPE_IP      0x0800
#define ETHTYPE_IP6     0x86dd

/// The ARP header
struct netArpHeader
{
  uint16_t    hwtype;
  uint16_t    protocol;
  uint8_t     hwlen;
  uint8_t     protolen;
  uint16_t    opcode;
  struct      netEthAddr shwaddr;
  uint32_t    sipaddr;
  struct      netEthAddr dhwaddr;
  uint32_t    dipaddr;
} GNUC_PACKED;

#define ARP_OPCODE_REQUEST     1
#define ARP_OPCODE_REPLY       2
#define ARP_HWTYPE_ETH         1

/// The IP header
struct netIpHeader
{
  uint8_t     vhl;
  uint8_t     tos;
  uint16_t    len;
  uint16_t    ipid;
  uint16_t    ipoffset;
  uint8_t     ttl;
  uint8_t     proto;
  uint16_t    ipchksum;
  uint32_t    srcipaddr;
  uint32_t    destipaddr;
} GNUC_PACKED;
#define IP_HEADER_LEN          20

#define IP_PROTO_ICMP          1
#define IP_PROTO_TCP           6
#define IP_PROTO_UDP           17

#define IP_SRC_P               0x1a
#define IP_DST_P               0x1e
#define IP_HEADER_LEN_VER_P    0xe
#define IP_CHECKSUM_P          0x18
#define IP_TTL_P               0x16
#define IP_FLAGS_P             0x14
#define IP_P                   0xe
#define IP_TOTLEN_H_P          0x10
#define IP_TOTLEN_L_P          0x11

#define IP_PROTO_P 0x17

#ifdef IPV6_SUPPORT
  /**The IPv6 */

  typedef union uip_ip6addr_t {
    uint8_t  u8[16];			/* Initializer, must come first!!! */
    uint16_t u16[8];
  } uip_ip6addr_t;
  typedef uip_ip6addr_t uip_ipaddr_t;

  /**
   * In IPv6 the length of the L3 headers before the transport header is
   * not fixed, due to the possibility to include extension option headers
   * after the IP header. hence we split here L3 and L4 headers
   */
  /* The IPv6 header */
  struct uip_ipv6_hdr {
    uint8_t vtc;
    uint8_t tcflow;
    uint16_t flow;
    uint8_t len[2];
    uint8_t proto, ttl;
    uip_ip6addr_t srcipaddr, destipaddr;
  };
  #define UIP_IPv6H_LEN    40
  #define UIP_IPH_LEN    UIP_IPv6H_LEN
  #define UIP_FRAGH_LEN  8

  /** The ICMPv6 header. */
  struct uip_icmp6_hdr {
    uint8_t type, icode;
    uint16_t icmpchksum;
  };
  #define IPV6_ICMPH_LEN  4 /* Size of ICMPv6 header */
  #define UIP_ICMPH_LEN   IPV6_ICMPH_LEN

#endif /*IPV6_SUPPORT*/


/// The ICMP header
struct netIcmpHeader
{
  uint8_t     type;
  uint8_t     icode;
  uint16_t    icmpchksum;
  uint16_t    id;
  uint16_t    seqno;
} GNUC_PACKED;
#define ICMP_HEADER_LEN        8

#define ICMP_TYPE_ECHOREPLY    0
#define ICMP_TYPE_ECHOREQUEST  8

/// The UDP header
struct netUdpHeader
{
  uint16_t    srcport;
  uint16_t    destport;
  uint16_t    udplen;
  uint16_t    udpchksum;
} GNUC_PACKED;
#define UDP_HEADER_LEN         8

/// The TCP header
struct netTcpHeader
{
  uint16_t    srcport;
  uint16_t    destport;
  uint32_t    seqno;
  uint32_t    ackno;
  uint8_t     tcpoffset;
  uint8_t     flags;
  uint16_t    wnd;
  uint16_t    tcpchksum;
  uint16_t    urgp;
//uint8_t     optdata[4];
} GNUC_PACKED;
#define TCP_HEADER_LEN  20

#define TCP_FLAGS_FIN   0x01
#define TCP_FLAGS_SYN   0x02
#define TCP_FLAGS_RST   0x04
#define TCP_FLAGS_PSH   0x08
#define TCP_FLAGS_ACK   0x10
#define TCP_FLAGS_URG   0x20
#define TCP_FLAGS_ECE   0x40  //ECN-Echo
#define TCP_FLAGS_CWR   0x80  //Congestion Window Reduced


// the tcp seq number is 4 bytes 0x26-0x29
//#define TCP_SEQ_H_P 0x26
//#define TCP_SEQACK_H_P 0x2a
// flags: SYN=2

//  plain len without the options:
//#define TCP_HEADER_LEN_PLAIN 20
//#define TCP_HEADER_LEN_P 0x2e
//#define TCP_CHECKSUM_H_P 0x32
//#define TCP_CHECKSUM_L_P 0x33
//#define TCP_OPTIONS_P 0x36
//


/**
 * Ethernet/ARP header
 */


// The UDP and IP headers
typedef struct
{
  struct netIpHeader  ip;
  struct netUdpHeader udp;
} udpip_hdr;

//@{
//! Convert dot-notation IP address into 32-bit word.
/// Example: IPDOT(192l,168l,1l,1l)
#define IPDOT(a,b,c,d)	((a<<24)|(b<<16)|(c<<8)|(d))

//! Host-to-Network SHORT (16-bit) byte-order swap (macro).
#define HTONS(s)		((s<<8) | (s>>8))
//! Host-to-Network LONG (32-bit) byte-order swap (macro).
#define HTONL(l)		((l<<24) | ((l&0x00FF0000l)>>8) | ((l&0x0000FF00l)<<8) | (l>>24))


/**
 * Host-to-Network SHORT (16-bit) byte-order swap (function).
 */
uint16_t htons(uint16_t val);


uint16_t ntohs(uint16_t val);


/**
 * Host-to-Network LONG (32-bit) byte-order swap (function).
 */
uint32_t htonl(uint32_t val);


uint32_t ntohl(uint32_t val);


/**
 * Calculate IP-style checksum from data.
 */
uint16_t netChecksum(uint8_t *data, uint16_t len);

/**
 * Print Ethernet address in XX:XX:XX:XX:XX:XX format.
 */
void netPrintEthAddr(FILE *stream, struct netEthAddr *ethaddr);

/**
 * Print IP address in dot notation.
 * @param *stream - output stream
 * @param ip_hdr  - ipAddress stored in network order
 */
void netPrintIPAddr(FILE *stream, uint32_t ipaddr);

/**
 * Print Ethernet header information.
 */
void netPrintEthHeader(FILE *stream, struct netEthHeader* eth_hdr);

/**
 * Print IP header information.
 */
void netPrintIpHeader(FILE *stream, struct netIpHeader* ipheader);

/**
 * Print TCP header information.
 */
void netPrintTcpHeader(FILE *stream, struct netTcpHeader* tcpheader);

#endif
//@}

