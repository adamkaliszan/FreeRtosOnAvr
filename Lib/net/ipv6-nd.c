/**
 * @file ipv6-nd.c 
 * @brief IP Network Discovery 
 * 
 * Please Notice, that we do have a fwe strategies - Router Discovery/Solicitation, Neighbour Discovery/Solicitation
 * For now, only Neighbour Discovery is implemented
 */

#include "ipv6-nd.h"
#include "include/ipv6-nd.h"

/** \brief Offset from the end of the icmpv6 header to the option in uip_buf*/
static u8_t nd6_opt_offset;
/** \brief Pointer to llao option in uip_buf */
static struct uip_nd6_opt_llao *nd6_opt_llao;
/** \brief Pointer to an interface address */
static struct uip_netif_addr *ifaddr;

void vNDinit(void)
{
  entries = xmalloc( ENTRIES * sizeof(neighbor_entry));
  #if IP_DEBUG
  fprintf_P(debugStream, PSTR("ENTRIES %d %d:"), ENTRIES, sizeof(neighbor_entry));
  #endif/**IP_DEBUG*/
  
  for(uint8_t i = 0; i < ENTRIES; ++i) {
    entries[i].time = 0;
  }
}

#if IP_DEBUG
void vNDDebugInit(FILE *stream)
{
  debugStream = stream;
}
#endif/**IP_DEBUG*/

#if JM
/*---------------------------------------------------------------------------*/
void
uip_neighbor_add(uip_ipaddr_t *ipaddr, struct netEthAddr *addr)
{
  uint8_t i, insertPos;
  uint32_t oldestTime = 0xFFFFFFFF;
  
 //#if IP_DEBUG
  fprintf_P(debugStream, PSTR("Adding neighbor with link address %02x:%02x:%02x:%02x:%02x:%02x\n\r"),
	 addr->addr[0], addr->addr[1], addr->addr[2], addr->addr[3],
	 addr->addr[4], addr->addr[5]);
 
  
  fprintf_P(debugStream, PSTR("SizeofInt %d %d \n\r"),
	 sizeof(int), xTaskGetTickCount());
 //#endif/**IP_DEBUG*/
  
 insertPos = 0;
  /* Find the first unused entry or the oldest used entry. */
 for(i = 0; i < ENTRIES; ++i) {
    if(entries[i].time == 0) {
      insertPos = i;
      break;
    }
    if(uip_ipaddr_cmp(&entries[i].ipaddr, ipaddr)) {
      insertPos = i;
      break;
    }
    if(entries[i].time < oldestTime) {
      oldestTime = entries[i].time;
      insertPos =i;
    }
 }
 
   fprintf_P(debugStream, PSTR("oldestTime %d \n\r"),oldestTime);
 
  /* Use the oldest or first free entry (either pointed to by the
     "oldestTime" variable). */
  entries[insertPos].time = (uint32_t)xTaskGetTickCount();
  uip_ipaddr_copy(&entries[insertPos].ipaddr, ipaddr);
  memcpy(&entries[insertPos].addr, addr, sizeof(struct netEthAddr));
  
 //#if IP_DEBUG
  fprintf_P(debugStream, PSTR("EntryPacket[0]:\r\n"));
  for (i =0; i < sizeof(neighbor_entry); i++)
    fprintf_P(debugStream, PSTR("%02x:"), ((uint8_t *)entries)[i]);
  fprintf_P(debugStream, PSTR("\n\r"));
  
  fprintf_P(debugStream, PSTR("link address %02x:%02x:%02x:%02x:%02x:%02x\n\r"),
	 entries[0].addr.addr[0], entries[0].addr.addr[1], entries[0].addr.addr[2], entries[0].addr.addr[3],
	 entries[0].addr.addr[4], entries[0].addr.addr[5]);
  
  fprintf_P(debugStream, PSTR("IP address:\r\n"));
  fprintf_P(debugStream, PSTR("%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x"),
	  entries[0].ipaddr.u8[0], entries[0].ipaddr.u8[1],
	  entries[0].ipaddr.u8[2], entries[0].ipaddr.u8[3],
	  entries[0].ipaddr.u8[4], entries[0].ipaddr.u8[5],
	  entries[0].ipaddr.u8[6], entries[0].ipaddr.u8[7],
	  entries[0].ipaddr.u8[8], entries[0].ipaddr.u8[9],
	  entries[0].ipaddr.u8[10], entries[0].ipaddr.u8[11],
	  entries[0].ipaddr.u8[12], entries[0].ipaddr.u8[13],
	  entries[0].ipaddr.u8[14], entries[0].ipaddr.u8[15]);
  fprintf_P(debugStream, PSTR("\r\n"));
 //#endif/**IP_DEBUG*/
}
/*---------------------------------------------------------------------------*/
neighbor_entry *
find_entry(uip_ipaddr_t *ipaddr)
{
  int i;
  
  for(i = 0; i < ENTRIES; ++i) {
    if(uip_ipaddr_cmp(&entries[i].ipaddr, ipaddr)) {
      return &entries[i];
    }
  }
  return NULL;
}
/*---------------------------------------------------------------------------*/
#endif /*JM*/

