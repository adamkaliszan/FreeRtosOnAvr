#include "hardware.h"

#if LANG_EN
#include "hardware_en.h"
#endif

#if LANG_PL
#include "hardware_pl.h"
#endif

xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu

void hardwareInit(void)
{
  //DDRA = 0x00;  //External Memory
  portENTER_CRITICAL();
  xSpiRx          = xQueueCreate(1, 1);
  portEXIT_CRITICAL();

  DDRB = 0xF7;
  PORTB = 0xF1;
  /*
   0 - Sl_RST
   1 - SCK
   2 - MOSI
   3 - MISO
   4 - External SPI ASR 4
   5 - External SPI ASR 5
   6 - External SPI ASR 6
   7 - External SPI ASR 7
  */

  //DDRC = 0x00;  //External Memory

  DDRD = 0xF0;
  /*
   0 - SCL
   1 - SDA
   2 - RxD USB
   3 - TxD USB
   4 - External SPI ASR 0
   5 - External SPI ASR 1
   6 - External SPI ASR 2
   7 - External SPI ASR 3
   */

  DDRE  = 0x0E;
  PORTE = 0x0C;
  /*
   0 - RxD Rs485
   1 - TxD Rs485
   2 - ENC RST
   3 - ENC CS
   4 - INT 4
   5 - INT 5
   6 - INT 6
   7 - INT Enc28j60
  */
  DDRF = 0x00;    //JTAG and A/C
  DDRG = 0x1F;
  /*
   0 - WR
   1 - RD
   2 - ALE
   3 - SD CS
   4 - RS485 TxEn
   5 -
   6 -
   7 -
   */
}

uint8_t spiSend(uint8_t data)
{
  uint8_t result;
  SPDR = data;
  xQueueReceive(xSpiRx, &result, 10);
  return result;
}

uint8_t spiSendENC(uint8_t data)
{
  uint8_t result;
  SPDR = data;
  xQueueReceive(xSpiRx, &result, 10);
  return result;
}

uint8_t spiSendSpinBlock(uint8_t data)
{
  SPDR = data;
  SPCR &= ~(1<<SPIE);                //Disable SPI interrupt
  while(!(SPSR&(1<<SPIF)));
  data = SPSR;                       //Clearing interrupt flag
  data = SPDR;                       //Resfing DPI buffer register
  SPCR |= (1<<SPIE);                 //Enable SPI Interrupt
  return data;
}

uint8_t spiSendENCSpinBlock(uint8_t data)
{
  SPDR = data;
  SPCR &= ~(1<<SPIE);                //Disable SPI interrupt
  while(!(SPSR&(1<<SPIF)));
  data = SPSR;                       //Clearing interrupt flag
  data = SPDR;                       //Resfing DPI buffer register
  SPCR |= (1<<SPIE);                 //Enable SPI Interrupt
  return data;
}

void disableAllSpiDevices(void)
{
#if disableSpiPORTA_OR != 0
#error Port A is memory bus
  PORTA |= disableSpiPORTA_OR;
#endif
#if disableSpiPORTA_AND != 0xFF
#error Port A is memory bus
  PORTA &= disableSpiPORTA_AND;
#endif

#if disableSpiPORTB_OR != 0
  PORTB |= disableSpiPORTB_OR;
#endif
#if disableSpiPORTB_AND != 0xFF
  PORTB &= disableSpiPORTB_AND;
#endif

#if disableSpiPORTC_OR != 0
#error Port C is memory bus
  PORTC |= disableSpiPORTC_OR;
#endif
#if disableSpiPORTC_AND != 0xFF
#error Port C is memory bus
  PORTC &= disableSpiPORTC_AND;
#endif

#if disableSpiPORTD_OR != 0
  PORTD |= disableSpiPORTD_OR;
#endif
#if disableSpiPORTD_AND != 0xFF
  PORTD &= disableSpiPORTD_AND;
#endif
#if disableSpiPORTE_OR != 0
  PORTE |= disableSpiPORTE_OR;
#endif
#if disableSpiPORTE_AND != 0xFF
  PORTE &= disableSpiPORTE_AND;
#endif

#if disableSpiPORTF_OR != 0
  PORTF |= disableSpiPORTF_OR;
#endif
#if disableSpiPORTF_AND != 0xFF
  PORTF &= disableSpiPORTF_AND;
#endif

#if disableSpiPORTG_OR != 0
  PORTG |= disableSpiPORTG_OR;
#endif
#if disableSpiPORTG_AND != 0xFF
  PORTG &= disableSpiPORTG_AND;
#endif
}

void spiEnableEnc28j60(void)
{
  spiTake();
//SPSR |= (1<<SPI2X);
#if ENC_SPI_CS_EN_MASK_OR != 0
  ENC_SPI_CS_PORT |= ENC_SPI_CS_EN_MASK_OR;
#endif
#if ENC_SPI_CS_EN_MASK_AND != 0xFF
  ENC_SPI_CS_PORT &= ENC_SPI_CS_EN_MASK_AND;
#endif
}

void spiDisableEnc28j60(void)
{
#if ENC_SPI_CS_EN_MASK_OR != 0
  ENC_SPI_CS_PORT &= (~ENC_SPI_CS_EN_MASK_OR);
#endif
#if ENC_SPI_CS_EN_MASK_AND != 0xFF
  ENC_SPI_CS_PORT |= (~ENC_SPI_CS_EN_MASK_AND);
#endif
//SPSR = 0;
  spiGive();
}

void spiEnableDev(uint8_t devNo)
{
  spiTake();
  switch(devNo)
  {
  case 0: PORTD &= 0xEF; break;
  case 1: PORTD &= 0xDF; break;
  case 2: PORTD &= 0xBF; break;
  case 3: PORTD &= 0x7F; break;
  case 4: PORTB &= 0xEF; break;
  case 5: PORTB &= 0xDF; break;
  case 6: PORTB &= 0xBF; break;
  case 7: PORTB &= 0x7F; break;
  default: break;
  }
}

void spiDisableDev(uint8_t devNo)
{
  switch(devNo)
  {
  case 0:
  case 1:
  case 2:
  case 3: PORTD |= 0xF0; break;
  case 4:
  case 5:
  case 6:
  case 7: PORTB |= 0xF0; break;
  default: break;
  }
  spiGive();
}


ISR(SPI_STC_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;

  static uint8_t data;
  data = SPDR;

  xQueueSendFromISR(xSpiRx, &data, &xHigherPriorityTaskWoken);

  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }

  //clear SPI interrupt SPI |= 1;
}
