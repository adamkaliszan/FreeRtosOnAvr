#ifndef SPI_STD_H
#define SPI_STD_H

#include "main.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"

#define SS_ON_PORTD_MASK    0xFF
#define DISABLE_SS_ON_PORTD 0xFF

//Enc28j60        PE3 - SS
#define SS_ON_PORTE_MASK    0x08
#define DISABLE_SS_ON_PORTE 0x08

//SD              PG3 - CS
#define SS_ON_PORTG_MASK    0x08
#define DISABLE_SS_ON_PORTG 0x08


xSemaphoreHandle  xSemaphoreSpiSS;
xQueueHandle      xSpiRx;


void    spiInit                (void);
void    disableAllDevices      (void);
void    enableExternalSpiDevice(uint8_t devices);
void    enableEnc              (void);
void    enableSD               (void);
uint8_t spiSend                (uint8_t data);


#endif