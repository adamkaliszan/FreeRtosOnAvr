#include <stdlib.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
#include "hardware.h"

#define debug 1

/*-----------------------------------------------------------*/

void initQueueStreamUSB(FILE *stream)
{
  fdev_setup_stream(stream, VtyPutChar, VtyGetChar, _FDEV_SETUP_RW);
  fdev_set_udata(stream, NULL);
  return;
}

int VtyGetChar(FILE *stream)
{
  (void) stream;
  uint8_t c;
  if (xQueueReceive(xVtyRec, &c, portMAX_DELAY) == 0)
    return EOF;
  return c;
}

int VtyPutChar(char c, FILE *stream)
{
  (void) stream;
  uartVtySendByte(c);
  return 0;
}

void xSerialPortInitMinimal(void)
{ ///500 000 @ 16MHz
  portENTER_CRITICAL();
  {
    xVtyRec = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xVtyTx = xQueueCreate(16, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
  }
  portEXIT_CRITICAL();

  UBRR0L = 1;
  UBRR0H = 0;

  UCSR0B = ((1<<RXCIE0)|(1<<TXEN0)|(1<<RXEN0));
  UCSR0C = ( serUCSRC_SELECT | serEIGHT_DATA_BITS );     /* Set the data bits to 8. */
  return;
}

/*-----------------------------------------------------------*/
ISR(USART_RX_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  signed portCHAR cChar;

  cChar = UDR0;
//  xQueueSendFromISR(xVtyRec, &cChar, NULL);

  xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xVtyRec, &cChar, &xHigherPriorityTaskWoken);
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

ISR(USART_UDRE_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  static char data;
  if(xQueueReceiveFromISR(xVtyTx, (void *)(&data), &xHigherPriorityTaskWoken) == pdTRUE)
  {
    UDR0 = data;
  }
  else
  {
    xHigherPriorityTaskWoken = pdFALSE;
    vInterruptVtyOff();
  }
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

void uartVtySendByte(uint8_t data)
{
  xQueueSend(xVtyTx, &data, portMAX_DELAY);
  vInterruptVtyOn();
}

void InterruptVtyOn(void)
{
  unsigned portCHAR ucByte;
  ucByte = UCSR0B;
  ucByte |= serDATA_INT_ENABLE;
  UCSR0B = ucByte;
}
