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
  PORTB = 0xD1;
  /*
   0 - Sl_RST
   1 - SCK
   2 - MOSI
   3 - MISO
   4 - External SPI ASR 4
   5 - External SPI ASR 5 (DS1305)     0 - off; 1 - on 
   6 - External SPI ASR 6 (MCP3008)    0 - on;  1 - off
   7 - External SPI ASR 7 (MPC23S17)   0 - on;  1 - off
  */
  
  //DDRC = 0x00;  //External Memory

  DDRD = 0x00;
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

void LockersMemInit(void)
{
  lockSensors = xmalloc(4 * sizeof(struct lockerSensor));
}

uint8_t printLockers(FILE *stream)
{
  uint8_t i;
  uint8_t result = 0;
  struct lockerSensor *tmpLock = lockSensors;
  for (i=1; i<=4; i++)
  {
    if (tmpLock->enabled)
    {
      fprintf_P(stream, statusLockerSensDescStr, i);
      if (tmpLock->threshold > tmpLock->acVal)
        fprintf_P(stream, statusLockerOpenStr);
      else
        fprintf_P(stream, statusLockerCloseStr);
      fprintf_P(stream, statusLockerSensAdditionalDescStr, tmpLock->threshold, tmpLock->acVal);
      result++;
    }
    tmpLock++;
  }
  return result;
}

void checkLockerSensors(void)
{
  if (lockSensors[0].enabled)
  {
    MPC23s17SetBitsOnPortA(LOCK_SENS_1_LIGHT, 0);
    vTaskDelay(30);
    lockSensors[0].acVal = MCP3008_getSampleSingle(LOCK_SENS_1_AC_IN);
    MPC23s17ClearBitsOnPortA(LOCK_SENS_1_LIGHT, 0);
    lockSensors[0].locked = (lockSensors[0].acVal > lockSensors[0].threshold) ? 1 : 0;
    vTaskDelay(10);
  }
  
  if (lockSensors[1].enabled)
  {
    MPC23s17SetBitsOnPortA(LOCK_SENS_2_LIGHT, 0);
    vTaskDelay(30);
    lockSensors[1].acVal = MCP3008_getSampleSingle(LOCK_SENS_2_AC_IN);
    MPC23s17ClearBitsOnPortA(LOCK_SENS_2_LIGHT, 0);
    lockSensors[1].locked = (lockSensors[1].acVal > lockSensors[1].threshold) ? 1 : 0;
    vTaskDelay(10);
  }
  
  if (lockSensors[2].enabled)
  {
    MPC23s17SetBitsOnPortA(LOCK_SENS_3_LIGHT, 0);
    vTaskDelay(30);
    lockSensors[2].acVal = MCP3008_getSampleSingle(LOCK_SENS_3_AC_IN);
    MPC23s17ClearBitsOnPortA(LOCK_SENS_3_LIGHT, 0);
    lockSensors[2].locked = (lockSensors[2].acVal > lockSensors[2].threshold) ? 1 : 0;
    vTaskDelay(10);
  }
  
  if (lockSensors[3].enabled)
  {
    MPC23s17SetBitsOnPortA(LOCK_SENS_4_LIGHT, 0);
    vTaskDelay(30);
    lockSensors[3].acVal = MCP3008_getSampleSingle(LOCK_SENS_4_AC_IN);
    MPC23s17ClearBitsOnPortA(LOCK_SENS_4_LIGHT, 0);
    lockSensors[3].locked = (lockSensors[3].acVal > lockSensors[3].threshold) ? 1 : 0;
    vTaskDelay(10);
  }
}


uint8_t spiSend(uint8_t data)
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
}

void enableSpiSd(void)
{
#if SD_SPI_CS_EN_MASK_OR != 0
  SD_SPI_CS_PORT |= SD_SPI_CS_EN_MASK_OR;
#endif
#if SD_SPI_CS_EN_MASK_AND != 0xFF
  SD_SPI_CS_PORT &= SD_SPI_CS_EN_MASK_AND;
#endif   
}

void disableSpiSd(void)
{
#if SD_SPI_CS_EN_MASK_OR != 0
  SD_SPI_CS_PORT &= (~SD_SPI_CS_EN_MASK_OR);
#endif
#if SD_SPI_CS_EN_MASK_AND != 0xFF
  SD_SPI_CS_PORT |= (~SD_SPI_CS_EN_MASK_AND);
#endif  
}

void enableSpiMPC23S17(void)
{
#if MCP23S17_SPI_CS_EN_MASK_OR != 0
  MCP23S17_SPI_CS_PORT |= MCP23S17_SPI_CS_EN_MASK_OR;
#endif
#if MCP23S17_SPI_CS_EN_MASK_AND != 0xFF
  MPC23S17_SPI_CS_PORT &= MPC23S17_SPI_CS_EN_MASK_AND;
#endif
}

void disableSpiMPC23S17(void)
{
#if MCP23S17_SPI_CS_EN_MASK_OR != 0
  MCP23S17_SPI_CS_PORT &= (~MCP23S17_SPI_CS_EN_MASK_OR);
#endif
#if MCP23S17_SPI_CS_EN_MASK_AND != 0xFF
  MPC23S17_SPI_CS_PORT |= (~MPC23S17_SPI_CS_EN_MASK_AND);
#endif
}

#define MCP3008_SPCR_OR_MASK ((1<<SPR1)|(1<<SPR0))
void enableSpiMCP3008(void)
{
  SPCR |= MCP3008_SPCR_OR_MASK;
#if MCP3008_SPI_CS_EN_MASK_OR != 0
  MCP3008_SPI_CS_PORT |= MCP3008_SPI_CS_EN_MASK_OR;
#endif
#if MCP3008_SPI_CS_EN_MASK_AND != 0xFF
  MCP3008_SPI_CS_PORT &= MCP3008_SPI_CS_EN_MASK_AND;
#endif  

}

void disableSpiMCP3008(void)
{
  SPCR &= ~MCP3008_SPCR_OR_MASK;
  #if MCP3008_SPI_CS_EN_MASK_OR != 0
  MCP3008_SPI_CS_PORT &= (~MCP3008_SPI_CS_EN_MASK_OR);
#endif
#if MCP3008_SPI_CS_EN_MASK_AND != 0xFF
  MCP3008_SPI_CS_PORT |= (~MCP3008_SPI_CS_EN_MASK_AND);
#endif
}

#define DS_SPCR_OR_MASK ((1<<CPHA)|(1<<SPR0))

void spiEnableDS1305(void)
{
  SPCR |= DS_SPCR_OR_MASK;
#if DS1305_SPI_CS_EN_MASK_OR != 0
  DS1305_SPI_CS_PORT |= DS1305_SPI_CS_EN_MASK_OR;
#endif
#if DS1305_SPI_CS_EN_MASK_AND != 0xFF
  DS1305_SPI_CS_PORT &= DS1305_SPI_CS_EN_MASK_AND;
#endif
}

void spiDisableDS1305(void)
{
  SPCR &= (~(DS_SPCR_OR_MASK));
#if DS1305_SPI_CS_EN_MASK_OR != 0
  DS1305_SPI_CS_PORT &= (~(DS1305_SPI_CS_EN_MASK_OR));
#endif
#if DS1305_SPI_CS_EN_MASK_AND != 0xFF
  DS1305_SPI_CS_PORT |= (~(DS1305_SPI_CS_EN_MASK_AND));
#endif  
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
