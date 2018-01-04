/**
 * vim:sw=2:ts=2:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 *
 * @author Guido Socher 
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * IP, Arp, UDP and TCP functions.
 *
 * The TCP implementation uses some size optimisations which are valid
 * only if all data can be sent in one single packet. This is however
 * not a big limitation for a microcontroller as you will anyhow use
 * small web-pages. The TCP stack is therefore a SDP-TCP stack (single data packet TCP).
 *
 * A big advantage of this stack is that it can actually handle really serious
 * traffic. You can have many (e.g 100) users accessing a web-page almost in parallel.
 *
 * Chip type           : atmega88/atmega168/atmega328 with enc28j60
 *********************************************/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "net.h"
#include "enc28j60.h"
#include "softwareConfig.h"

//@{

extern uint8_t wwwport;
extern uint8_t macaddr[6];
extern uint8_t ipaddr[4];
extern int16_t info_hdr_len;
extern int16_t info_data_len;
extern uint8_t seqnum; // my initial tcp sequence number



uint16_t checksum(uint8_t *buf1, uint16_t len,uint8_t type)
{
  // type 0=ip 
  //      1=udp
  //      2=tcp
  uint32_t sum = 0;

  //if(type==0){
  //        // do not add anything
  //}
  if(type==1)
  {
    sum+=IP_PROTO_UDP; // protocol udp
    // the length here is the length of udp (data+header len)
    // =length given to this function - (IP.scr+IP.dst length)
    sum+=len-8; // = real tcp len
  }
  if(type==2)
  {
    sum+=IP_PROTO_TCP; 
    // the length here is the length of tcp (data+header len)
    // =length given to this function - (IP.scr+IP.dst length)
    sum+=len-8; // = real tcp len
  }
  // build the sum of 16bit words
  while(len >1)
  {
    sum += 0xFFFF & (((uint32_t)*buf1<<8)|*(buf1+1));
    buf1+=2;
    len-=2;
  }
  // if there is a byte left then add it (padded with zero)
  if (len)
  {
    sum += ((uint32_t)(0xFF & *buf1))<<8;
  }
  // now calculate the sum over the bytes in the sum
  // until the result is only 16bit long
  while (sum>>16)
  {
    sum = (sum & 0xFFFF)+(sum >> 16);
  }
  // build 1's complement:
  return( (uint16_t) sum ^ 0xFFFF);
}

// you must call this function once before you use any of the other functions:
void init_ip_arp_udp_tcp(uint8_t *mymac,uint8_t *myip,uint8_t wwwp)
{
  uint8_t i=0;
  wwwport=wwwp;
  while(i<4)
  {
    ipaddr[i]=myip[i];
    i++;
  }
  i=0;
  while(i<6)
  {
    macaddr[i]=mymac[i];
    i++;
  }
}

uint8_t eth_type_is_arp_and_my_ip(uint8_t *buf1,uint16_t len)
{
  uint8_t i=0;

  if (len<41)
    return(0);

  if(buf1[ETH_TYPE_H_P] != ETHTYPE_ARP_H_V || buf1[ETH_TYPE_L_P] != ETHTYPE_ARP_L_V)
    return(0);

  while(i<4)
  {
    if(buf1[ETH_ARP_DST_IP_P+i] != ipaddr[i])
      return(0);
    i++;
  }
  return(1);
}

uint8_t eth_type_is_ip_and_my_ip(uint8_t *buf1,uint16_t len)
{
  uint8_t i=0;
  //eth+ip+udp header is 42
  if (len<42)
  {
    return(0);
  }
  if(buf1[ETH_TYPE_H_P]!=ETHTYPE_IP_H_V || buf1[ETH_TYPE_L_P]!=ETHTYPE_IP_L_V)
    return(0);

  if (buf1[IP_HEADER_LEN_VER_P]!=0x45)
  {
    // must be IP V4 and 20 byte header
    return(0);
  }
  while(i<4)
  {
    if(buf1[IP_DST_P+i]!=ipaddr[i])
    {
      return(0);
    }
    i++;
  }
  return(1);
}

