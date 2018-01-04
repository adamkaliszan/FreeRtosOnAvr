#include <stdlib.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
#include "hardware.h"

#define debug 1

/*-----------------------------------------------------------*/

void initQueueRs1Stream(FILE *stream)
{
  fdev_setup_stream(stream, Rs1PutChar, Rs1GetChar, _FDEV_SETUP_RW);
  fdev_set_udata(stream, NULL);
  return;
}

int Rs1GetChar(FILE *stream)
{
  (void) stream;
  uint8_t c;
  if (xQueueReceive(xRs1Rec, &c, portMAX_DELAY) == 0)
    return EOF;
  return c;
}

int Rs1PutChar(char c, FILE *stream)
{
  (void) stream;
  uartRs1SendByte(c);
  return 0;
}

/*
void initQueueRsLanStream(FILE *stream)
{
  fdev_setup_stream(stream, RsLanPutChar, RsLanGetChar, _FDEV_SETUP_RW);
  fdev_set_udata(stream, NULL);
  return;
}

int RsLanGetChar(FILE *stream)
{
  (void) stream;
  uint8_t c;
  if (xQueueReceive(xRsLanRec, &c, portMAX_DELAY) == 0)
    return EOF;
  return c;
}

int RsLanPutChar(char c, FILE *stream)
{
  (void) stream;
  uartRsLanSendByte(c);
  return 0;
}
*/

void xSerialPortInitMinimal(void)
{
  portENTER_CRITICAL();
  {
    xRs1Rec = xQueueCreate(64, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xRs1Tx = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xRsLanRec = xQueueCreate( 64, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
    xRsLanTx = xQueueCreate( 32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
  }
  portEXIT_CRITICAL();

  UBRR0L = 8;
  UBRR0H = 0;

  UBRR1L = 8;
  UBRR1H = 0;

//UCSR0B = ((1<<RXCIE0)|(1<<TXEN0)|(1<<RXEN0));
//UCSR0C = ( serUCSRC_SELECT | serEIGHT_DATA_BITS );     /* Set the data bits to 8. */

  //Serial VTY
  UCSR1B = ((1<<RXCIE1)|(1<<TXEN1)|(1<<RXEN1));
  UCSR1C = ( serUCSRC_SELECT | serEIGHT_DATA_BITS );     /* Set the data bits to 8. */

  //UDR0='a';
  //UDR0='A';

  UDR1='\n';
  UDR1='1';

  return;
}

/*-----------------------------------------------------------*/


void uartRsLanSendByte(uint8_t data)
{
  xQueueSend(xRsLanTx, &data, portMAX_DELAY);
  vInterruptRsLanOn();
}


void uartRs1SendByte(uint8_t data)
{
  xQueueSend(xRs1Tx, &data, portMAX_DELAY);
  InterruptRs1On();
}

void InterruptRs1On(void)
{
  unsigned portCHAR ucByte;
  ucByte = UCSR1B;
  ucByte |= serDATA_INT_ENABLE;
  UCSR1B = ucByte;
}

void vInterruptRsLanOn()
{
  unsigned portCHAR ucByte;
  ucByte = UCSR0B;
  ucByte |= serDATA_INT_ENABLE;
  UCSR0B = ucByte;
}


void InterruptRsLanOff()
{
  unsigned portCHAR ucInByte;
  ucInByte = UCSR0B;
  ucInByte &= ~serDATA_INT_ENABLE;
  UCSR0B = ucInByte;
}

void InterruptRs1Off()
{
  unsigned portCHAR ucInByte;
  ucInByte = UCSR1B;
  ucInByte &= ~serDATA_INT_ENABLE;
  UCSR1B = ucInByte;
}

/*-----------------------------------------------------------*/

ISR(USART0_RX_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
  signed portCHAR cChar;

  cChar = UDR0;

  xHigherPriorityTaskWoken = pdFALSE;

  xQueueSendFromISR( xRsLanRec, &cChar, &xHigherPriorityTaskWoken );

  if( xHigherPriorityTaskWoken )
    taskYIELD();
}

ISR(USART0_UDRE_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  static char data;
  if(xQueueReceiveFromISR(xRsLanTx, &data, &xHigherPriorityTaskWoken) == pdTRUE)
  {
    UDR0 = data;
  }
  else
  {
    xHigherPriorityTaskWoken = pdFALSE;
    InterruptRsLanOff();
  }
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

ISR(USART1_RX_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
  signed portCHAR cChar;

  cChar = UDR1;

  xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xRs1Rec, &cChar, &xHigherPriorityTaskWoken);

  if( xHigherPriorityTaskWoken )
    taskYIELD();
}

ISR(USART1_UDRE_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  static char data;
  if(xQueueReceiveFromISR(xRs1Tx, &data, &xHigherPriorityTaskWoken) == pdTRUE)
  {
    UDR1 = data;
  }
  else
  {
    xHigherPriorityTaskWoken = pdFALSE;
    InterruptRs1Off();
  }
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}
