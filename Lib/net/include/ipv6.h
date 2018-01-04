#ifndef IPV6_H
#define IPV6_H 1

/**
 * Definicja zmiennych potrzebnych do obsługi IPv6
 * (część deklaracji może się znajdować w pliku net.h i nic.h)
 * 
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include "net.h"
#include "nic.h"
#include "softwareConfig.h"
#include "ip.h"
#include "ipv6Conf.h"
#include "uip-netif.h"
#include "icmp6.h"
#include "ipv6-nd.h"

#if IPV6_SUPPORT

/** Kompatybilność ze stosem Adama Kaliszana*/
#define UIP_PROTO_ICMP  IP_PROTO_ICMP //1
#define UIP_PROTO_TCP   IP_PROTO_TCP //6
#define UIP_PROTO_UDP   IP_PROTO_UDP //17
#define UIP_PROTO_ICMP6 58
#define UIP_LLADDR_LEN  6 //MAC 48 bit

#define UIP_TTL IP_TIME_TO_LIVE	

#define PRINTF(...) fprintf_P(__VA_ARGS__)
#define PRINT6ADDR(stream, addr) PRINTF(stream, PSTR("\tIPv6 addr: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x \r\n"),((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])

/*
 * IP addressing/configuration structure

struct ipv6Config                 
{
  uip_ip6addr_t *ip;                  ///< IPv6 address
  uint8_t       prefix;               ///< netmask (prefix)
  uip_ip6addr_t *gateway;             ///< gateway IPv6 address
}; */

typedef struct netEthAddr uip_lladdr_t;

//struct ipv6Config   Ipv6MyConfig;     ///< Local IP address/config structure
extern struct uip_netif *uip_netif_physical_if; ///< Local IP address/config structure
extern nicState_t nicState;

#if IP_DEBUG
FILE *debugStream;
#endif /*IP_DEBUG*/

/**
 * \brief length of the extension headers read. updated each time we process
 * a header
 */
uint8_t uip_ext_len;
/** \brief length of the header options read */
uint8_t uip_ext_opt_offset;

/**
 * The sums below are quite used in ND. When used for uip_buf, we
 * include link layer length when used for uip_len, we do not, hence
 * we need values with and without LLH_LEN we do not use capital
 * letters as these values are variable
 */
#define UIP_LLH_LEN	 ETH_HEADER_LEN	
#define uip_l2_l3_hdr_len (UIP_LLH_LEN + UIP_IPH_LEN + uip_ext_len)
#define uip_l2_l3_icmp_hdr_len (UIP_LLH_LEN + UIP_IPH_LEN + uip_ext_len + UIP_ICMPH_LEN)
#define uip_l3_hdr_len (UIP_IPH_LEN + uip_ext_len)
#define uip_l3_icmp_hdr_len (UIP_IPH_LEN + uip_ext_len + UIP_ICMPH_LEN)

/*---------------------------------------------------------------------------*/
/* Buffers                                                                   */
/*---------------------------------------------------------------------------*/
//#define FBUF                             ((struct uip_tcpip_hdr *)&uip_reassbuf[0])
#define UIP_IP_BUF                        ((struct uip_ipv6_hdr *)&nicState.layer2.buf[UIP_LLH_LEN])
#define UIP_ICMP_BUF                     ((struct uip_icmp6_hdr *)&nicState.layer2.buf[uip_l2_l3_hdr_len])
#define UIP_UDP_BUF                        ((struct uip_udp_hdr *)&nicState.layer2.buf[uip_l2_l3_hdr_len])
#define UIP_TCP_BUF                       ((struct netTcpHeader *)&nicState.layer2.buf[uip_l2_l3_hdr_len])
#define UIP_EXT_BUF                        ((struct uip_ext_hdr *)&nicState.layer2.buf[uip_l2_l3_hdr_len])
#define UIP_ROUTING_BUF                ((struct uip_routing_hdr *)&nicState.layer2.buf[uip_l2_l3_hdr_len])
#define UIP_FRAG_BUF                      ((struct uip_frag_hdr *)&nicState.layer2.buf[uip_l2_l3_hdr_len])
#define UIP_HBHO_BUF                      ((struct uip_hbho_hdr *)&nicState.layer2.buf[uip_l2_l3_hdr_len])
#define UIP_DESTO_BUF                    ((struct uip_desto_hdr *)&nicState.layer2.buf[uip_l2_l3_hdr_len])
#define UIP_EXT_HDR_OPT_BUF            ((struct uip_ext_hdr_opt *)&nicState.layer2.buf[uip_l2_l3_hdr_len + uip_ext_opt_offset])
#define UIP_EXT_HDR_OPT_PADN_BUF  ((struct uip_ext_hdr_opt_padn *)&nicState.layer2.buf[uip_l2_l3_hdr_len + uip_ext_opt_offset])
#define UIP_ICMP6_ERROR_BUF            ((struct uip_icmp6_error *)&nicState.layer2.buf[uip_l2_l3_icmp_hdr_len])

/**
 * Construct an IPv6 address from eight 16-bit words.
 *
 * This function constructs an IPv6 address.
 *
 * \hideinitializer
 */
