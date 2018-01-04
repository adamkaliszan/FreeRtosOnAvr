/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * http://www.gnu.org/licenses/gpl.html
 *
 * Based on the enc28j60.c file from the AVRlib library by Pascal Stang
 * For AVRlib See http://www.procyonengineering.com/
 * Used with explicit permission of Pascal Stang.
 *
 * Title: Microchip ENC28J60 Ethernet Interface Driver
 * Chip type           : ATMEGA88 with ENC28J60
 *********************************************/

#include "enc28j60.h"


static uint8_t Enc28j60Bank;
static uint16_t gNextPacketPtr;


/**
 * Initialize enc28j60
 * @param *macaddr - pointer to the mac address (6 bytes)
 */
static void     enc28j60Init(uint8_t* macaddr);

/**
 * Read register value (without changing the bank)
 * @param op      - operation type
 * @param address - register address
 * @return register value
 */
static uint8_t  enc28j60ReadOp (uint8_t op, uint8_t address);
static void     enc28j60WriteOp(uint8_t op, uint8_t address, uint8_t data);
static void     enc28j60SetBank(uint8_t address);
static void     enc28j60ReadBuffer(uint16_t len, uint8_t* data);
static void     enc28j60WriteBuffer(uint16_t len, uint8_t* data);

/**
 * Reads Enc28j60 control register
 * @param address - register address. Banks are changed automatically
 * @return control register value
 */
static uint8_t  enc28j60Read(uint8_t address);

/**
 * Writes Enc28j60 control register
 * @param address - register address. Banks are changed automatically
 * @param data    - control register value to be writen
 */
static void     enc28j60Write(uint8_t address, uint8_t data);

/**
 * Reads Enc28j60 phy register
 * @param address - register address. Banks are changed automatically
 * @return phy register value
 */
static uint16_t enc28j60PhyReadH(uint8_t address);

/**
 * Writes Enc28j60 phy register
 * @param address - register address. Banks are changed automatically
 * @param data    - phy register value to be writen
 */
static void     enc28j60PhyWrite(uint8_t address, uint16_t data);


//void     enc28j60BufferSend(uint16_t len, roundBuffer* buffer);

uint8_t  enc28j60hasRxPkt(void);
uint8_t  enc28j60getrev(void);
uint8_t  enc28j60linkup(void);

void nicMacInit(void)
{
    vTaskDelay          (5);
    enc28j60Init        (nicState.mac.addr);
//  enc28j60clkout      (2);     // change clkout from 6.25MHz to 12.5MHz
    vTaskDelay          (5);
    enc28j60PhyWrite    (PHLCON, 0x476);
    vTaskDelay          (2);
}

uint8_t enc28j60ReadOp(uint8_t op, uint8_t address)
{
  uint8_t result;
  //spiTake();
  spiEnableEnc28j60();

  // issue read command
  spiSendENC(op | (address & ADDR_MASK));

  // read data
  result = spiSendENC(0x00);

  // do dummy read if needed (for mac and mii, see datasheet page 29)
  if(address & 0x80)
  {
    result = spiSendENC(0x00);
  }

  spiDisableEnc28j60();
 // spiGive();
  return result;
}

void enc28j60WriteOp(uint8_t op, uint8_t address, uint8_t data)
{
 // spiTake();
  spiEnableEnc28j60();
  // issue write command
  //spiSend(op | (address & ADDR_MASK));
  spiSendENC(op | (address & ADDR_MASK));
  spiSendENC(data);
  spiDisableEnc28j60();
 // spiGive();
}

void enc28j60ReadBuffer(uint16_t len, uint8_t* data)
{
 // spiTake();
  spiEnableEnc28j60();
  spiSendENC(ENC28J60_READ_BUF_MEM);
  while(len)
  {
    len--;
    *data = spiSendENC(0x00);
    data++;
  }
  *data='\0';
  spiDisableEnc28j60();
 // spiGive();
}

