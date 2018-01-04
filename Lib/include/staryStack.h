/*********************************************
 * vim:sw=2:ts=2:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 *         Adam Kaliszan
 * Copyright: GPL V2
 *
 * IP/ARP/UDP/TCP functions
 *
 *********************************************/
#ifndef IP_ARP_UDP_TCP_H
#define IP_ARP_UDP_TCP_H
#include <avr/pgmspace.h>

#include "enc_task.h"
//@{

/**
 * Calculates CRC 16 bit checksum
 *   The Ip checksum is calculated over the ip header only starting
 *   with the header length field and a total length of 20 bytes unitl ip.dst
 *   You must set the IP checksum field to zero before you start the calculation. 
 *   len for ip is 20.
 *   For UDP/TCP we do not make up the required pseudo header. Instead we 
 *   use the ip.src and ip.dst fields of the real packet:
 *   The udp checksum calculation starts with the ip.src field
 *   Ip.src=4bytes,Ip.dst=4 bytes,Udp header=8bytes + data length=16+len
 *   In other words the len here is 8 + length over which you actually
 *   want to calculate the checksum.
 *   You must set the checksum field to zero before you start
 *   the calculation.
 *   len for udp is: 8 + 8 + data length
 *   len for tcp is: 4+4 + 20 + option len + data length
 * 
 *   For more information on how this algorithm works see:
 *   http://www.netfor2.com/checksum.html
 *   http://www.msc.uky.edu/ken/cs471/notes/chap3.htm
 *   The RFC has also a C code example: http://www.faqs.org/rfcs/rfc1071.html
 * @param *buf1 - pointer to the buffer
 * @param length - buffer length
 * @param type
 * @return CRC
 */
uint16_t checksum                    (uint8_t *buf1, uint16_t len, uint8_t type);
void     make_eth                    (uint8_t *buf1);
void     fill_ip_hdr_checksum        (uint8_t *buf1);

/**
 * make a return ip header from a received ip packet
 */
void     make_ip                     (uint8_t *buf1);

/**
 * make a return tcp header from a received tcp packet
 * rel_ack_num is how much we must step the seq number received from the
 * other side. We do not send more than 255 bytes of text (=data) in the tcp packet.
 * If mss=1 then mss is included in the options list
 *
 * After calling this function you can fill in the first data byte at TCP_OPTIONS_P+4
 * If cp_seq=0 then an initial sequence number is used (should be use in synack)
 * otherwise it is copied from the packet we received
 */
void     make_tcphead                (uint8_t *buf1,uint16_t rel_ack_num,uint8_t mss,uint8_t cp_seq);

// you must call this function once before you use any of the other functions:
void     init_ip_arp_udp_tcp         (uint8_t *mymac,uint8_t *myip,uint8_t wwwp);

uint8_t  eth_type_is_arp_and_my_ip   (uint8_t *buf1,uint16_t len);
uint8_t  eth_type_is_ip_and_my_ip    (uint8_t *buf1,uint16_t len);
void     make_arp_answer_from_request(uint8_t *buf1);
void     make_echo_reply_from_request(uint8_t *buf1,uint16_t len);
void     make_udp_reply_from_request (uint8_t *buf1,char *data,uint8_t datalen,uint16_t port);

void     make_tcp_synack_from_syn    (uint8_t *buf1);

/**
 *do some basic length calculations and store the result in static varibales
 */
void     init_len_info               (uint8_t *buf1);

/**
 *get a pointer to the start of tcp data in buf
 * Returns 0 if there is no data
 * You must call init_len_info once before calling this function
 */
uint16_t get_tcp_data_pointer        (void);


/**
 * Return memory addres to write to the buffer
 * @param *buffer pointer to the beginning of TCP buffer
 * @param *pos - write index
 */
char *getBufPosToWrite(uint8_t *buf, uint16_t pos);


/**
 * fill in tcp data at position pos. pos=0 means start of tcp data. 
 * @param buf       - output buffer
 * @param pos       - position to write in the buffer
 * @param progmem_s - string stored in flash to write
 * @return the position at which the string after this string could be filled.
 */
uint16_t fill_tcp_data_p             (uint8_t *buf1,uint16_t pos, const prog_char *progmem_s);

/**
 * fill in tcp data at position pos. pos=0 means start of tcp data. 
 * @param buf       - output buffer
 * @param pos       - position to write in the buffer
 * @param s         - string stored in ram to write
 * @return the position at which the string after this string could be filled.
 */
uint16_t fill_tcp_data               (uint8_t *buf1,uint16_t pos, const char *s);

/**
 * fill in tcp data at position pos. pos=0 means start of tcp data. 
 * @param buf       - output buffer
 * @param pos       - position to write in the buffer
 * @param data      - one byte to write
 * @return the position at which the string after this string could be filled.
 */
uint16_t add_tcp_byte               (uint8_t *buf1,uint16_t pos, const char data);

/**
 * Make just an ack packet with no tcp data inside
 * This will modify the eth/ip/tcp header
 */
void     make_tcp_ack_from_any       (uint8_t *buf1);

/**
 * you must have called init_len_info at some time before calling this function
 * dlen is the amount of tcp data (http data) we send in this packet
 * You can use this function only immediately after make_tcp_ack_from_any
 * This is because this function will NOT modify the eth/ip/tcp header except for
 * length and checksum
 */
void     make_tcp_ack_with_data      (uint8_t *buf1, uint16_t dlen);

//@}
#endif /* IP_ARP_UDP_TCP_H */