#define uip_ip6addr(addr, addr0,addr1,addr2,addr3,addr4,addr5,addr6,addr7) do { \
    (addr)->u16[0] = HTONS(addr0);                                      \
    (addr)->u16[1] = HTONS(addr1);                                      \
    (addr)->u16[2] = HTONS(addr2);                                      \
    (addr)->u16[3] = HTONS(addr3);                                      \
    (addr)->u16[4] = HTONS(addr4);                                      \
    (addr)->u16[5] = HTONS(addr5);                                      \
    (addr)->u16[6] = HTONS(addr6);                                      \
    (addr)->u16[7] = HTONS(addr7);                                      \
  } while(0)
  
/**
 * Construct an IPv6 address from eight 8-bit words.
 *
 * This function constructs an IPv6 address.
 *
 * \hideinitializer
 */
#define uip_ip6addr_u8(addr, addr0,addr1,addr2,addr3,addr4,addr5,addr6,addr7,addr8,addr9,addr10,addr11,addr12,addr13,addr14,addr15) do { \
    (addr)->u8[0] = addr0;                                       \
    (addr)->u8[1] = addr1;                                       \
    (addr)->u8[2] = addr2;                                       \
    (addr)->u8[3] = addr3;                                       \
    (addr)->u8[4] = addr4;                                       \
    (addr)->u8[5] = addr5;                                       \
    (addr)->u8[6] = addr6;                                       \
    (addr)->u8[7] = addr7;                                       \
    (addr)->u8[8] = addr8;                                       \
    (addr)->u8[9] = addr9;                                       \
    (addr)->u8[10] = addr10;                                     \
    (addr)->u8[11] = addr11;                                     \
    (addr)->u8[12] = addr12;                                     \
    (addr)->u8[13] = addr13;                                     \
    (addr)->u8[14] = addr14;                                     \
    (addr)->u8[15] = addr15;                                     \
  } while(0)

/**
 * Compare two IP addresses
 *
 * Compares two IP addresses.
 *
 * Example:
 \code
 uip_ipaddr_t ipaddr1, ipaddr2;

 uip_ipaddr(&ipaddr1, 192,16,1,2);
 if(uip_ipaddr_cmp(&ipaddr2, &ipaddr1)) {
 printf("They are the same");
 }
 \endcode
 *
 * \param addr1 The first IP address.
 * \param addr2 The second IP address.
 *
 * \hideinitializer
 */
#define uip_ipaddr_cmp(addr1, addr2) (memcmp(addr1, addr2, sizeof(uip_ip6addr_t)) == 0)

/**
 * Compare two IP addresses with netmasks
 *
 * Compares two IP addresses with netmasks. The masks are used to mask
 * out the bits that are to be compared.
 *
 * Example:
 \code
 uip_ipaddr_t ipaddr1, ipaddr2, mask;

 uip_ipaddr(&mask, 255,255,255,0);
 uip_ipaddr(&ipaddr1, 192,16,1,2);
 uip_ipaddr(&ipaddr2, 192,16,1,3);
 if(uip_ipaddr_maskcmp(&ipaddr1, &ipaddr2, &mask)) {
 printf("They are the same");
 }
 \endcode
 *
 * \param addr1 The first IP address.
 * \param addr2 The second IP address.
 * \param mask The netmask.
 *
 * \hideinitializer
 */
#define uip_ipaddr_prefixcmp(addr1, addr2, length) (memcmp(addr1, addr2, length>>3) == 0)

/**
 * Process Ip v6 packet
 */
void netstackIPv6Process(void);

/**
 * Switch src->dest, src=nic.mac.
 */
void swithEthAddresses(void);

/**
 * Init an IPv6 packet.
 */
void ipv6Init(void);

/**
 * Check IPv6 packet length.
 */
uint8_t checkPacketLen(void);

/**
 * @param myIp   - ipv6 Address
 */
void ipSetConfigIpv6(uint32_t myIp);

/**
 * Get our local IPv6 configuration.
 * @return pointer to current IPv6 address/configuration.
 */
//struct ipv6Config* ipv6GetConfig(void);

/**
 * Print IPv6 configuration
 * @param straem - input stream
 * @param *config - pointer to IPv6 config struct
 */
void ipv6PrintConfig(FILE *stream, struct uip_netif* config);

/**
 * Send an IPv6 packet.
 */
void ipv6Send(uint8_t protocol, uint16_t len);



#if IP_DEBUG
/**
 * Init debug stram
 */
void ipv6DebugInit(FILE *inDebugStream);

/**
 * Print IPv6 header information.
 */
void netPrintIpv6Header(FILE *stream);

/**
 * Print IPv6 header information. (Raw bits - hex format)
 */
void netPrintIpv6HeaderRAW(FILE *stream);

/**
 * Print IPv6 payload. (Raw bits - hex format)
 */
void netPrintIpv6PayloadRAW(FILE *stream);
#endif /*IP_DEBUG*/

#endif /* IPV6_SUPPORT */
#endif /*IPV6_H*/