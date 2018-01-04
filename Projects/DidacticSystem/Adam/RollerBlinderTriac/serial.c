#include <stdlib.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
#include "../../freeRtos/Lib/include/protocol1.h"
#include "hardware.h"

//#define debug 1

/*-----------------------------------------------------------*/

static xQueueHandle xRxedChars;
static xQueueHandle xCharsForTx;


static volatile uint8_t kodRozkazu;
static volatile uint8_t dlDanych;
static uint8_t bDane[MAX_DATA_LEN];

static uint8_t wiadomosc;


static xSemaphoreHandle xSemaphore;

static uint8_t crcLo;
static uint8_t crcHi;
static uint16_t crc;

static uint8_t wykonajRozkaz(void)
{
//  static  portBASE_TYPE   xResult;
  uint8_t                 wysylac   = 0;

  switch (kodRozkazu)
  {
    case rOpuscRolete1:
      wiadomosc = 0x3F;
      wysylac = 2;
      break;

    case rOpuscRolete2:
#if X2
      wiadomosc = 0x3F;
      wysylac = 3;
#endif
      break;

    case rPodniesRolete1:
      wiadomosc = 0xBF;
      wysylac = 2;
      break;

    case rPodniesRolete2:
#if X2
      wiadomosc = 0xBF;
      wysylac = 3;
#endif
      break;

    case rZatrzymajRolete1:
      wiadomosc = 0x40;
      wysylac = 2;
      break;

    case rZatrzymajRolete2:
#if X2
      wiadomosc = 0x40;
      wysylac = 3;
#endif
      break;

    case rPING:
      wysylac = 1;
      break;
    case rHELLO:
      wysylac = 4;
      break;
    case rFLASH:
      wysylac = 1;
      break;
      
    case rUstaw:
      wczytajUstawienia(bDane[0]);
      settings = bDane[0];
      break;
      
    case rZapiszUstawienia:
      eeprom_write_byte(&settingsEep, settings);	
      break;
  }
  return wysylac;
}

void vProtocol(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  (void) uxIndex;

  crSTART( xHandle );
  static t_serialState   stan;
  static uint8_t         znak;
  static portBASE_TYPE   xResult;
  static uint8_t         dobryAdres;
  static uint8_t         lOdebrDanych;
  static uint8_t         rezultat;
  stan = s_sync;
  
  for( ;; )
  {
    if (stan == s_sync)
    {
      znak=0;
      crQUEUE_RECEIVE(xHandle, xRxedChars, &znak, portMAX_DELAY, &xResult);
      if ((xResult == pdPASS) && (znak == SYNC))
      {
        stan = s_addr;
	//TODO tutaj jest zawsze wartość stała. Lepiej ją przypisać
        crc = _crc_xmodem_update(0, znak);
      }
    }
    if (stan == s_addr)
    {
      crQUEUE_RECEIVE(xHandle, xRxedChars, &znak, 1, &xResult);
      if (xResult == pdPASS)
      {
        stan = s_rozkaz;
        crc = _crc_xmodem_update(crc, znak);
        if (znak == adres)
          dobryAdres = 1;
        else
          dobryAdres = 0;
      }
      else
      {
        stan = s_sync;
      }
    }
    if (stan == s_rozkaz)
    {
      crQUEUE_RECEIVE(xHandle, xRxedChars, (void *)(&kodRozkazu), 1, &xResult);
      if (xResult == pdPASS)
      {
        crc = _crc_xmodem_update(crc, kodRozkazu);
        stan = s_len;
      }
      else
      {
        stan = s_sync;
      }
    }
    if (stan == s_len)
    {
      crQUEUE_RECEIVE(xHandle, xRxedChars, (void *)(&dlDanych), 1, &xResult);
      if (xResult == pdPASS)
      {
        crc = _crc_xmodem_update(crc, dlDanych);
        lOdebrDanych = 0;
        stan = s_dane;
      }
      else
      {
        stan = s_sync;
      }
    }
    if (stan == s_dane)
    {
      if (lOdebrDanych == dlDanych)
      {
        stan = s_CRC_HI;
      }
      else
      {
        crQUEUE_RECEIVE(xHandle, xRxedChars, &znak, 1, &xResult);
        if (xResult == pdPASS)
        {
          crc = _crc_xmodem_update(crc, znak);
          if (lOdebrDanych < MAX_DATA_LEN)
            bDane[lOdebrDanych] = znak;
          lOdebrDanych++; 
        }
        else
        {
          stan = s_sync;
        }
      }
    }
    if (stan == s_CRC_HI)
    {
      crQUEUE_RECEIVE(xHandle, xRxedChars, (void *)(&crcHi), 1, &xResult);
      if (xResult == pdPASS)
      {
        stan = s_CRC_LO;
      }
      else
      {
        stan = s_sync;
      }    
    }
    if (stan == s_CRC_LO)
    {
      crQUEUE_RECEIVE(xHandle, xRxedChars, (void *)(&crcLo), 1, &xResult);
      if (xResult == pdPASS)
      {
        if ((crcHi != (uint8_t)(crc >> 8)) || (crcLo != (uint8_t)(crc & 0xFF)))
        {
          stan = s_sync;
        }
        else
        {
          stan = s_CRC_OK;
        }
      }
    }
    if (stan == s_CRC_OK)
    {
      if (dobryAdres == 1)
      {
        if (lOdebrDanych > MAX_DATA_LEN)
          lOdebrDanych = MAX_DATA_LEN;
        rezultat = wykonajRozkaz();
        if (rezultat == 1)
        {
          //SYNC
          uint8_t temp;
          temp = SYNC;
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&temp), 0, &xResult);
          crc = _crc_xmodem_update(0, temp);

         //ADRES 0x00 adres mastera
          temp = 0x00;
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&temp), 0, &xResult);
          crc = _crc_xmodem_update(crc, temp);
            
          //Rozkaz
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&kodRozkazu), 0, &xResult);
          crc = _crc_xmodem_update(crc, kodRozkazu);
          
          //Długość danych
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&lOdebrDanych), 0, &xResult);
          crc = _crc_xmodem_update(crc, lOdebrDanych);
          
          //Dane
          for (temp = 0; temp < lOdebrDanych; temp++)
          {
            crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&bDane[temp]), 1, &xResult);
            crc = _crc_xmodem_update(crc, bDane[temp]);
          }
  
          temp = (uint8_t)(crc>>8);
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&temp), 1, &xResult);
          temp = (uint8_t)(crc & 0xFF);
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&temp), 1, &xResult);

          if (xResult == pdPASS)
          {
            TxStart();
          }
          vInterruptOn();  //W przypadku błędu wysyłamy wszystko z bufora przy wyłączonym nadajniku
        
          if (kodRozkazu == rFLASH)
          {
            (*((void(*)(void))BOOT_START))();            //reboot
          }
        }
        else if (rezultat == 2)
        {
          crQUEUE_SEND(xHandle, xRoleta[0], (void *)(&wiadomosc), 0, &xResult); 
        }
