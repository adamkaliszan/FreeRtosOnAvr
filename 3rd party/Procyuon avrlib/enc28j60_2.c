00001 /*! \file enc28j60.c \brief Microchip ENC28J60 Ethernet Interface Driver. */
00002 //*****************************************************************************
00003 //
00004 // File Name    : 'enc28j60.c'
00005 // Title        : Microchip ENC28J60 Ethernet Interface Driver
00006 // Author       : Pascal Stang (c)2005
00007 // Created      : 9/22/2005
00008 // Revised      : 9/22/2005
00009 // Version      : 0.1
00010 // Target MCU   : Atmel AVR series
00011 // Editor Tabs  : 4
00012 //
00013 // Description  : This driver provides initialization and transmit/receive
00014 //  functions for the Microchip ENC28J60 10Mb Ethernet Controller and PHY.
00015 // This chip is novel in that it is a full MAC+PHY interface all in a 28-pin
00016 // chip, using an SPI interface to the host processor.
00017 //
00018 //*****************************************************************************
00019 
00020 #include "avr/io.h"
00021 
00022 #include "global.h"
00023 #include "timer.h"
00024 #include "rprintf.h"
00025 
00026 #include "enc28j60.h"
00027 
00028 #ifdef SPDR0
00029     #define SPDR    SPDR0
00030     #define SPCR    SPCR0
00031     #define SPSR    SPSR0
00032 
00033     #define SPIF    SPIF0
00034     #define MSTR    MSTR0
00035     #define CPOL    CPOL0
00036     #define DORD    DORD0
00037     #define SPR0    SPR00
00038     #define SPR1    SPR01
00039     #define SPI2X   SPI2X0
00040     #define SPE     SPE0
00041 #endif
00042 
00043 // include configuration
00044 #include "enc28j60conf.h"
00045 
00046 u08 Enc28j60Bank;
00047 u16 NextPacketPtr;
00048 
00049 void nicInit(void)
00050 {
00051     enc28j60Init();
00052 }
00053 
00054 void nicSend(unsigned int len, unsigned char* packet)
00055 {
00056     enc28j60PacketSend(len, packet);
00057 }
00058 
00059 unsigned int nicPoll(unsigned int maxlen, unsigned char* packet)
00060 {
00061     return enc28j60PacketReceive(maxlen, packet);
00062 }
00063 
00064 void nicGetMacAddress(u08* macaddr)
00065 {
00066     // read MAC address registers
00067     // NOTE: MAC address in ENC28J60 is byte-backward
00068     *macaddr++ = enc28j60Read(MAADR5);
00069     *macaddr++ = enc28j60Read(MAADR4);
00070     *macaddr++ = enc28j60Read(MAADR3);
00071     *macaddr++ = enc28j60Read(MAADR2);
00072     *macaddr++ = enc28j60Read(MAADR1);
00073     *macaddr++ = enc28j60Read(MAADR0);
00074 }
00075 
00076 void nicSetMacAddress(u08* macaddr)
00077 {
00078     // write MAC address
00079     // NOTE: MAC address in ENC28J60 is byte-backward
00080     enc28j60Write(MAADR5, *macaddr++);
00081     enc28j60Write(MAADR4, *macaddr++);
00082     enc28j60Write(MAADR3, *macaddr++);
00083     enc28j60Write(MAADR2, *macaddr++);
00084     enc28j60Write(MAADR1, *macaddr++);
00085     enc28j60Write(MAADR0, *macaddr++);
00086 }
00087 
00088 void nicRegDump(void)
00089 {
00090     enc28j60RegDump();
00091 }
00092 
00093 /*
00094 void ax88796SetupPorts(void)
00095 {
00096 #if NIC_CONNECTION == MEMORY_MAPPED
00097     // enable external SRAM interface - no wait states
00098     sbi(MCUCR, SRE);
00099 //  sbi(MCUCR, SRW10);
00100 //  sbi(XMCRA, SRW00);
00101 //  sbi(XMCRA, SRW01);
00102 //  sbi(XMCRA, SRW11);
00103 #else
00104     // set address port to output
00105     AX88796_ADDRESS_DDR = AX88796_ADDRESS_MASK;
00106     
00107     // set data port to input with pull-ups
00108     AX88796_DATA_DDR = 0x00;
00109     AX88796_DATA_PORT = 0xFF;
00110 
00111     // initialize the control port read and write pins to de-asserted
00112     sbi( AX88796_CONTROL_PORT, AX88796_CONTROL_READPIN );
00113     sbi( AX88796_CONTROL_PORT, AX88796_CONTROL_WRITEPIN );
00114     // set the read and write pins to output
00115     sbi( AX88796_CONTROL_DDR, AX88796_CONTROL_READPIN );
00116     sbi( AX88796_CONTROL_DDR, AX88796_CONTROL_WRITEPIN );
00117 #endif
00118     // set reset pin to output
00119     sbi( AX88796_RESET_DDR, AX88796_RESET_PIN );
00120 }
00121 */
00122 
00123 u08 enc28j60ReadOp(u08 op, u08 address)
00124 {
00125     u08 data;
00126    
00127     // assert CS
00128     ENC28J60_CONTROL_PORT &= ~(1<<ENC28J60_CONTROL_CS);
00129     
00130     // issue read command
00131     SPDR = op | (address & ADDR_MASK);
00132     while(!(SPSR & (1<<SPIF)));
00133     // read data
00134     SPDR = 0x00;
00135     while(!(SPSR & (1<<SPIF)));
00136     // do dummy read if needed
00137     if(address & 0x80)
00138     {
00139         SPDR = 0x00;
00140         while(!(inb(SPSR) & (1<<SPIF)));
00141     }
00142     data = SPDR;
00143     
00144     // release CS
00145     ENC28J60_CONTROL_PORT |= (1<<ENC28J60_CONTROL_CS);
00146 
00147     return data;
00148 }
00149 
00150 void enc28j60WriteOp(u08 op, u08 address, u08 data)
00151 {
00152     // assert CS
00153     ENC28J60_CONTROL_PORT &= ~(1<<ENC28J60_CONTROL_CS);
00154 
00155     // issue write command
00156     SPDR = op | (address & ADDR_MASK);
00157     while(!(SPSR & (1<<SPIF)));
00158     // write data
00159     SPDR = data;
00160     while(!(SPSR & (1<<SPIF)));
00161 
00162     // release CS
00163     ENC28J60_CONTROL_PORT |= (1<<ENC28J60_CONTROL_CS);
00164 }
00165 
00166 void enc28j60ReadBuffer(u16 len, u08* data)
00167 {
00168     // assert CS
00169     ENC28J60_CONTROL_PORT &= ~(1<<ENC28J60_CONTROL_CS);
00170     
00171     // issue read command
00172     SPDR = ENC28J60_READ_BUF_MEM;
00173     while(!(SPSR & (1<<SPIF)));
00174     while(len--)
00175     {
00176         // read data
00177         SPDR = 0x00;
00178         while(!(SPSR & (1<<SPIF)));
00179         *data++ = SPDR;
00180     }   
00181     // release CS
00182     ENC28J60_CONTROL_PORT |= (1<<ENC28J60_CONTROL_CS);
00183 }
00184 
00185 void enc28j60WriteBuffer(u16 len, u08* data)
00186 {
00187     // assert CS
00188     ENC28J60_CONTROL_PORT &= ~(1<<ENC28J60_CONTROL_CS);
00189     
00190     // issue write command
00191     SPDR = ENC28J60_WRITE_BUF_MEM;
00192     while(!(SPSR & (1<<SPIF)));
00193     while(len--)
00194     {
00195         // write data
00196         SPDR = *data++;
00197         while(!(SPSR & (1<<SPIF)));
00198     }   
00199     // release CS
00200     ENC28J60_CONTROL_PORT |= (1<<ENC28J60_CONTROL_CS);
00201 }
00202 
00203 void enc28j60SetBank(u08 address)
00204 {
00205     // set the bank (if needed)
00206     if((address & BANK_MASK) != Enc28j60Bank)
00207     {
00208         // set the bank
00209         enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
00210         enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);
00211         Enc28j60Bank = (address & BANK_MASK);
00212     }
00213 }
00214 
00215 u08 enc28j60Read(u08 address)
00216 {
00217     // set the bank
00218     enc28j60SetBank(address);
00219     // do the read
00220     return enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);
00221 }
00222 
00223 void enc28j60Write(u08 address, u08 data)
00224 {
00225     // set the bank
00226     enc28j60SetBank(address);
00227     // do the write
00228     enc28j60WriteOp(ENC28J60_WRITE_CTRL_REG, address, data);
00229 }
00230 
00231 u16 enc28j60PhyRead(u08 address)
00232 {
00233     u16 data;
00234 
00235     // Set the right address and start the register read operation
00236     enc28j60Write(MIREGADR, address);
00237     enc28j60Write(MICMD, MICMD_MIIRD);
00238 
00239     // wait until the PHY read completes
00240     while(enc28j60Read(MISTAT) & MISTAT_BUSY);
00241 
00242     // quit reading
00243     enc28j60Write(MICMD, 0x00);
00244     
00245     // get data value
00246     data  = enc28j60Read(MIRDL);
00247     data |= enc28j60Read(MIRDH);
00248     // return the data
00249     return data;
00250 }
00251 
00252 void enc28j60PhyWrite(u08 address, u16 data)
00253 {
00254     // set the PHY register address
00255     enc28j60Write(MIREGADR, address);
00256     
00257     // write the PHY data
00258     enc28j60Write(MIWRL, data); 
00259     enc28j60Write(MIWRH, data>>8);
00260 
00261     // wait until the PHY write completes
00262     while(enc28j60Read(MISTAT) & MISTAT_BUSY);
00263 }
00264 
00265 void enc28j60Init(void)
00266 {
00267     // initialize I/O
00268     sbi(ENC28J60_CONTROL_DDR, ENC28J60_CONTROL_CS);
00269     sbi(ENC28J60_CONTROL_PORT, ENC28J60_CONTROL_CS);
00270 
00271     // setup SPI I/O pins
00272     sbi(ENC28J60_SPI_PORT, ENC28J60_SPI_SCK);   // set SCK hi
00273     sbi(ENC28J60_SPI_DDR, ENC28J60_SPI_SCK);    // set SCK as output
00274     cbi(ENC28J60_SPI_DDR, ENC28J60_SPI_MISO);   // set MISO as input
00275     sbi(ENC28J60_SPI_DDR, ENC28J60_SPI_MOSI);   // set MOSI as output
00276     sbi(ENC28J60_SPI_DDR, ENC28J60_SPI_SS);     // SS must be output for Master mode to work
00277     // initialize SPI interface
00278     // master mode
00279     sbi(SPCR, MSTR);
00280     // select clock phase positive-going in middle of data
00281     cbi(SPCR, CPOL);
00282     // Data order MSB first
00283     cbi(SPCR,DORD);
00284     // switch to f/4 2X = f/2 bitrate
00285     cbi(SPCR, SPR0);
00286     cbi(SPCR, SPR1);
00287     sbi(SPSR, SPI2X);
00288     // enable SPI
00289     sbi(SPCR, SPE);
00290 
00291     // perform system reset
00292     enc28j60WriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
00293     // check CLKRDY bit to see if reset is complete
00294     delay_us(50);
00295     while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));
00296 
00297     // do bank 0 stuff
00298     // initialize receive buffer
00299     // 16-bit transfers, must write low byte first
00300     // set receive buffer start address
00301     NextPacketPtr = RXSTART_INIT;
00302     enc28j60Write(ERXSTL, RXSTART_INIT&0xFF);
00303     enc28j60Write(ERXSTH, RXSTART_INIT>>8);
00304     // set receive pointer address
00305     enc28j60Write(ERXRDPTL, RXSTART_INIT&0xFF);
00306     enc28j60Write(ERXRDPTH, RXSTART_INIT>>8);
00307     // set receive buffer end
00308     // ERXND defaults to 0x1FFF (end of ram)
00309     enc28j60Write(ERXNDL, RXSTOP_INIT&0xFF);
00310     enc28j60Write(ERXNDH, RXSTOP_INIT>>8);
00311     // set transmit buffer start
00312     // ETXST defaults to 0x0000 (beginnging of ram)
00313     enc28j60Write(ETXSTL, TXSTART_INIT&0xFF);
00314     enc28j60Write(ETXSTH, TXSTART_INIT>>8);
00315 
00316     // do bank 2 stuff
00317     // enable MAC receive
00318     enc28j60Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
00319     // bring MAC out of reset
00320     enc28j60Write(MACON2, 0x00);
00321     // enable automatic padding and CRC operations
00322     enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
00323 //  enc28j60Write(MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
00324     // set inter-frame gap (non-back-to-back)
00325     enc28j60Write(MAIPGL, 0x12);
00326     enc28j60Write(MAIPGH, 0x0C);
00327     // set inter-frame gap (back-to-back)
00328     enc28j60Write(MABBIPG, 0x12);
00329     // Set the maximum packet size which the controller will accept
00330     enc28j60Write(MAMXFLL, MAX_FRAMELEN&0xFF);  
00331     enc28j60Write(MAMXFLH, MAX_FRAMELEN>>8);
00332 
00333     // do bank 3 stuff
00334     // write MAC address
00335     // NOTE: MAC address in ENC28J60 is byte-backward
00336     enc28j60Write(MAADR5, ENC28J60_MAC0);
00337     enc28j60Write(MAADR4, ENC28J60_MAC1);
00338     enc28j60Write(MAADR3, ENC28J60_MAC2);
00339     enc28j60Write(MAADR2, ENC28J60_MAC3);
00340     enc28j60Write(MAADR1, ENC28J60_MAC4);
00341     enc28j60Write(MAADR0, ENC28J60_MAC5);
00342 
00343     // no loopback of transmitted frames
00344     enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);
00345 
00346     // switch to bank 0
00347     enc28j60SetBank(ECON1);
00348     // enable interrutps
00349     enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
00350     // enable packet reception
00351     enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
00352 /*
00353     enc28j60PhyWrite(PHLCON, 0x0AA2);
00354 
00355     // setup duplex ----------------------
00356 
00357     // Disable receive logic and abort any packets currently being transmitted
00358     enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS|ECON1_RXEN);
00359     
00360     {
00361         u16 temp;
00362         // Set the PHY to the proper duplex mode
00363         temp = enc28j60PhyRead(PHCON1);
00364         temp &= ~PHCON1_PDPXMD;
00365         enc28j60PhyWrite(PHCON1, temp);
00366         // Set the MAC to the proper duplex mode
00367         temp = enc28j60Read(MACON3);
00368         temp &= ~MACON3_FULDPX;
00369         enc28j60Write(MACON3, temp);
00370     }
00371 
00372     // Set the back-to-back inter-packet gap time to IEEE specified 
00373     // requirements.  The meaning of the MABBIPG value changes with the duplex
00374     // state, so it must be updated in this function.
00375     // In full duplex, 0x15 represents 9.6us; 0x12 is 9.6us in half duplex
00376     //enc28j60Write(MABBIPG, DuplexState ? 0x15 : 0x12);    
00377     
00378     // Reenable receive logic
00379     enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
00380 
00381     // setup duplex ----------------------
00382 */
00383 }
00384 
00385 void enc28j60PacketSend(unsigned int len, unsigned char* packet)
00386 {
00387     // Set the write pointer to start of transmit buffer area
00388     enc28j60Write(EWRPTL, TXSTART_INIT);
00389     enc28j60Write(EWRPTH, TXSTART_INIT>>8);
00390     // Set the TXND pointer to correspond to the packet size given
00391     enc28j60Write(ETXNDL, (TXSTART_INIT+len));
00392     enc28j60Write(ETXNDH, (TXSTART_INIT+len)>>8);
00393 
00394     // write per-packet control byte
00395     enc28j60WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
00396 
00397     // copy the packet into the transmit buffer
00398     enc28j60WriteBuffer(len, packet);
00399     
00400     // send the contents of the transmit buffer onto the network
00401     enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
00402 }
00403 
00404 unsigned int enc28j60PacketReceive(unsigned int maxlen, unsigned char* packet)
00405 {
00406     u16 rxstat;
00407     u16 len;
00408 
00409     // check if a packet has been received and buffered
00410 //  if( !(enc28j60Read(EIR) & EIR_PKTIF) )
00411     if( !enc28j60Read(EPKTCNT) )
00412         return 0;
00413     
00414     // Make absolutely certain that any previous packet was discarded   
00415     //if( WasDiscarded == FALSE)
00416     //  MACDiscardRx();
00417 
00418     // Set the read pointer to the start of the received packet
00419     enc28j60Write(ERDPTL, (NextPacketPtr));
00420     enc28j60Write(ERDPTH, (NextPacketPtr)>>8);
00421     // read the next packet pointer
00422     NextPacketPtr  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
00423     NextPacketPtr |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
00424     // read the packet length
00425     len  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
00426     len |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
00427     // read the receive status
00428     rxstat  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
00429     rxstat |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
00430 
00431     // limit retrieve length
00432     // (we reduce the MAC-reported length by 4 to remove the CRC)
00433     len = MIN(len, maxlen);
00434 
00435     // copy the packet from the receive buffer
00436     enc28j60ReadBuffer(len, packet);
00437 
00438     // Move the RX read pointer to the start of the next received packet
00439     // This frees the memory we just read out
00440     enc28j60Write(ERXRDPTL, (NextPacketPtr));
00441     enc28j60Write(ERXRDPTH, (NextPacketPtr)>>8);
00442 
00443     // decrement the packet counter indicate we are done with this packet
00444     enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
00445 
00446     return len;
00447 }
00448 
00449 void enc28j60ReceiveOverflowRecover(void)
00450 {
00451     // receive buffer overflow handling procedure
00452 
00453     // recovery completed
00454 }
00455 
00456 void enc28j60RegDump(void)
00457 {
00458 //  unsigned char macaddr[6];
00459 //  result = ax88796Read(TR);
00460     
00461 //  rprintf("Media State: ");
00462 //  if(!(result & AUTOD))
00463 //      rprintf("Autonegotiation\r\n");
00464 //  else if(result & RST_B)
00465 //      rprintf("PHY in Reset   \r\n");
00466 //  else if(!(result & RST_10B))
00467 //      rprintf("10BASE-T       \r\n");
00468 //  else if(!(result & RST_TXB))
00469 //      rprintf("100BASE-T      \r\n");
00470                 
00471     rprintf("RevID: 0x%x\r\n", enc28j60Read(EREVID));
00472 
00473     rprintfProgStrM("Cntrl: ECON1 ECON2 ESTAT  EIR  EIE\r\n");
00474     rprintfProgStrM("         ");
00475     rprintfu08(enc28j60Read(ECON1));
00476     rprintfProgStrM("    ");
00477     rprintfu08(enc28j60Read(ECON2));
00478     rprintfProgStrM("    ");
00479     rprintfu08(enc28j60Read(ESTAT));
00480     rprintfProgStrM("    ");
00481     rprintfu08(enc28j60Read(EIR));
00482     rprintfProgStrM("   ");
00483     rprintfu08(enc28j60Read(EIE));
00484     rprintfCRLF();
00485 
00486     rprintfProgStrM("MAC  : MACON1  MACON2  MACON3  MACON4  MAC-Address\r\n");
00487     rprintfProgStrM("        0x");
00488     rprintfu08(enc28j60Read(MACON1));
00489     rprintfProgStrM("    0x");
00490     rprintfu08(enc28j60Read(MACON2));
00491     rprintfProgStrM("    0x");
00492     rprintfu08(enc28j60Read(MACON3));
00493     rprintfProgStrM("    0x");
00494     rprintfu08(enc28j60Read(MACON4));
00495     rprintfProgStrM("   ");
00496     rprintfu08(enc28j60Read(MAADR5));
00497     rprintfu08(enc28j60Read(MAADR4));
00498     rprintfu08(enc28j60Read(MAADR3));
00499     rprintfu08(enc28j60Read(MAADR2));
00500     rprintfu08(enc28j60Read(MAADR1));
00501     rprintfu08(enc28j60Read(MAADR0));
00502     rprintfCRLF();
00503 
00504     rprintfProgStrM("Rx   : ERXST  ERXND  ERXWRPT ERXRDPT ERXFCON EPKTCNT MAMXFL\r\n");
00505     rprintfProgStrM("       0x");
00506     rprintfu08(enc28j60Read(ERXSTH));
00507     rprintfu08(enc28j60Read(ERXSTL));
00508     rprintfProgStrM(" 0x");
00509     rprintfu08(enc28j60Read(ERXNDH));
00510     rprintfu08(enc28j60Read(ERXNDL));
00511     rprintfProgStrM("  0x");
00512     rprintfu08(enc28j60Read(ERXWRPTH));
00513     rprintfu08(enc28j60Read(ERXWRPTL));
00514     rprintfProgStrM("  0x");
00515     rprintfu08(enc28j60Read(ERXRDPTH));
00516     rprintfu08(enc28j60Read(ERXRDPTL));
00517     rprintfProgStrM("   0x");
00518     rprintfu08(enc28j60Read(ERXFCON));
00519     rprintfProgStrM("    0x");
00520     rprintfu08(enc28j60Read(EPKTCNT));
00521     rprintfProgStrM("  0x");
00522     rprintfu08(enc28j60Read(MAMXFLH));
00523     rprintfu08(enc28j60Read(MAMXFLL));
00524     rprintfCRLF();
00525 
00526     rprintfProgStrM("Tx   : ETXST  ETXND  MACLCON1 MACLCON2 MAPHSUP\r\n");
00527     rprintfProgStrM("       0x");
00528     rprintfu08(enc28j60Read(ETXSTH));
00529     rprintfu08(enc28j60Read(ETXSTL));
00530     rprintfProgStrM(" 0x");
00531     rprintfu08(enc28j60Read(ETXNDH));
00532     rprintfu08(enc28j60Read(ETXNDL));
00533     rprintfProgStrM("   0x");
00534     rprintfu08(enc28j60Read(MACLCON1));
00535     rprintfProgStrM("     0x");
00536     rprintfu08(enc28j60Read(MACLCON2));
00537     rprintfProgStrM("     0x");
00538     rprintfu08(enc28j60Read(MAPHSUP));
00539     rprintfCRLF();
00540 
00541     delay_ms(25);
00542 }


