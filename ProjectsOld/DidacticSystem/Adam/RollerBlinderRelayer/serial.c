/*
	FreeRTOS.org V5.2.0 - Copyright (C) 2003-2009 Richard Barry.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify it 
	under the terms of the GNU General Public License (version 2) as published
	by the Free Software Foundation and modified by the FreeRTOS exception.

	FreeRTOS.org is distributed in the hope that it will be useful,	but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
	more details.

	You should have received a copy of the GNU General Public License along 
	with FreeRTOS.org; if not, write to the Free Software Foundation, Inc., 59 
	Temple Place, Suite 330, Boston, MA  02111-1307  USA.

	A special exception to the GPL is included to allow you to distribute a 
	combined work that includes FreeRTOS.org without being obliged to provide
	the source code for any proprietary components.  See the licensing section
	of http://www.FreeRTOS.org for full details.


	***************************************************************************
	*                                                                         *
	* Get the FreeRTOS eBook!  See http://www.FreeRTOS.org/Documentation      *
	*                                                                         *
	* This is a concise, step by step, 'hands on' guide that describes both   *
	* general multitasking concepts and FreeRTOS specifics. It presents and   *
	* explains numerous examples that are written using the FreeRTOS API.     *
	* Full source code for all the examples is provided in an accompanying    *
	* .zip file.                                                              *
	*                                                                         *
	***************************************************************************

	1 tab == 2 spaces!

	Please ensure to read the configuration and relevant port sections of the
	online documentation.

	http://www.FreeRTOS.org - Documentation, latest information, license and
	contact details.

	http://www.SafeRTOS.com - A version that is certified for use in safety
	critical systems.

	http://www.OpenRTOS.com - Commercial support, development, porting,
	licensing and training services.
*/
#include <stdlib.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
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
static portBASE_TYPE xHigherPriorityTaskWoken; 

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
      wiadomosc = 0x3F;
      wysylac = 3;
      break;

    case rPodniesRolete1:
      wiadomosc = 0xBF;
      wysylac = 2;
      break;

    case rPodniesRolete2:
      wiadomosc = 0xBF;
      wysylac = 3;
      break;

    case rZatrzymajRolete1:
      wiadomosc = 0x40;
      wysylac = 2;
      break;

    case rZatrzymajRolete2:
      wiadomosc = 0x40;
      wysylac = 3;
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
  
/*
  for ( ;; )
  {
    static uint8_t tmp = 'C';
    crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&tmp), 0, &xResult);
    Led1On();
    TxStart();
    vInterruptOn();  //W przypadku błędu wysyłamy wszystko z bufora przy wyłączonym nadajniku
    crDELAY(xHandle, 100);

    crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&tmp), 0, &xResult);
    Led1Off();
    TxStart();
    vInterruptOn();  //W przypadku błędu wysyłamy wszystko z bufora przy wyłączonym nadajniku
    crDELAY(xHandle, 100);
  }
*/
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
      Led1On();
      Led2Off();
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
      Led1Off();
      Led2On();
      crQUEUE_RECEIVE(xHandle, xRxedChars, (void *)(&dlDanych), 1, &xResult);
      if (xResult == pdPASS)
      {
        crc = _crc_xmodem_update(crc, dlDanych);
        lOdebrDanych = 0;
        stan = s_dane;
        Led1On();
        Led2On();
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
        //Led2Off();
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
          Led1Off();
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
        Led1Off();
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
          Led1Off();
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
            Led1On();
            Led2On();
            crDELAY(xHandle, 10);
            Led1Off();
            Led2Off();
            (*((void(*)(void))BOOT_START))();            //reboot
          }
        }
        else if (rezultat == 2)
        {
          crQUEUE_SEND(xHandle, xRoleta[0], (void *)(&wiadomosc), 0, &xResult); 
        }
        else if (rezultat == 3)
        {
          crQUEUE_SEND(xHandle, xRoleta[1], (void *)(&wiadomosc), 0, &xResult); 
        }
        else if (rezultat == 4)
        {
          //SYNC
          crc = 0;
          uint8_t temp;
          
          //Dane 
          for (temp = 0; temp < 11; temp++)
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
        Led1Off();
        Led2Off();
        stan = s_sync;
      }
      else //Zły adres
      {
        if (kodRozkazu == rFLASH)
        {
          DISABLE_RX();
          Led1On();
          Led2On();
          //TODO disable RX buffer
          crDELAY(xHandle, 1000);
          ENABLE_RX();
        }
        Led1Off();
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
//  Led2Toggle();
  crQUEUE_SEND_FROM_ISR( xRxedChars, &cChar, pdFALSE );
}
/*-----------------------------------------------------------*/

ISR(USART_UDRE_vect)
{
  signed portCHAR cChar, cTaskWoken;

  if( xQueueReceiveFromISR( xCharsForTx, &cChar, &cTaskWoken ) == pdTRUE )
  {
    /* Send the next character queued for Tx. */
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
      xHigherPriorityTaskWoken = pdFALSE;
//      xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken);
  }
}
