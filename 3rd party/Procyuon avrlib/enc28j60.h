#ifndef _ENC28J60
#define _ENC28j60

#define 	nop()   asm volatile ("nop")
#define 	ADDR_MASK   0x1F
#define 	BANK_MASK   0x60
#define 	SPRD_MASK   0x80
#define 	EIE   0x1B
#define 	EIR   0x1C
#define 	ESTAT   0x1D
#define 	ECON2   0x1E
#define 	ECON1   0x1F
#define 	ERDPTL   (0x00|0x00)
#define 	ERDPTH   (0x01|0x00)
#define 	EWRPTL   (0x02|0x00)
#define 	EWRPTH   (0x03|0x00)
#define 	ETXSTL   (0x04|0x00)
#define 	ETXSTH   (0x05|0x00)
#define 	ETXNDL   (0x06|0x00)
#define 	ETXNDH   (0x07|0x00)
#define 	ERXSTL   (0x08|0x00)
#define 	ERXSTH   (0x09|0x00)
#define 	ERXNDL   (0x0A|0x00)
#define 	ERXNDH   (0x0B|0x00)
#define 	ERXRDPTL   (0x0C|0x00)
#define 	ERXRDPTH   (0x0D|0x00)
#define 	ERXWRPTL   (0x0E|0x00)
#define 	ERXWRPTH   (0x0F|0x00)
#define 	EDMASTL   (0x10|0x00)
#define 	EDMASTH   (0x11|0x00)
#define 	EDMANDL   (0x12|0x00)
#define 	EDMANDH   (0x13|0x00)
#define 	EDMADSTL   (0x14|0x00)
#define 	EDMADSTH   (0x15|0x00)
#define 	EDMACSL   (0x16|0x00)
#define 	EDMACSH   (0x17|0x00)
#define 	EHT0   (0x00|0x20)
#define 	EHT1   (0x01|0x20)
#define 	EHT2   (0x02|0x20)
#define 	EHT3   (0x03|0x20)
#define 	EHT4   (0x04|0x20)
#define 	EHT5   (0x05|0x20)
#define 	EHT6   (0x06|0x20)
#define 	EHT7   (0x07|0x20)
#define 	EPMM0   (0x08|0x20)
#define 	EPMM1   (0x09|0x20)
#define 	EPMM2   (0x0A|0x20)
#define 	EPMM3   (0x0B|0x20)
#define 	EPMM4   (0x0C|0x20)
#define 	EPMM5   (0x0D|0x20)
#define 	EPMM6   (0x0E|0x20)
#define 	EPMM7   (0x0F|0x20)
#define 	EPMCSL   (0x10|0x20)
#define 	EPMCSH   (0x11|0x20)
#define 	EPMOL   (0x14|0x20)
#define 	EPMOH   (0x15|0x20)
#define 	EWOLIE   (0x16|0x20)
#define 	EWOLIR   (0x17|0x20)
#define 	ERXFCON   (0x18|0x20)
#define 	EPKTCNT   (0x19|0x20)
#define 	MACON1   (0x00|0x40|0x80)
#define 	MACON2   (0x01|0x40|0x80)
#define 	MACON3   (0x02|0x40|0x80)
#define 	MACON4   (0x03|0x40|0x80)
#define 	MABBIPG   (0x04|0x40|0x80)
#define 	MAIPGL   (0x06|0x40|0x80)
#define 	MAIPGH   (0x07|0x40|0x80)
#define 	MACLCON1   (0x08|0x40|0x80)
#define 	MACLCON2   (0x09|0x40|0x80)
#define 	MAMXFLL   (0x0A|0x40|0x80)
#define 	MAMXFLH   (0x0B|0x40|0x80)
#define 	MAPHSUP   (0x0D|0x40|0x80)
#define 	MICON   (0x11|0x40|0x80)
#define 	MICMD   (0x12|0x40|0x80)
#define 	MIREGADR   (0x14|0x40|0x80)
#define 	MIWRL   (0x16|0x40|0x80)
#define 	MIWRH   (0x17|0x40|0x80)
#define 	MIRDL   (0x18|0x40|0x80)
#define 	MIRDH   (0x19|0x40|0x80)
#define 	MAADR1   (0x00|0x60|0x80)
#define 	MAADR0   (0x01|0x60|0x80)
#define 	MAADR3   (0x02|0x60|0x80)
#define 	MAADR2   (0x03|0x60|0x80)
#define 	MAADR5   (0x04|0x60|0x80)
#define 	MAADR4   (0x05|0x60|0x80)
#define 	EBSTSD   (0x06|0x60)
#define 	EBSTCON   (0x07|0x60)
#define 	EBSTCSL   (0x08|0x60)
#define 	EBSTCSH   (0x09|0x60)
#define 	MISTAT   (0x0A|0x60|0x80)
#define 	EREVID   (0x12|0x60)
#define 	ECOCON   (0x15|0x60)
#define 	EFLOCON   (0x17|0x60)
#define 	EPAUSL   (0x18|0x60)
#define 	EPAUSH   (0x19|0x60)
#define 	PHCON1   0x00
#define 	PHSTAT1   0x01
#define 	PHHID1   0x02
#define 	PHHID2   0x03
#define 	PHCON2   0x10
#define 	PHSTAT2   0x11
#define 	PHIE   0x12
#define 	PHIR   0x13
#define 	PHLCON   0x14
#define 	EIE_INTIE   0x80
#define 	EIE_PKTIE   0x40
#define 	EIE_DMAIE   0x20
#define 	EIE_LINKIE   0x10
#define 	EIE_TXIE   0x08
#define 	EIE_WOLIE   0x04
#define 	EIE_TXERIE   0x02
#define 	EIE_RXERIE   0x01
#define 	EIR_PKTIF   0x40
#define 	EIR_DMAIF   0x20
#define 	EIR_LINKIF   0x10
#define 	EIR_TXIF   0x08
#define 	EIR_WOLIF   0x04
#define 	EIR_TXERIF   0x02
#define 	EIR_RXERIF   0x01
#define 	ESTAT_INT   0x80
#define 	ESTAT_LATECOL   0x10
#define 	ESTAT_RXBUSY   0x04
#define 	ESTAT_TXABRT   0x02
#define 	ESTAT_CLKRDY   0x01
#define 	ECON2_AUTOINC   0x80
#define 	ECON2_PKTDEC   0x40
#define 	ECON2_PWRSV   0x20
#define 	ECON2_VRPS   0x08
#define 	ECON1_TXRST   0x80
#define 	ECON1_RXRST   0x40
#define 	ECON1_DMAST   0x20
#define 	ECON1_CSUMEN   0x10
#define 	ECON1_TXRTS   0x08
#define 	ECON1_RXEN   0x04
#define 	ECON1_BSEL1   0x02
#define 	ECON1_BSEL0   0x01
#define 	MACON1_LOOPBK   0x10
#define 	MACON1_TXPAUS   0x08
#define 	MACON1_RXPAUS   0x04
#define 	MACON1_PASSALL   0x02
#define 	MACON1_MARXEN   0x01
#define 	MACON2_MARST   0x80
#define 	MACON2_RNDRST   0x40
#define 	MACON2_MARXRST   0x08
#define 	MACON2_RFUNRST   0x04
#define 	MACON2_MATXRST   0x02
#define 	MACON2_TFUNRST   0x01
#define 	MACON3_PADCFG2   0x80
#define 	MACON3_PADCFG1   0x40
#define 	MACON3_PADCFG0   0x20
#define 	MACON3_TXCRCEN   0x10
#define 	MACON3_PHDRLEN   0x08
#define 	MACON3_HFRMLEN   0x04
#define 	MACON3_FRMLNEN   0x02
#define 	MACON3_FULDPX   0x01
#define 	MICMD_MIISCAN   0x02
#define 	MICMD_MIIRD   0x01
#define 	MISTAT_NVALID   0x04
#define 	MISTAT_SCAN   0x02
#define 	MISTAT_BUSY   0x01
#define 	PHCON1_PRST   0x8000
#define 	PHCON1_PLOOPBK   0x4000
#define 	PHCON1_PPWRSV   0x0800
#define 	PHCON1_PDPXMD   0x0100
#define 	PHSTAT1_PFDPX   0x1000
#define 	PHSTAT1_PHDPX   0x0800
#define 	PHSTAT1_LLSTAT   0x0004
#define 	PHSTAT1_JBSTAT   0x0002
#define 	PHCON2_FRCLINK   0x4000
#define 	PHCON2_TXDIS   0x2000
#define 	PHCON2_JABBER   0x0400
#define 	PHCON2_HDLDIS   0x0100
#define 	PKTCTRL_PHUGEEN   0x08
#define 	PKTCTRL_PPADEN   0x04
#define 	PKTCTRL_PCRCEN   0x02
#define 	PKTCTRL_POVERRIDE   0x01
#define 	ENC28J60_READ_CTRL_REG   0x00
#define 	ENC28J60_READ_BUF_MEM   0x3A
#define 	ENC28J60_WRITE_CTRL_REG   0x40
#define 	ENC28J60_WRITE_BUF_MEM   0x7A
#define 	ENC28J60_BIT_FIELD_SET   0x80
#define 	ENC28J60_BIT_FIELD_CLR   0xA0
#define 	ENC28J60_SOFT_RESET   0xFF
#define 	TXSTART_INIT   0x0000
#define 	RXSTART_INIT   0x0600
#define 	RXSTOP_INIT   0x1FFF
#define 	MAX_FRAMELEN   1518
#define 	ETHERNET_MIN_PACKET_LENGTH   0x3C

Functions
/// 	do a ENC28J60 read operation
u08 	enc28j60ReadOp (u08 op, u08 address);

/// 	do a ENC28J60 write operation
void 	enc28j60WriteOp (u08 op, u08 address, u08 data);

/// 	read the packet buffer memory
void 	enc28j60ReadBuffer (u16 len, u08 *data);

/// 	write the packet buffer memory
void 	enc28j60WriteBuffer (u16 len, u08 *data);

/// 	set the register bank for register at address
void 	enc28j60SetBank (u08 address);

/// 	read ax88796 register
u08 	enc28j60Read (u08 address);

/// 	write ax88796 register
void 	enc28j60Write (u08 address, u08 data);

///	read a PHY register
u16 enc28j60PhyRead (u08 address);

/// 	write a PHY register
void 	enc28j60PhyWrite (u08 address, u16 data);

/// 	initialize the ethernet interface for transmit/receive
void 	enc28j60Init (void);

void 	enc28j60PacketSend (unsigned int len, unsigned char *packet)

unsigned int 	enc28j60PacketReceive (unsigned int maxlen, unsigned char *packet)

void 	enc28j60ReceiveOverflowRecover (void)

/// 	formatted print of important ENC28J60 registers
void 	enc28j60RegDump (void)
#endif