void uip_nd6_io_ns_input(uint16_t packetLenght)
{
  
  fprintf_P(debugStream, PSTR("Received NS from\n\r"));
  PRINT6ADDR(debugStream, &nicState.layer3.ipv6->srcipaddr);
  fprintf_P(debugStream, PSTR("to\n\r"));
  PRINT6ADDR(debugStream, &nicState.layer3.ipv6->destipaddr);
  ///Adres z icmpv6 header'a    
  PRINTF2(debugStream,"with target address\n\r");
  PRINT6ADDR(debugStream, (&UIP_ND6_NS_BUF->tgtipaddr));
  /*
  PRINTF2(debugStream,"with target address\n\r");
  fprintf_P(debugStream, PSTR("to (l2 %04x)\n\r"), &nicState.layer2.buf[14+40+4]);
  fprintf_P(debugStream, PSTR("to (l4 %04x)\n\r"), nicState.layer4.buf);
  fprintf_P(debugStream, PSTR("to (l4 %04x)\n\r"), &((struct uip_nd6_ns *)&nicState.layer2.buf[14+40+4])->tgtipaddr);
  PRINT6ADDR(debugStream, &((struct uip_nd6_ns *)&nicState.layer2.buf[14+40+4])->tgtipaddr );
  PRINT6ADDR(debugStream, &((struct uip_nd6_ns *)	(nicState.layer4.buf + 4))->tgtipaddr );
  */
  
  u8_t flags;
 
  
  /* Options reading: we handle only SLLAO for now */
  nd6_opt_llao = NULL;
  nd6_opt_offset = UIP_ND6_NS_LEN;
  
  fprintf_P(debugStream, PSTR("uip_l2_l3_icmp_hdr_len %04x; packetLenght  %04x ; UIP_ND6_OPT_HDR_BUF %04x, nd6_opt_offset %04x\r\n"), 
	                      uip_l2_l3_icmp_hdr_len, packetLenght, 
			      UIP_ND6_OPT_HDR_BUF, nd6_opt_offset );
  fprintf_P(debugStream, PSTR("ICMPv6 payload: \r\n"));
  for (uint8_t i=UIP_ND6_NS_LEN + UIP_ICMPH_LEN; i < 32; i++) ///Tu dlugosc ICMP
  {
  fprintf_P(debugStream, PSTR("%02x:"), nicState.layer3.buf[i+UIP_IPH_LEN]);
  }
  fprintf_P(debugStream, PSTR("\r\n"));
  
  
  while(uip_l2_l3_icmp_hdr_len + nd6_opt_offset < packetLenght) {
#if UIP_CONF_IPV6_CHECKS
    if(UIP_ND6_OPT_HDR_BUF->len == 0) {
      goto badpkt;
    }
#endif /* UIP_CONF_IPV6_CHECKS */ 
    switch(UIP_ND6_OPT_HDR_BUF->type) {
      case UIP_ND6_OPT_SLLAO:
        nd6_opt_llao = (struct uip_nd6_opt_llao *)UIP_ND6_OPT_HDR_BUF;
        break;
      default:
        PRINTF2(debugStream,"ND option not supported in NS");
        break;
    }
    nd6_opt_offset += (UIP_ND6_OPT_HDR_BUF->len << 3);
  }
  
  
	

  /* Options processing: only SLLAO */
  if(nd6_opt_llao != NULL) {
#if UIP_CONF_IPV6_CHECKS
    /* There must be NO option in a DAD NS */
    if(uip_is_addr_unspecified(&UIP_IP_BUF->srcipaddr)) {
      goto badpkt;
    } else {
#endif /*UIP_CONF_IPV6_CHECKS*/
/*
      neighbor = uip_nd6_nbrcache_lookup(&UIP_IP_BUF->srcipaddr);
      if(neighbor == NULL) {
        /* we need to add the neighbor*
        uip_nd6_nbrcache_add(&UIP_IP_BUF->srcipaddr,
                             &nd6_opt_llao->addr, 0, STALE);
      } else {
        /* If LL address changed, set neighbor state to stale *
        if(memcmp(&nd6_opt_llao->addr, &neighbor->lladdr, UIP_LLADDR_LEN) != 0) {
          memcpy(&neighbor->lladdr, &nd6_opt_llao->addr, UIP_LLADDR_LEN);
          neighbor->state = STALE;
        } else {
          /* If neighbor state is INCOMPLETE, set to STALE *
          if(neighbor->state == INCOMPLETE) {
            neighbor->state = STALE;
          }
        }
      }
*/
#if UIP_CONF_IPV6_CHECKS
    }   
#endif /*UIP_CONF_IPV6_CHECKS*/
  }

  /* 
   * Rest of NS processing: Depends on the purpose of the NS: NUD or DAD or
   * Address Resolution 
   */
  ifaddr = xmalloc(sizeof(struct uip_netif_addr));
  /** \note we use ifaddr to remember the target address */
  memcpy(&ifaddr->ipaddr, &UIP_ND6_NS_BUF->tgtipaddr, sizeof(uip_ip6addr_t));
  
    fprintf_P(debugStream, PSTR("ifaddr: ** %04x \r\n"), &ifaddr->ipaddr);
    PRINT6ADDR(debugStream, &ifaddr->ipaddr);
    PRINT6ADDR(debugStream, &UIP_ND6_NS_BUF->tgtipaddr);
  //ifaddr = uip_netif_addr_lookup(&UIP_ND6_NS_BUF->tgtipaddr, 128, 0);  //Sprawdza czy to nasz adres, narazie wylaczone
  if(ifaddr != NULL) 
  {
#if JM
    if(uip_is_addr_unspecified(&UIP_IP_BUF->srcipaddr)){
      /* DAD CASE */
//#if UIP_CONF_IPV6_CHECKS 
      /* Dst address must be solicited node mcast address */
      if(!uip_netif_is_addr_my_solicited(&UIP_IP_BUF->destipaddr)){
        goto badpkt;
      }
//#endif /* UIP_CONF_IPV6_CHECKS */ 
      /*
       * If my address is not tentative, then send a NA to all nodes with
       * TLLAO flags are: override = yes.
       */
        if(ifaddr->state!=TENTATIVE) {  
        /* 
         * we need to send a NA, we set the src, dest, flags. tgt remains the
         * same and the rest is done at "create_na" 
         */
        uip_create_linklocal_allnodes_mcast(&UIP_IP_BUF->destipaddr);
        uip_netif_select_src(&UIP_IP_BUF->srcipaddr, &UIP_IP_BUF->destipaddr);
        flags = UIP_ND6_NA_FLAG_OVERRIDE;
        goto create_na;
      } else {
        /** \todo if I sent a NS before him, I win */
        uip_netif_dad_failed(&UIP_ND6_NS_BUF->tgtipaddr);
        goto discard;
      }
    }
#endif //JM  

#if UIP_CONF_IPV6_CHECKS  
    /* Duplicate check */  						///Tego narazie tez nie robie
    if(uip_netif_is_addr_my_unicast(&UIP_IP_BUF->srcipaddr)) {
      /**
       * \NOTE do we do something here? we both are using the same address.
       * If we are doing dad, we could cancel it, though we should receive a
       * NA in response of DAD NS we sent, hence DAD will fail anyway. If we
       * were not doing DAD, it means there is a duplicate in the network!
       */
      goto badpkt;
    }
#endif /*UIP_CONF_IPV6_CHECKS*/

    ///Bez tego powinno dzialac
    /* Address resolution case */  
    if(0){
    //if(uip_netif_is_addr_my_solicited(&UIP_IP_BUF->destipaddr)){
      /*   
       * we need to send a NA, we set the src, dest, flags. The rest is
       * set at the "create_na" label.
       */
      ///uip_ipaddr_copy(&UIP_IP_BUF->destipaddr, &UIP_IP_BUF->srcipaddr);
      ///uip_ipaddr_copy(&UIP_IP_BUF->srcipaddr, &UIP_ND6_NS_BUF->tgtipaddr);
      ///flags = UIP_ND6_NA_FLAG_SOLICITED | UIP_ND6_NA_FLAG_OVERRIDE;
      goto create_na;
    } 

    /* 
     * NUD CASE. at this point the packet must be for us! we check this, 
     * and at the same time if target == dest
     */
    ///Tu wyszukiwanie adresu jest nie tak - narazie przepuszcze dla tesotwego pakietu na stałe
   //if(uip_netif_addr_lookup(&UIP_IP_BUF->destipaddr, 128, 0) == ifaddr){
    if(1)
    {
      /*   
       * we need to send a NA, we set the src, dest, flags. The rest is set
       * at the "create_na" label.
       */
         fprintf_P(debugStream, PSTR("uip_netif_addr_lookup: \r\n"));
	  
	  fprintf_P(debugStream, PSTR("ORG addr: \r\n"));
	 PRINT6ADDR(debugStream, &nicState.layer3.ipv6->destipaddr);
         PRINT6ADDR(debugStream, &nicState.layer3.ipv6->srcipaddr);
    
         
	 memcpy(&nicState.layer3.ipv6->destipaddr, &nicState.layer3.ipv6->srcipaddr, sizeof(uip_ip6addr_t));
	 memcpy(&nicState.layer3.ipv6->srcipaddr, &UIP_ND6_NS_BUF->tgtipaddr, sizeof(uip_ip6addr_t));
	 
	 fprintf_P(debugStream, PSTR("MOD addr: \r\n"));
	 PRINT6ADDR(debugStream, &nicState.layer3.ipv6->destipaddr);
         PRINT6ADDR(debugStream, &nicState.layer3.ipv6->srcipaddr);
    
	 
	 
      ///uip_ipaddr_copy(&UIP_IP_BUF->destipaddr, &UIP_IP_BUF->srcipaddr);
      ///uip_ipaddr_copy(&UIP_IP_BUF->srcipaddr, &UIP_ND6_NS_BUF->tgtipaddr);
      flags = UIP_ND6_NA_FLAG_SOLICITED | UIP_ND6_NA_FLAG_OVERRIDE;
      goto create_na;
    } else {
#if UIP_CONF_IPV6_CHECKS
      goto badpkt;
#endif /* UIP_CONF_IPV6_CHECKS */ 
    }    
  } else {
    goto discard;
  }
  

 create_na:
  /* 
   * Fill the part of the NA which is common to all NAs. If the NS contained
   * extension headers, we must set the target address properly
   */
  uip_ext_len = 0; 
  
#ifdef DEBUG_PRINT_NET_PACKET
    fprintf_P(debugStream, PSTR("NS packet before:\r\n"));    
    for (uint8_t i=0; i < packetLenght; i++)
    {
      if (i!=0 && i%16==0) fprintf_P(debugStream, PSTR("\r\n"));
      fprintf_P(debugStream, PSTR("%02x:"), nicState.layer2.buf[i]);
    }
    fprintf_P(debugStream, PSTR("\r\n"));
#endif
  
  
  /* IP header */
  //UIP_IP_BUF->vtc = 0x60;
  nicState.layer3.ipv6->vtc = 0x60;
  nicState.layer3.ipv6->tcflow = 0;
  nicState.layer3.ipv6->flow = 0;
  nicState.layer3.ipv6->len[0] = 0; /* length will not be more than 255 */
  nicState.layer3.ipv6->len[1] = UIP_ICMPH_LEN + UIP_ND6_NA_LEN + UIP_ND6_OPT_LLAO_LEN;
  nicState.layer3.ipv6->proto = UIP_PROTO_ICMP6;
  nicState.layer3.ipv6->ttl = UIP_ND6_HOP_LIMIT;

  /* ICMP header */
  UIP_ICMP_BUF->type = ICMP6_NA;
  UIP_ICMP_BUF->icode = 0;

  /* NA static part */
  UIP_ND6_NA_BUF->flagsreserved = flags;
  memcpy(&UIP_ND6_NA_BUF->tgtipaddr, &ifaddr->ipaddr, sizeof(uip_ipaddr_t)); /////////////////////

  /* NA option: TLLAO. note that length field is in unit of 8 bytes */
  packetLenght = UIP_IPH_LEN + UIP_ICMPH_LEN + UIP_ND6_NA_LEN + UIP_ND6_OPT_LLAO_LEN + ETH_HEADER_LEN;
  nd6_opt_llao = (struct uip_nd6_opt_llao *)&nicState.layer2.buf[uip_l2_l3_icmp_hdr_len + UIP_ND6_NA_LEN];
  nd6_opt_llao->type = UIP_ND6_OPT_TLLAO;
  nd6_opt_llao->len = UIP_ND6_OPT_LLAO_LEN >> 3;
  
 fprintf_P(debugStream, PSTR("&nicState.mac.addr %02x \n\r"), &nicState.mac.addr);
  
  memcpy(&(nd6_opt_llao->addr.addr), &nicState.mac.addr, ETH_HEADER_LEN);
  /* padding if needed */
  memset((void *)(&nd6_opt_llao->addr.addr) + UIP_LLADDR_LEN, 0, UIP_ND6_OPT_LLAO_LEN - 2 - UIP_LLADDR_LEN);

  /*ICMP checksum*/
  fprintf_P(debugStream, PSTR("nicState.layer4.icmp->icmpchksum  %04x\n\r"), UIP_ICMP_BUF->icmpchksum );
  UIP_ICMP_BUF->icmpchksum = 0;
  UIP_ICMP_BUF->icmpchksum = ~uip_icmp6chksum();   
  ///nicState.layer4.icmpv6->icmpchksum = 0;
  ///nicState.layer4.icmpv6->icmpchksum =  ~uip_icmp6chksum();
	      //netChecksum((uint8_t*)(&nicState.layer4.icmpv6 + 4), 28 );// htons(nicState.layer3.ip->len)-IP_HEADER_LEN);
  fprintf_P(debugStream, PSTR("nicState.layer4.icmp->icmpchksum  %04x\n\r"), UIP_ICMP_BUF->icmpchksum );
  //nicState.layer4.icmpv6->icmpchksum = 0x923b;

  #ifdef DEBUG_PRINT_NET_PACKET
    fprintf_P(debugStream, PSTR("NS packet AFTER:\r\n"));    
    for (uint8_t i=0; i < packetLenght; i++)
    {
      if (i!=0 && i%16==0) fprintf_P(debugStream, PSTR("\r\n"));
      fprintf_P(debugStream, PSTR("%02x:"), nicState.layer2.buf[i]);
    }
    fprintf_P(debugStream, PSTR("\r\n"));
#endif
  
  PRINTF2(debugStream,"Sending NA to\n\r");
  PRINT6ADDR(debugStream,&UIP_IP_BUF->destipaddr);
    PRINT6ADDR(debugStream, &nicState.layer3.ipv6->destipaddr);
  PRINTF2(debugStream,"from\n\r");
  PRINT6ADDR(debugStream,&UIP_IP_BUF->srcipaddr);
    PRINT6ADDR(debugStream, &nicState.layer3.ipv6->srcipaddr);
  PRINTF2(debugStream,"with target address\n\r");
  PRINT6ADDR(debugStream,&UIP_ND6_NA_BUF->tgtipaddr);
  return;

#if UIP_CONF_IPV6_CHECKS
 badpkt:
 PRINTF2(debugStream,"NS received is bad\n\r");
#endif /* UIP_CONF_IPV6_CHECKS */
  
 discard:
  packetLenght = 0;
  return;
  
}

