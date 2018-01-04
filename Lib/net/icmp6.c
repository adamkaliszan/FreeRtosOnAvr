/**
 * @file icmp6.c 
 * @brief ICMPv6 
 *
 */

#include "icmp6.h"
#include "include/nic.h"
#include "include/icmp6.h"

/** \brief temporary IP address */
static uip_ipaddr_t tmp_ipaddr;


void vICMP6DebugInit(FILE *stream)
{
  debugStream = stream;
}

void
uip_icmp6_echo_request_input(void)
{
  /*
   * we send an echo reply. It is trivial if there was no extension
   * headers in the request otherwise we need to remove the extension
   * headers and change a few fields
   */
  
  fprintf_P(debugStream, PSTR("Echo request\n\r"));
  
  /* IP header */
  //UIP_IP_BUF->ttl = uip_netif_physical_if.cur_hop_limit;
  UIP_IP_BUF->ttl = 64;

  /*
  if(uip_is_addr_mcast(&UIP_IP_BUF->destipaddr)){
    uip_ipaddr_copy(&UIP_IP_BUF->destipaddr, &UIP_IP_BUF->srcipaddr);
    uip_netif_select_src(&UIP_IP_BUF->srcipaddr, &UIP_IP_BUF->destipaddr);
  } else {
    */
    //uip_ipaddr_copy(&tmp_ipaddr, &UIP_IP_BUF->srcipaddr);
    //uip_ipaddr_copy(&UIP_IP_BUF->srcipaddr, &UIP_IP_BUF->destipaddr);
    //uip_ipaddr_copy(&UIP_IP_BUF->destipaddr, &tmp_ipaddr);
    
    memcpy(&tmp_ipaddr, &UIP_IP_BUF->srcipaddr, sizeof(uip_ip6addr_t));
    memcpy(&UIP_IP_BUF->srcipaddr, &UIP_IP_BUF->destipaddr, sizeof(uip_ip6addr_t));
    memcpy(&UIP_IP_BUF->destipaddr, &tmp_ipaddr, sizeof(uip_ip6addr_t));
 // }

  //if(uip_ext_len > 0) {
    /* If there were extension headers*/
    
    
  /* Below is important for the correctness of UIP_ICMP_BUF and the
   * checksum
   *
  uip_ext_len = 0;
  /* Note: now UIP_ICMP_BUF points to the beginning of the echo reply */
  UIP_ICMP_BUF->type = ICMP6_ECHO_REPLY;
  UIP_ICMP_BUF->icode = 0;
  UIP_ICMP_BUF->icmpchksum = 0;
  UIP_ICMP_BUF->icmpchksum = ~uip_icmp6chksum();
 
 
  return;
}