void enc28j60WriteBuffer(uint16_t len, uint8_t* data)
{
  //spiTake();
  spiEnableEnc28j60();
  // issue write command
  //spiSend(ENC28J60_WRITE_BUF_MEM);      //
  spiSendENC(ENC28J60_WRITE_BUF_MEM);
  while(len)
  {
    len--;
    spiSendENC(*data);       // write data
    data++;
  }
  spiDisableEnc28j60();
  //spiGive();
}

// void enc28j60WriteRoundBuffer(uint8_t len, roundBuffer *buffer)
// {
//   spiTake();
//   spiEnableEnc28j60();
//   // issue write command
//   //spiSend(ENC28J60_WRITE_BUF_MEM);      //
//   spiSend(ENC28J60_WRITE_BUF_MEM);
//   uint8_t data;
//   while(len)
//   {
//     len--;
//     data = *buffer->readIdx.ptr16;
//     buffer->readIdx.ptr.L++;
//
//     spiSend(*data);       // write data
//   }
//   spiDisableEnc28j60();
//   spiGive();
// }


void enc28j60SetBank(uint8_t address)
{
  // set the bank (if needed)
  if((address & BANK_MASK) != Enc28j60Bank)
  {
    // set the bank
    enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
    enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);
    Enc28j60Bank = (address & BANK_MASK);
  }
}

uint8_t enc28j60Read(uint8_t address)
{
  // set the bank
  enc28j60SetBank(address);
  // do the read
  uint8_t result = enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);
  return result;
}

// read upper 8 bits
uint16_t enc28j60PhyReadH(uint8_t address)
{
  // Set the right address and start the register read operation
  enc28j60Write(MIREGADR, address);
  enc28j60Write(MICMD, MICMD_MIIRD);

  vTaskDelay(0);

  // wait until the PHY read completes
  while(enc28j60Read(MISTAT) & MISTAT_BUSY)
   vTaskDelay ( 0 ); //FIXME być może tutaj następuje zawieszenie

  // reset reading bit
  enc28j60Write(MICMD, 0x00);

  return (enc28j60Read(MIRDH));
}

void enc28j60Write(uint8_t address, uint8_t data)
{
  // set the bank
  enc28j60SetBank(address);
  // do the write
  enc28j60WriteOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

void enc28j60PhyWrite(uint8_t address, uint16_t data)
{
  // set the PHY register address
  enc28j60Write(MIREGADR, address);
  // write the PHY data
  enc28j60Write(MIWRL, data);
  enc28j60Write(MIWRH, data>>8);
  // wait until the PHY write completes

  while(enc28j60Read(MISTAT) & MISTAT_BUSY)
  {
    vTaskDelay ( 0 );         //FIXME być może tutaj następuje zakleszczenie
  }
}

void enc28j60Init(uint8_t* macaddr)
{
  // perform system reset

  //ENC28j60 reset is on PE2 TODO add in hardware.c macros for that.
  ENC_RST_ON;   // PORTE &= ~0x04;
  vTaskDelay(5); // 50ms
  ENC_RST_OFF;  //PORTE |= 0x04;
  vTaskDelay(5); // 50ms

  // check CLKRDY bit to see if reset is complete
  // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
  //while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));
  // do bank 0 stuff
  // initialize receive buffer
  // 16-bit transfers, must write low byte first
  // set receive buffer start address
  gNextPacketPtr = RXSTART_INIT;
  // Rx start
  enc28j60Write(ERXSTL, RXSTART_INIT&0xFF);
  enc28j60Write(ERXSTH, RXSTART_INIT>>8);
  // set receive pointer address
  enc28j60Write(ERXRDPTL, RXSTART_INIT&0xFF);
  enc28j60Write(ERXRDPTH, RXSTART_INIT>>8);
  // RX end
  enc28j60Write(ERXNDL, RXSTOP_INIT&0xFF);
  enc28j60Write(ERXNDH, RXSTOP_INIT>>8);
  // TX start
  enc28j60Write(ETXSTL, TXSTART_INIT&0xFF);
  enc28j60Write(ETXSTH, TXSTART_INIT>>8);
  // TX end
  enc28j60Write(ETXNDL, TXSTOP_INIT&0xFF);
  enc28j60Write(ETXNDH, TXSTOP_INIT>>8);

  // do bank 1 stuff, packet filter:
  // For broadcast packets we allow only ARP packtets
  // All other packets should be unicast only for our mac (MAADR)
  //
  // The pattern to match on is therefore
  // Type     ETH.DST
  // ARP      BROADCAST
  // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
  // in binary these poitions are:11 0000 0011 1111
  // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
  //enc28j60Write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);  //Bez wejsca dla broadcastu (jak opis powyzej, wpusci tylko arp - na zasadzie checksumy zgodnej z pakietem)
  enc28j60Write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN|ERXFCON_MCEN); //Z wejsciem dla calego broadcastu
  enc28j60Write(EPMM0, 0x3f);
  enc28j60Write(EPMM1, 0x30);
  enc28j60Write(EPMCSL, 0xf9);
  enc28j60Write(EPMCSH, 0xf7);
  //
  //
  // do bank 2 stuff
  // enable MAC receive
  enc28j60Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
  // bring MAC out of reset
  enc28j60Write(MACON2, 0x00);
  // enable automatic padding to 60bytes and CRC operations
  enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
  // set inter-frame gap (non-back-to-back)
  enc28j60Write(MAIPGL, 0x12);
  enc28j60Write(MAIPGH, 0x0C);
  // set inter-frame gap (back-to-back)
  enc28j60Write(MABBIPG, 0x12);
  // Set the maximum packet size which the controller will accept
  // Do not send packets longer than MAX_FRAMELEN:
  enc28j60Write(MAMXFLL, MAX_FRAMELEN&0xFF);
  enc28j60Write(MAMXFLH, MAX_FRAMELEN>>8);

  // do bank 3 stuff
  // write MAC address

  nicSetMacAddress(macaddr);

  // no loopback of transmitted frames
  enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);

  enc28j60SetBank(ECON1);                             // switch to bank 0
  // enable interrutps
  enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
  // enable packet reception
  enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
}

