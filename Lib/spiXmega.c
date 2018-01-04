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

  //SPCR  = (1<<SPE)|(1<<MSTR)|(1<<SPIE);  //TODO Adam: użyć odpowiednich nazw rejestrów
  //SPSR |= (1<<SPI2X);
  SPID.CTRL=(1<<SPI_ENABLE_bp)|(1<<SPI_MASTER_bp);
  SPID.INTCTRL|=(1<<SPI_INTLVL1_bp);//0x01;
  SPID.CTRL|=(1<<SPI_CLK2X_bp);
  portEXIT_CRITICAL();

  //mode 0,0
}
void spiInitENC(void (*disableAllSpiDevicesFun)(void)) // PORTC
{
  disableAllSpiDevicesFun();
  portENTER_CRITICAL();
  vSemaphoreCreateBinary(xSemaphoreSpiSS);

  //SPCR  = (1<<SPE)|(1<<MSTR)|(1<<SPIE);  //TODO Adam: użyć odpowiednich nazw rejestrów
  //SPSR |= (1<<SPI2X);
  SPIC.CTRL=(1<<SPI_ENABLE_bp)|(1<<SPI_MASTER_bp);
  SPIC.INTCTRL|=(1<<SPI_INTLVL1_bp);//0x01;
  SPIC.CTRL|=(1<<SPI_CLK2X_bp);
  portEXIT_CRITICAL();

  //mode 0,0
}

void spiSetCPHA(void)
{
  //SPCR |= (1<<CPHA);
  SPID.CTRL|=(1<<SPI_MODE_gp);
}
void spiSetCPHAENC(void)
{
  //SPCR |= (1<<CPHA);
  SPIC.CTRL|=(1<<SPI_MODE_gp);
}

void spiClearCPHA(void)
{
  //SPCR &= ~(1<<CPHA);
  SPID.CTRL&=~(1<<SPI_MODE_gp);
}
void spiClearCPHAENC(void)
{
  //SPCR &= ~(1<<CPHA);
  SPIC.CTRL&=~(1<<SPI_MODE_gp);
}


void spiSetCPOL(void)
{
  //SPCR |= (1<<CPOL);
  SPID.CTRL|=(1<<SPI_MODE1_bp);
}
void spiSetCPOLENC(void)
{
  //SPCR |= (1<<CPOL);
  SPIC.CTRL|=(1<<SPI_MODE1_bp);
}

void spiClearCPOL(void)
{
  //SPCR &= ~(1<<CPOL);
   SPID.CTRL&=~(1<<SPI_MODE1_bp);
}
void spiClearCPOLENC(void)
{
  //SPCR &= ~(1<<CPOL);
   SPID.CTRL&=~(1<<SPI_MODE1_bp);
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

//