// make a return eth header from a received eth packet
void make_eth(uint8_t *buf)
{
  uint8_t i=0;
  //
  //copy the destination mac from the source and fill my mac into src
  while(i<6)
  {
    buf[ETH_DST_MAC +i]=buf[ETH_SRC_MAC +i];
    buf[ETH_SRC_MAC +i]=macaddr[i];
    i++;
  }
}

void fill_ip_hdr_checksum(uint8_t *buf1)
{
  uint16_t ck;
  // clear the 2 byte checksum
  buf1[IP_CHECKSUM_P]=0;
  buf1[IP_CHECKSUM_P+1]=0;
  buf1[IP_FLAGS_P]=0x40; // don't fragment
  buf1[IP_FLAGS_P+1]=0;  // fragement offset
  buf1[IP_TTL_P]=64; // ttl
  // calculate the checksum:
  ck=checksum(&buf1[IP_P], IP_HEADER_LEN,0);
  buf1[IP_CHECKSUM_P]=ck>>8;
  buf1[IP_CHECKSUM_P+1]=ck& 0xff;
}

void make_ip(uint8_t *buf1)
{
  uint8_t i=0;
  while(i<4)
  {
    buf1[IP_DST_P+i]=buf1[IP_SRC_P+i];
    buf1[IP_SRC_P+i]=ipaddr[i];
    i++;
  }
  fill_ip_hdr_checksum(buf1);
}

void make_tcphead(uint8_t *buf1,uint16_t rel_ack_num,uint8_t mss,uint8_t cp_seq)
{
        uint8_t i=0;
        uint8_t tseq;
        while(i<2){
                buf1[TCP_DST_PORT_H_P+i]=buf1[TCP_SRC_PORT_H_P+i];
                buf1[TCP_SRC_PORT_H_P+i]=0; // clear source port
                i++;
        }
        // set source port  (http):
        buf1[TCP_SRC_PORT_L_P]=wwwport;
        i=4;
        // sequence numbers:
        // add the rel ack num to SEQACK
        while(i>0){
                rel_ack_num=buf1[TCP_SEQ_H_P+i-1]+rel_ack_num;
                tseq=buf1[TCP_SEQACK_H_P+i-1];
                buf1[TCP_SEQACK_H_P+i-1]=0xff&rel_ack_num;
                if (cp_seq){
                        // copy the acknum sent to us into the sequence number
                        buf1[TCP_SEQ_H_P+i-1]=tseq;
                }else{
                        buf1[TCP_SEQ_H_P+i-1]= 0; // some preset vallue
                }
                rel_ack_num=rel_ack_num>>8;
                i--;
        }
        if (cp_seq==0){
                // put inital seq number
                buf1[TCP_SEQ_H_P+0]= 0;
                buf1[TCP_SEQ_H_P+1]= 0;
                // we step only the second byte, this allows us to send packts 
                // with 255 bytes or 512 (if we step the initial seqnum by 2)
                buf1[TCP_SEQ_H_P+2]= seqnum; 
                buf1[TCP_SEQ_H_P+3]= 0;
                // step the inititial seq num by something we will not use
                // during this tcp session:
                seqnum+=2;
        }
        // zero the checksum
        buf1[TCP_CHECKSUM_H_P]=0;
        buf1[TCP_CHECKSUM_L_P]=0;

        // The tcp header length is only a 4 bit field (the upper 4 bits).
        // It is calculated in units of 4 bytes. 
        // E.g 24 bytes: 24/4=6 => 0x60=header len field
        //buf1[TCP_HEADER_LEN_P]=(((TCP_HEADER_LEN_PLAIN+4)/4)) <<4; // 0x60
        if (mss){
                // the only option we set is MSS to 1408:
                // 1408 in hex is 0x580
                buf1[TCP_OPTIONS_P]=2;
                buf1[TCP_OPTIONS_P+1]=4;
                buf1[TCP_OPTIONS_P+2]=0x05; 
                buf1[TCP_OPTIONS_P+3]=0x80;
                // 24 bytes:
                buf1[TCP_HEADER_LEN_P]=0x60;
        }else{
                // no options:
                // 20 bytes:
                buf1[TCP_HEADER_LEN_P]=0x50;
        }
}

