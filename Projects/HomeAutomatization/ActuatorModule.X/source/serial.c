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
#include "protocol_TlvRs485.h"

//#define debug 1

/*-----------------------------------------------------------*/

static xQueueHandle xRxedChars;
static xQueueHandle xCharsForTx;


static volatile uint8_t opCode;
static volatile uint8_t dlDanych;
static uint8_t bDane[MAX_DATA_LEN];

static uint8_t msgCode;


//static xSemaphoreHandle xSemaphore;
//static portBASE_TYPE xHigherPriorityTaskWoken; 

static uint8_t crcLo;
static uint8_t crcHi;
static uint16_t crc;

static uint8_t executeCommand(void)
{
  uint8_t doSendData   = 0;

  switch (opCode)
  {
    case rRoller1Down:
      msgCode = 0x3F;
      doSendData = 2;
      break;

    case rRoller2Down:
      msgCode = 0x3F;
      doSendData = 3;
      break;

    case rRoller1Up:
      msgCode = 0xBF;
      doSendData = 2;
      break;

    case rRoller2Up:
      msgCode = 0xBF;
      doSendData = 3;
      break;

    case rRoller1Stop:
      msgCode = 0x40;
      doSendData = 2;
      break;

    case rRoller2Stop:
      msgCode = 0x40;
      doSendData = 3;
      break;

    case rPING:
      doSendData = 1;
      break;
    case rHELLO:
      doSendData = 4;
      break;
    case rFLASH:
      doSendData = 1;
      break;
  }
  return doSendData;
}

void vProtocol(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  (void) uxIndex;

  crSTART( xHandle );
  static t_serialState   state;
  static uint8_t         charracter;
  static portBASE_TYPE   xResult;
  static uint8_t         corectAddress;
  static uint8_t         numberOfReceivedData;
  static uint8_t         result;
  state = s_sync;
  
  static uint8_t temp;

  for( ;; )
  {
      
    if (state == s_sync)
    {
      charracter=0;
      crQUEUE_RECEIVE(xHandle, xRxedChars, &charracter, portMAX_DELAY, &xResult);
      if ((xResult == pdPASS) && (charracter == SYNC))
      {
        state = s_addr;
        crc = _crc_xmodem_update(0, charracter);  	//TODO tutaj calculate value and assign without calling function�
      }
    }
    if (state == s_addr)
    {
      crQUEUE_RECEIVE(xHandle, xRxedChars, &charracter, 1, &xResult);
      if (xResult == pdPASS)
      {
        state = s_command;
        crc = _crc_xmodem_update(crc, charracter);
        if (charracter == address)
          corectAddress = 1;
        else
          corectAddress = 0;
      }
      else
      {
        state = s_sync;
      }
    }
    if (state == s_command)
    {
      Led1On();
      Led2Off();
      crQUEUE_RECEIVE(xHandle, xRxedChars, (void *)(&opCode), 1, &xResult);
      if (xResult == pdPASS)
      {
        crc = _crc_xmodem_update(crc, opCode);
        state = s_len;
      }
      else
      {
        state = s_sync;
      }
    }
    if (state == s_len)
    {
      Led1Off();
      Led2On();
      crQUEUE_RECEIVE(xHandle, xRxedChars, (void *)(&dlDanych), 1, &xResult);
      if (xResult == pdPASS)
      {
        crc = _crc_xmodem_update(crc, dlDanych);
        numberOfReceivedData = 0;
        state = s_data;
        Led1On();
        Led2On();
      }
      else
      {
        state = s_sync;
      }
    }
    if (state == s_data)
    {
      if (numberOfReceivedData == dlDanych)
      {
        state = s_CRC_HI;
      }
      else
      {
        //Led2Off();
        crQUEUE_RECEIVE(xHandle, xRxedChars, &charracter, 1, &xResult);
        if (xResult == pdPASS)
        {
          crc = _crc_xmodem_update(crc, charracter);
          if (numberOfReceivedData < MAX_DATA_LEN)
            bDane[numberOfReceivedData] = charracter;
          numberOfReceivedData++; 
        }
        else
        {
          Led1Off();
          state = s_sync;
        }
      }
    }
    if (state == s_CRC_HI)
    {
      crQUEUE_RECEIVE(xHandle, xRxedChars, (void *)(&crcHi), 1, &xResult);
      if (xResult == pdPASS)
      {
        state = s_CRC_LO;
      }
      else
      {
        Led1Off();
        state = s_sync;
      }    
    }
    if (state == s_CRC_LO)
    {
      crQUEUE_RECEIVE(xHandle, xRxedChars, (void *)(&crcLo), 1, &xResult);
      if (xResult == pdPASS)
      {
        if ((crcHi != (uint8_t)(crc >> 8)) || (crcLo != (uint8_t)(crc & 0xFF)))
        {
          Led1Off();
          state = s_sync;
        }
        else
        {
          state = s_CRC_OK;
        }
      }
    }
    if (state == s_CRC_OK)
    {
      if (corectAddress == 1)
      {
        if (numberOfReceivedData > MAX_DATA_LEN)
          numberOfReceivedData = MAX_DATA_LEN;
        result = executeCommand();
        if (result == 1)
        {
          //SYNC
          temp = SYNC;
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&temp), 0, &xResult);
          crc = _crc_xmodem_update(0, temp);

         //ADRES 0x00 adres mastera
          temp = 0x00;
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&temp), 0, &xResult);
          crc = _crc_xmodem_update(crc, temp);
            
          //Rozkaz
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&opCode), 0, &xResult);
          crc = _crc_xmodem_update(crc, opCode);
          
          //Długość danych
          crQUEUE_SEND(xHandle, xCharsForTx, (void *)(&numberOfReceivedData), 0, &xResult);
          crc = _crc_xmodem_update(crc, numberOfReceivedData);
          
          //Dane
          for (temp = 0; temp < numberOfReceivedData; temp++)
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
          vInterruptOn();  //In case of error disable the transmitter and flush the buffer
        
          if (opCode == rFLASH)
          {
            Led1On();
            Led2On();
            crDELAY(xHandle, 10);
            Led1Off();
            Led2Off();
            (*((void(*)(void))BOOT_START))();            //reboot
          }
        }
        else if (result == 2)
        {
          crQUEUE_SEND(xHandle, xRollersQueue[0], (void *)(&msgCode), 0, &xResult); 
        }
        else if (result == 3)
        {
          crQUEUE_SEND(xHandle, xRollersQueue[1], (void *)(&msgCode), 0, &xResult); 
        }
        else if (result == 4)
        {
          //SYNC
          crc = 0;
          
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
          vInterruptOn();  //In case of error disable the transmitter and flush the buffer
        }
        Led1Off();
        Led2Off();
        state = s_sync;
      }
      else /// Wrong address
      {
        if (opCode == rFLASH)
        {
          DISABLE_RX();
          Led1On();
          Led2On();
          // TODO disable RX buffer
          crDELAY(xHandle, 1000);
          ENABLE_RX();
        }
        Led1Off();
        state = s_sync;
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
    
//    vSemaphoreCreateBinary( xSemaphore );
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
//      xHigherPriorityTaskWoken = pdFALSE;
//      xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken);
  }
}
