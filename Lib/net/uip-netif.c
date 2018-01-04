
/**
 * \file
 *         Network interface and configuration 
 */

/*
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 */

#include "uip-netif.h"
#include <string.h>
#include <stdlib.h>
#include "include/uip-netif.h"

/*---------------------------------------------------------------------------*/


void uip_netif_init(void)
{    
  uip_netif_physical_if = xmalloc(sizeof(struct uip_netif));
  /* INITIALIZE INTERFACE (default values for now) */
  uip_netif_physical_if->link_mtu = UIP_LINK_MTU;
  uip_netif_physical_if->cur_hop_limit = UIP_TTL;
  uip_netif_physical_if->base_reachable_time = UIP_ND6_REACHABLE_TIME;
  uip_netif_physical_if->reachable_time = 0;//uip_netif_compute_reachable_time(); //to trzeba zmienić w przypadku potrowania całęgo ND
  uip_netif_physical_if->retrans_timer = UIP_ND6_RETRANS_TIMER;
  uip_netif_physical_if->dup_addr_detect_transmit = 0;

  /*
   * STATELESS AUTOCONFIGURATION of the link local address. We set it to 
   * infinite (this will become really true once DAD succeeds)
   */
  uip_ip6addr(&(uip_netif_physical_if->addresses[0].ipaddr),
              0xfe80,0,0,0,0,0,0,0);
  uip_netif_addr_autoconf_set(&(uip_netif_physical_if->addresses[0].ipaddr), &nicState.mac.addr);
  uip_netif_physical_if->addresses[0].state = TENTATIVE; 
  uip_netif_physical_if->addresses[0].type = MANUAL;
  uip_netif_physical_if->addresses[0].is_infinite = 1;
 
  /* set all other addresses to NOT_USED initialy */
  uint8_t i;
  for(i = 1; i < UIP_CONF_NETIF_MAX_ADDRESSES; i ++)
    uip_netif_physical_if->addresses[i].state = NOT_USED;  
 
  uip_ip6addr_u8(&(uip_netif_physical_if->solicited_node_mcastaddr),
                 0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01, 0xff,
		 nicState.mac.addr[3],
		 nicState.mac.addr[4],
		 nicState.mac.addr[5]);
   /* Find router (send rs to all-routers multicast group)) */
  //uip_netif_sched_send_rs();   
}

void uip_netif_addr_autoconf_set(uip_ipaddr_t *ipaddr, struct netEthAddr *lladdr)
{
  /* We consider only links with IEEE EUI-64 identifier or
     IEEE 48-bit MAC addresses */
#if (UIP_LLADDR_LEN == 8)
  memcpy(ipaddr->u8 + 8, lladdr->addr, UIP_LLADDR_LEN);
  ipaddr->u8[8] ^= 0x02;  
#elif (UIP_LLADDR_LEN == 6)
  memcpy(ipaddr->u8 + 8, lladdr->addr, 3);
  ipaddr->u8[11] = 0xff;
  ipaddr->u8[12] = 0xfe;
  memcpy(ipaddr->u8 + 13, (u8_t*)lladdr->addr + 3, 3);
  ipaddr->u8[8] ^= 0x02;
#else
  //UIP_LOG("CAN NOT BUIL INTERFACE IDENTIFIER");
#endif
}

/*---------------------------------------------------------------------------*/
struct uip_netif_addr *uip_netif_addr_lookup(uip_ipaddr_t *ipaddr, u8_t length, uip_netif_type type) 
{
    uint8_t i;
    for(i = 0; i < UIP_CONF_NETIF_MAX_ADDRESSES; i ++) {
    if((uip_netif_physical_if->addresses[i].state != NOT_USED) &&
       (uip_netif_physical_if->addresses[i].type == type || type == 0) &&
       (uip_ipaddr_prefixcmp(&(uip_netif_physical_if->addresses[i].ipaddr), ipaddr, length))) { 
      return &uip_netif_physical_if->addresses[i]; 
    }
  }
  //return NULL; 
  
  return &uip_netif_physical_if->addresses[0];
  
  return NULL; 
}


/** @} */
