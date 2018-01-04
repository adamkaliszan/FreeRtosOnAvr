#ifndef SOFTWARE_CONFIG_H
#define SOFTWARE_CONFIG_H 1

#include <inttypes.h>

/* ************ Debug options ***************************************** */
#define UDP_DEBUG  1
#define IP_DEBUG   1
#define ARP_DEBUG  1
#define ICMP_DEBUG 1
#define TCP_DEBUG  1

/* Only one language can be available */
#define LANG_EN 1
#define LANG_PL 0


#define MAX_NUMBER_OF_ROLLERS 10


/* maximum number of TCP connections */
#define NUMBER_OF_SOCKETS 20

/* CLI */
#define CMD_STATE_HISTORY 4
#define CMD_STATE_HISTORY_MASK 0x03


/* Telnet */
#define MYTELNETPOERT 25000


uint8_t wwwport; // 80 is just a default value. Gets overwritten during init
//int16_t info_hdr_len=0;
//int16_t info_data_len=0;


#define SEQNUM 0xFA000000; // my initial tcp sequence number

#define ARP_TABLE_SIZE          10
#define ARP_CACHE_TIME_TO_LIVE  128


#define MYWWWPORT    80
#define MYTELNETPORT 23
#define MYUDPPORT    1200

#define MY_IP1 192
#define MY_IP2 168
#define MY_IP3 0
#define MY_IP4 2

#define MY_GW1 192
#define MY_GW2 168
#define MY_GW3 0
#define MY_GW4 1

#define MY_MASK1 255
#define MY_MASK2 255
#define MY_MASK3 255
#define MY_MASK4 0

#define UDP_SRC_PORT 3000
#define UDP_DST_PORT 0

#define UDP_DST_IP1 192
#define UDP_DST_IP2 168
#define UDP_DST_IP3 0
#define UDP_DST_IP4 1

#define MAC_ADR1 0x12
#define MAC_ADR2 0x34
#define MAC_ADR3 0x56
#define MAC_ADR4 0x78
#define MAC_ADR5 0x9A
#define MAC_ADR6 0xBC



//#define NUMBER_OF_UDP_SOCK

#endif