// read the revision of the chip:
uint8_t enc28j60getrev(void)
{
  return(enc28j60Read(EREVID));
}

// link status
uint8_t enc28j60linkup(void)
{
  // bit 10 (= bit 3 in upper reg)
  return(enc28j60PhyReadH(PHSTAT2) && 4);
}

void nicSend(uint16_t len)
{
  // Check no transmit in progress
  while (enc28j60ReadOp(ENC28J60_READ_CTRL_REG, ECON1) & ECON1_TXRTS)
  {
    // Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
    if( (enc28j60Read(EIR) & EIR_TXERIF) )
    {
      enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
      enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
    }
    vTaskDelay ( 0 ); //FIXME być może tutaj następuje zakleszczenie
  }
  // Set the write pointer to start of transmit buffer area
  enc28j60Write(EWRPTL, TXSTART_INIT&0xFF);
  enc28j60Write(EWRPTH, TXSTART_INIT>>8);
  // Set the TXND pointer to correspond to the packet size given
  enc28j60Write(ETXNDL, (TXSTART_INIT+len)&0xFF);
  enc28j60Write(ETXNDH, (TXSTART_INIT+len)>>8);
  // write per-packet control byte (0x00 means use macon3 settings)
  enc28j60WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
  // copy the packet into the transmit buffer
  enc28j60WriteBuffer(len, nicState.layer2.buf);
  // send the contents of the transmit buffer onto the network
  enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}

// just probe if there might be a packet
uint8_t enc28j60hasRxPkt(void)
{
  if( enc28j60Read(EPKTCNT) ==0 )
  {
    return(0);
  }
  return(1);
}