void make_arp_answer_from_request(uint8_t *buf1)
{
        uint8_t i=0;
        //
        make_eth(buf1);
        buf1[ETH_ARP_OPCODE_H_P]=ETH_ARP_OPCODE_REPLY_H_V;
        buf1[ETH_ARP_OPCODE_L_P]=ETH_ARP_OPCODE_REPLY_L_V;
        // fill the mac addresses:
        while(i<6){
                buf1[ETH_ARP_DST_MAC_P+i]=buf1[ETH_ARP_SRC_MAC_P+i];
                buf1[ETH_ARP_SRC_MAC_P+i]=macaddr[i];
                i++;
        }
        i=0;
        while(i<4){
                buf1[ETH_ARP_DST_IP_P+i]=buf1[ETH_ARP_SRC_IP_P+i];
                buf1[ETH_ARP_SRC_IP_P+i]=ipaddr[i];
                i++;
        }
        // eth+arp is 42 bytes:
        enc28j60PacketSend(42, buf1); 
}

void make_echo_reply_from_request(uint8_t *buf1,uint16_t len)
{
        make_eth(buf1);
        make_ip(buf1);
        buf1[ICMP_TYPE_P]=ICMP_TYPE_ECHOREPLY_V;
        // we changed only the icmp.type field from request(=8) to reply(=0).
        // we can therefore easily correct the checksum:
        if (buf1[ICMP_CHECKSUM_P] > (0xff-0x08)){
                buf1[ICMP_CHECKSUM_P+1]++;
        }
        buf1[ICMP_CHECKSUM_P]+=0x08;
        //
        enc28j60PacketSend(len,buf1);
}

// you can send a max of 220 bytes of data
void make_udp_reply_from_request(uint8_t *buf1, char *data, uint8_t datalen, uint16_t port)
{
        uint8_t i=0;
        uint16_t ck;
        make_eth(buf1);
        if (datalen>220){
                datalen=220;
        }
        // total length field in the IP header must be set:
        buf1[IP_TOTLEN_H_P]=0;
        buf1[IP_TOTLEN_L_P]=IP_HEADER_LEN+UDP_HEADER_LEN+datalen;
        make_ip(buf1);
        // send to port:
        //buf1[UDP_DST_PORT_H_P]=port>>8;
        //buf1[UDP_DST_PORT_L_P]=port & 0xff;
        // sent to port of sender and use "port" as own source:
        buf1[UDP_DST_PORT_H_P]=buf1[UDP_SRC_PORT_H_P];
        buf1[UDP_DST_PORT_L_P]= buf1[UDP_SRC_PORT_L_P];
        buf1[UDP_SRC_PORT_H_P]=port>>8;
        buf1[UDP_SRC_PORT_L_P]=port & 0xff;
        // calculte the udp length:
        buf1[UDP_LEN_H_P]=0;
        buf1[UDP_LEN_L_P]=UDP_HEADER_LEN+datalen;
        // zero the checksum
        buf1[UDP_CHECKSUM_H_P]=0;
        buf1[UDP_CHECKSUM_L_P]=0;
        // copy the data:
  while(i<datalen)
  {
    buf1[UDP_DATA_P+i]=data[i];
    i++;
  }
  ck=checksum(&buf1[IP_SRC_P], 16 + datalen,1);
  buf1[UDP_CHECKSUM_H_P]=ck>>8;
  buf1[UDP_CHECKSUM_L_P]=ck& 0xff;
  enc28j60PacketSend(UDP_HEADER_LEN+IP_HEADER_LEN+ETH_HEADER_LEN+datalen, buf1);
}

void make_tcp_synack_from_syn(uint8_t *buf1)
{
  uint16_t ck;
  make_eth(buf1);
  // total length field in the IP header must be set:
  // 20 bytes IP + 24 bytes (20tcp+4tcp options)
  buf1[IP_TOTLEN_H_P]=0;
  buf1[IP_TOTLEN_L_P]=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4;
  make_ip(buf1);
  buf1[TCP_FLAGS_P]=TCP_FLAGS_SYNACK_V;
  make_tcphead(buf1, 1, 1, 0);
  // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + 4 (one option: mss)
  ck=checksum(&buf1[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN+4,2);
  buf1[TCP_CHECKSUM_H_P]=ck>>8;
  buf1[TCP_CHECKSUM_L_P]=ck& 0xff;
  // add 4 for option mss:
  enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4+ETH_HEADER_LEN, buf1);
}

