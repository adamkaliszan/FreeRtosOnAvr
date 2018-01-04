#include <stdlib.h>
#include <avr/io.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"
#include "spi.h"
#include "semphr.h"


void spiInit(void (*disableAllSpiDevicesFun)(void))
{
  disableAllSpiDevicesFun();
  portENTER_CRITICAL();
  vSemaphoreCreateBinary(xSemaphoreSpiSS);

  SPCR  = (1<<SPE)|(1<<MSTR)|(1<<SPIE)|(1<<SPR0) ;  //TODO Adam: użyć odpowiednich nazw rejestrów
  //SPSR |= (1<<SPI2X);

  portEXIT_CRITICAL();

  //mode 0,0
}

void spiSetCPHA(void)
{
  SPCR |= (1<<CPHA);
}

void spiClearCPHA(void)
{
  SPCR &= ~(1<<CPHA);
}

void spiSetCPOL(void)
{
  SPCR |= (1<<CPOL);
}

void spiClearCPOL(void)
{
  SPCR &= ~(1<<CPOL);
}

void spiTake(void)
{
  xSemaphoreTake(xSemaphoreSpiSS, portMAX_DELAY);
}

void spiGive(void)
{
  xSemaphoreGive(xSemaphoreSpiSS);
}

uint8_t spiSend(uint8_t data)          { (void) data;  return 0; }
uint8_t spiSendSpinBlock(uint8_t data) { (void) data;  return 0; }

