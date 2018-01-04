/**
 * Konfiguracja protokołu IPv6
 * 
 */


/** IPv6 support  - add UIP_CONF_IPV6 flag for compatibility with uip Adam Dunkel project*/
#if IPV6_SUPPORT
#define UIP_CONF_IPV6 1
#else
#define UIP_CONF_IPV6 0
#endif

#if UIP_CONF_IPV6

/** Types compatibility */
#define u8_t uint8_t 
#define u16_t uint16_t

/**
 * The size of the uIP packet buffer.
 *
 * The uIP packet buffer should not be smaller than 60 bytes, and does
 * not need to be larger than 1514 bytes. Lower size results in lower
 * TCP throughput, larger size results in higher TCP throughput.
 *
 * \hideinitializer
 */
#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_BUFSIZE UIP_LINK_MTU + UIP_LLH_LEN
#else /* UIP_CONF_BUFFER_SIZE */
#define UIP_BUFSIZE UIP_CONF_BUFFER_SIZE
#endif /* UIP_CONF_BUFFER_SIZE */

/** The maximum transmission unit at the IP Layer*/
#define UIP_LINK_MTU 1280

#ifndef UIP_LITTLE_ENDIAN
#define UIP_LITTLE_ENDIAN  3412
#endif /* UIP_LITTLE_ENDIAN */
#ifndef UIP_BIG_ENDIAN
#define UIP_BIG_ENDIAN     1234
#endif /* UIP_BIG_ENDIAN */



#endif /* UIP_CONF_IPV6 */