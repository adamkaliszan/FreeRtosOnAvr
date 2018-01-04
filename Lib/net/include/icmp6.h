#ifndef ICMP6_H
#define ICMP6_H

/**
 * @file icmp6.h 
 * @brief ICMPv6 
 *
 */


#include "net.h"
#include "nic.h"
#include "softwareConfig.h"
#include "ipv6.h"
#include "checksum.h"

#define PRINTF(...) fprintf_P(__VA_ARGS__)
#define PRINTF2(stream, ...) fprintf_P(stream, PSTR(__VA_ARGS__))
//#define PRINT6ADDR(stream, addr) PRINTF(stream, PSTR("IPv6 addr: %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x \r\n"), ((u8_t *)addr)[0], ((u8_t *)addr)[1], ((u8_t *)addr)[2], ((u8_t *)addr)[3], ((u8_t *)addr)[4], ((u8_t *)addr)[5], ((u8_t *)addr)[6], ((u8_t *)addr)[7], ((u8_t *)addr)[8], ((u8_t *)addr)[9], ((u8_t *)addr)[10], ((u8_t *)addr)[11], ((u8_t *)addr)[12], ((u8_t *)addr)[13], ((u8_t *)addr)[14], ((u8_t *)addr)[15])





extern nicState_t nicState;

/** \name ICMPv6 message types */
/** @{ */
#define ICMP6_DST_UNREACH               1    /**< dest unreachable */
#define ICMP6_PACKET_TOO_BIG	        2    /**< packet too big */
#define ICMP6_TIME_EXCEEDED	        3    /**< time exceeded */
#define ICMP6_PARAM_PROB	        4    /**< ip6 header bad */
#define ICMP6_ECHO_REQUEST              128  /**< Echo request */
#define ICMP6_ECHO_REPLY                129  /**< Echo reply */

#define ICMP6_RS                        133  /**< Router Solicitation */
#define ICMP6_RA                        134  /**< Router Advertisement */
#define ICMP6_NS                        135  /**< Neighbor Solicitation */
#define ICMP6_NA                        136  /**< Neighbor advertisement */
#define ICMP6_REDIRECT                  137  /**< Redirect */
/** @} */
#define UIP_ND6_OPT_LLAO_LEN           8

/** \name ND6 message length (excluding options) */
/** @{ */
#define UIP_ND6_NA_LEN                  20  
#define UIP_ND6_NS_LEN                  20
/** @} */

/** \name ND6 option types */
/** @{ */
#define UIP_ND6_OPT_SLLAO               1
#define UIP_ND6_OPT_TLLAO               2
#define UIP_ND6_OPT_PREFIX_INFO         3
#define UIP_ND6_OPT_REDIRECTED_HDR      4
#define UIP_ND6_OPT_MTU                 5

/** \brief HOP LIMIT to be used when sending ND messages (255) */
#define UIP_ND6_HOP_LIMIT               0xff


/** \name Neighbor Advertisement flags masks */
/** @{ */
#define UIP_ND6_NA_FLAG_ROUTER          0x80
#define UIP_ND6_NA_FLAG_SOLICITED       0x40
#define UIP_ND6_NA_FLAG_OVERRIDE        0x20
/** @} */

/**
 * \name ND message structures
 * @{
 */

/** 
 * \brief A neighbor solicitation constant part
 * 
 * Possible option is: SLLAO
 */
struct uip_nd6_ns {
  uint32_t reserved;
  uip_ipaddr_t tgtipaddr;
};

/**
 * \brief A neighbor advertisement constant part.
 * 
 * Possible option is: TLLAO
 */ 
struct uip_nd6_na {
  uint8_t flagsreserved;
  uint8_t reserved[3];
  uip_ipaddr_t tgtipaddr;
};


/**
 * The sums below are quite used in ND. 
 */
//#define uip_l2_l3_hdr_len (ETH_HEADER_LEN + UIP_IPH_LEN + uip_ext_len)
////#define uip_l2_l3_icmp_hdr_len (ETH_HEADER_LEN + UIP_IPH_LEN + uip_ext_len + UIP_ICMPH_LEN)
////#define uip_l3_hdr_len (IP_HEADER_LEN + uip_ext_len)
//#define uip_l2_l3_icmp_hdr_len (ETH_HEADER_LEN + UIP_IPH_LEN + uip_ext_len + UIP_ICMPH_LEN)

//#define uip_l2_l3_icmp_hdr_len  ETH_HEADER_LEN + IP_HEADER_LEN + uip_ext_len + UIP_ICMPH_LEN
#define UIP_ND6_NS_BUF     ((struct uip_nd6_ns *)&nicState.layer2.buf[uip_l2_l3_icmp_hdr_len])

/** Pointer to ND option */
#define UIP_ND6_OPT_HDR_BUF  ((struct uip_nd6_opt_hdr *)&nicState.layer2.buf[uip_l2_l3_icmp_hdr_len + nd6_opt_offset])

//#define UIP_IP_BUF                          ((struct uip_ip_hdr *)&nicState.layer2.buf[14]) 
//#define UIP_ICMP_BUF                        ((struct uip_icmp6_hdr *)&nicState.layer2.buf[uip_l2_l3_hdr_len])
#define UIP_ND6_NA_BUF                      ((struct uip_nd6_na *)&nicState.layer2.buf[uip_l2_l3_icmp_hdr_len])


/** \brief ND option header */
struct uip_nd6_opt_hdr {
  u8_t type;
  u8_t len;
};

/** \brief ND option: both TLLAO and SLLAO */
struct uip_nd6_opt_llao {
  u8_t type;
  u8_t len;
  struct netEthAddr addr;
};


/**
 * The length of the extension headers
 */
extern uint8_t uip_ext_len;



/** \name ICMPv6 RFC4443 Message processing and sending */
/** @{ */
/** \
 * brief Process an echo request 
 *
 * Perform a few checks, then send an Echop reply. The reply is 
 * built here.
  */
void
uip_icmp6_echo_request_input(void);

/**
 * \brief Send an icmpv6 error message
 * \param type type of the Error message
 * \param code of the error message
 * \param type 32 bit parameter of the error message, semantic depends on error 
 */
//void uip_icmp6_error_output(u8_t type, u8_t code, uint32_t param); 



#if ICMP6_DEBUG
/** Debug Network Discovery */
FILE *debugStream; 

/** Assign debug stram */
void vICMP6DebugInit(FILE *stream); 

#endif /**ICMP6_DEBUG*/


#endif/**ICMP6_H*/