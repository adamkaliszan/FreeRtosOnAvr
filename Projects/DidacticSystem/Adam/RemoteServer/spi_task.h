#ifndef SPI_TASK_H
#define SPI_TASK_H

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "enc28j60.h"
#include "net.h"
#include "nic.h"
#include "ip.h"
#include "arp.h"
#include "udp.h"
#include "icmp.h"
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"

#include <util/crc16.h>


#if NET_DEBUG


FILE   *netDebug;
uint8_t netDebugLevel;
void setNetDebug(FILE *stream, uint8_t level);

#endif /*NET_DEBUG*/



extern xQueueHandle xSpi2SerialTx[];
extern xQueueHandle xSpi2SerialRx[];

extern uint8_t NoOfSerialPorts;
extern uint8_t NoOfSpiSlaves;



enum REC_STATE
{
    ST_SYNC,
    ST_CMD,
    ST_LEN,
    ST_DATA,
    ST_CRC_LO,
    ST_CRC_HI
} ;

void spiTask ( void *pvParameters );
void XpNetReceiverTask ( void *pvParameters );
void XpNetTransmitterTask ( void *pvParameters );

void handleSpiDev(uint8_t spiDevNo);
void initQeuesSpi2Serial(void);

/** Format wiadomosci spi
Bajt danych odebrany przez porty szeregowych wsyłany jest jako 2 bajty przez SPI. Pierwszy bajt zawiera bity tanych 3-0, natomiast drugi bajt bity danych 7-4. Istotna jest kolejność

 * bit  | znaczenie
 * 7, 6 | 0x dane, 10 pusta ramka 11 rozkaz,

Format danych
 * 5    | portNo
 * 4    | 1 in data are stored bits 7-4, 0 in data are stored bits 3-0
 * 3-0  | data bits


Format rozkazu
 * 5    | 1 power On, 0 power Off
 * 4    | portNo
 * 3-0  | data bits

Pusta ramka gdy:
 - slave wysyła 0xCF
 - master wysyłą 0x8F

 */

#define POWER_OFF 0xFE
#define POWER_ON  0xFF


/** Format wiadomości UDP
 * 1 bajt Sync - wartość 0xA5
 *
 */

#endif // SPI_TASK_H