#if X2
        else if (rezultat == 3)
        {
          crQUEUE_SEND(xHandle, xRoleta[1], (void *)(&wiadomosc), 0, &xResult); 
        }
#endif
        else if (rezultat == 4)
        {
          //SYNC
          crc = 0;
          uint8_t temp;
          
          //Dane 
          bHelloResp[HDR_LEN+2] = settings;
          for (temp = 0; temp < HELLO_RESP_LEN+HDR_LEN; temp++)
          {
            crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&bHelloResp[temp]), 1, &xResult);
            crc = _crc_xmodem_update(crc, bHelloResp[temp]);
          }
  
          temp = (uint8_t)(crc>>8);
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&temp), 1, &xResult);
          temp = (uint8_t)(crc & 0xFF);
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&temp), 1, &xResult);

          if (xResult == pdPASS)
          {
            TxStart();
          }
          vInterruptOn();  //W przypadku błędu wysyłamy wszystko z bufora przy wyłączonym nadajniku
        }
        stan = s_sync;
      }
      else //Zły adres
      {
        if (kodRozkazu == rFLASH)
        {
          DISABLE_RX();
          crDELAY(xHandle, 1000);
          ENABLE_RX();
        }
        stan = s_sync;
      }
    }
  }
  crEND();
}

void xSerialPortInitMinimal(unsigned portBASE_TYPE uxQueueLength )
{
  portENTER_CRITICAL();
  {
    /* Create the queues used by the com test task. */
    xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
    xCharsForTx = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );

    UBRR0L = 3;
    UBRR0H = 0;

    /* Enable the Rx interrupt.  The Tx interrupt will get enabled later. Also enable the Rx and Tx. */
    UCSR0B = ((1<<RXCIE0)|(1<<TXCIE0)|(1<<TXEN0)|(1<<RXEN0));

    /* Set the data bits to 8. */
    UCSR0C = ( serUCSRC_SELECT | serEIGHT_DATA_BITS );
    
    vSemaphoreCreateBinary( xSemaphore );
  }
  portEXIT_CRITICAL();
  return;
}

/*-----------------------------------------------------------*/
ISR(USART_RX_vect)
{
  signed portCHAR cChar;
  cChar = UDR0;
  crQUEUE_SEND_FROM_ISR( xRxedChars, &cChar, pdFALSE );
}
/*-----------------------------------------------------------*/

ISR(USART_UDRE_vect)
{
  signed portCHAR cChar, cTaskWoken;

  if( xQueueReceiveFromISR( xCharsForTx, &cChar, &cTaskWoken ) == pdTRUE )
  {
    /* Send the next character queued for Tx. */
    TxStart();
    UDR0 = cChar;
  }
  else
  {
    /* Queue empty, nothing to send. */
    vInterruptOff();
  }
}

ISR(USART_TX_vect)
{
  if (!vIsInterruptOn())
  {
    TxStop();
  }
}