uint16_t nicPoll(void)
{
  uint16_t rxstat;
  uint16_t len;
  // check if a packet has been received and buffered
  //if( !(enc28j60Read(EIR) & EIR_PKTIF) ){
  // The above does not work. See Rev. B4 Silicon Errata point 6.
  if( enc28j60Read(EPKTCNT) == 0 )
  {
    return(0);
  }

  // Set the read pointer to the start of the received packet
  enc28j60Write(ERDPTL, (gNextPacketPtr &0xFF));
  enc28j60Write(ERDPTH, (gNextPacketPtr)>>8);
  // read the next packet pointer
  gNextPacketPtr  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
  gNextPacketPtr |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
  // read the packet length (see datasheet page 43)
  len  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
  len |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
  len-=4; //remove the CRC count
  // read the receive status (see datasheet page 43)
  rxstat  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
  rxstat |= ((uint16_t)enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0))<<8;
  // limit retrieve length
  if (len> nicState.bufferSize -1)
  {
    len= nicState.bufferSize-1;
  }
  // check CRC and symbol errors (see datasheet page 44, table 7-3):
  // The ERXFCON.CRCEN is set by default. Normally we should not
  // need to check this.

  if ((rxstat & 0x80)==0)
  {
    // invalid
    len=0;
  }
  else
  {
    // copy the packet from the receive buffer
    enc28j60ReadBuffer(len, nicState.layer2.buf);
  }
  // Move the RX read pointer to the start of the next received packet
  // This frees the memory we just read out
  enc28j60Write(ERXRDPTL, (gNextPacketPtr &0xFF));
  enc28j60Write(ERXRDPTH, (gNextPacketPtr)>>8);

#if RXSTART_INIT > 0
  // Move the RX read pointer to the start of the next received packet
  // This frees the memory we just read out.
  // However, compensate for the errata point 13, rev B4: enver write an even address!
  //FIXME remove this warning
  if ((gNextPacketPtr - 1 < RXSTART_INIT) || (gNextPacketPtr -1 > RXSTOP_INIT))
  {
    enc28j60Write(ERXRDPTL, (RXSTOP_INIT)&0xFF);
    enc28j60Write(ERXRDPTH, (RXSTOP_INIT)>>8);
  }
  else
  {
#endif
    enc28j60Write(ERXRDPTL, (gNextPacketPtr-1)&0xFF);
    enc28j60Write(ERXRDPTH, (gNextPacketPtr-1)>>8);
#if RXSTART_INIT > 0
  }
#endif
  // decrement the packet counter indicate we are done with this packet
  enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
  return(len);
}

void spiEnableEnc28j60(void)  {};
void spiDisableEnc28j60(void) {};

void     nicSetMacAddress(uint8_t* macaddr)
{
//NOTE: MAC address in ENC28J60 is byte-backward
  enc28j60Write(MAADR5, macaddr[0]);
  enc28j60Write(MAADR4, macaddr[1]);
  enc28j60Write(MAADR3, macaddr[2]);
  enc28j60Write(MAADR2, macaddr[3]);
  enc28j60Write(MAADR1, macaddr[4]);
  enc28j60Write(MAADR0, macaddr[5]);
  //strncpy((void *)(nicState.mac.addr), (void *)(macaddr), 6);
}

void     nicGetMacAddress(uint8_t* macaddr)
{
  macaddr[5] = enc28j60Read(MAADR0);
  macaddr[4] = enc28j60Read(MAADR1);
  macaddr[3] = enc28j60Read(MAADR2);
  macaddr[2] = enc28j60Read(MAADR3);
  macaddr[1] = enc28j60Read(MAADR4);
  macaddr[0] = enc28j60Read(MAADR5);
  //strncpy((void *)(nicState.mac.addr), (void *)(macaddr), 6);
}

void     nicRegDump(FILE *stream)
{
  uint8_t temp;
  fprintf_P(stream, PSTR("ENC28j60 stan rejestrow:\r\n"));

  temp = enc28j60Read(MAADR0); fprintf_P(stream, PSTR("\tMAADR0 0x%x\r\n"), temp);
  temp = enc28j60Read(MAADR1); fprintf_P(stream, PSTR("\tMAADR1 0x%x\r\n"), temp);
  temp = enc28j60Read(MAADR2); fprintf_P(stream, PSTR("\tMAADR2 0x%x\r\n"), temp);
  temp = enc28j60Read(MAADR3); fprintf_P(stream, PSTR("\tMAADR3 0x%x\r\n"), temp);
  temp = enc28j60Read(MAADR4); fprintf_P(stream, PSTR("\tMAADR4 0x%x\r\n"), temp);
  temp = enc28j60Read(MAADR5); fprintf_P(stream, PSTR("\tMAADR5 0x%x\r\n"), temp);
}