uint16_t get_tcp_data_pointer(void)
{
  if (info_data_len)
  {
    return((uint16_t)TCP_SRC_PORT_H_P+info_hdr_len);
  }
  else
  {
    return(0);
  }
}

void init_len_info(uint8_t *buf1)
{
  info_data_len=(((int16_t)buf1[IP_TOTLEN_H_P])<<8)|(buf1[IP_TOTLEN_L_P]&0xff);
  info_data_len-=IP_HEADER_LEN;
  info_hdr_len=(buf1[TCP_HEADER_LEN_P]>>4)*4; // generate len in bytes;
  info_data_len-=info_hdr_len;
  if (info_data_len<=0)
  {
    info_data_len=0;
  }
}

char *getBufPosToWrite(uint8_t *buf, uint16_t pos)
{
  return &buf[TCP_CHECKSUM_L_P+3+pos];  
}

uint16_t fill_tcp_data_p(uint8_t *buf1, uint16_t pos, const prog_char *progmem_s)
{
  char c;
  // fill in tcp data at position pos
  //
  // with no options the data starts after the checksum + 2 more bytes (urgent ptr)
  while ((c = pgm_read_byte(progmem_s++)))
  {
    buf1[TCP_CHECKSUM_L_P+3+pos]=c;
    pos++;
  }
  return(pos);
}

uint16_t fill_tcp_data(uint8_t *buf1,uint16_t pos, const char *s)
{
  // fill in tcp data at position pos
  //
  // with no options the data starts after the checksum + 2 more bytes (urgent ptr)
  while (*s)
  {
    buf1[TCP_CHECKSUM_L_P+3+pos]=*s;
    pos++;
    s++;
  }
  return(pos);
}

uint16_t add_tcp_byte(uint8_t *buf1,uint16_t pos, const char data)
{
  buf1[TCP_CHECKSUM_L_P+3+pos]=data;
  pos++;

  return(pos);
}


void make_tcp_ack_from_any(uint8_t *buf1)
{
  uint16_t j;
  make_eth(buf1);
  // fill the header:
  buf1[TCP_FLAGS_P]=TCP_FLAGS_ACK_V;
  if (info_data_len==0)
  {
    // if there is no data then we must still acknoledge one packet
    make_tcphead(buf1, 1, 0, 1); // no options
  }
  else
  {
    make_tcphead(buf1,info_data_len,0,1); // no options
  }

  // total length field in the IP header must be set:
  // 20 bytes IP + 20 bytes tcp (when no options) 
  j = IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN;
  buf1[IP_TOTLEN_H_P] = j>>8;
  buf1[IP_TOTLEN_L_P]=j& 0xff;
  make_ip(buf1);
  // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
  j=checksum(&buf1[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN,2);
  buf1[TCP_CHECKSUM_H_P]=j>>8;
  buf1[TCP_CHECKSUM_L_P]=j& 0xff;
  enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+ETH_HEADER_LEN,buf1);
}

void make_tcp_ack_with_data(uint8_t *buf1,uint16_t dlen)
{
  uint16_t j;
  // fill the header:
  // This code requires that we send only one data packet
  // because we keep no state information. We must therefore set
  // the fin here:
  buf1[TCP_FLAGS_P]=TCP_FLAGS_ACK_V|TCP_FLAGS_PUSH_V|TCP_FLAGS_FIN_V;

  // total length field in the IP header must be set:
  // 20 bytes IP + 20 bytes tcp (when no options) + len of data
  j=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+dlen;
  buf1[IP_TOTLEN_H_P]=j>>8;
  buf1[IP_TOTLEN_L_P]=j& 0xff;
  fill_ip_hdr_checksum(buf1);
  // zero the checksum
  buf1[TCP_CHECKSUM_H_P]=0;
  buf1[TCP_CHECKSUM_L_P]=0;
  // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
  j=checksum(&buf1[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN+dlen, 2);
  buf1[TCP_CHECKSUM_H_P]=j>>8;
  buf1[TCP_CHECKSUM_L_P]=j& 0xff;
  enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+dlen+ETH_HEADER_LEN, buf1);
}
//@}
/* end of ip_arp_udp.c */
