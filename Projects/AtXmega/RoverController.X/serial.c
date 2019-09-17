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

void initQueueStreamHC12(FILE *stream)
{
  fdev_setup_stream(stream, HC12PutChar, HC12GetChar, _FDEV_SETUP_RW);
  fdev_set_udata(stream, NULL);
  return;
}

void initQueueStreamHC12fake(FILE *stream)
{
  fdev_setup_stream(stream, HC12PutCharFake, HC12GetChar, _FDEV_SETUP_RW);
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

int HC12GetChar(FILE *stream)
{
  (void) stream;
  uint8_t c;
  if (xQueueReceive(xHC12Rec, &c, portMAX_DELAY) == 0)
    return EOF;
  return c;
}

int HC12PutChar(char c, FILE *stream)
{
  (void) stream;
  uartHC12SendByte(c);
  return 0;
}

int SIM900GetChar(FILE *stream)
{
  (void) stream;
  uint8_t c;
  if (xQueueReceive(xSIM900Rec, &c, portMAX_DELAY) == 0)
    return EOF;
  return c;
}

int SIM900PutChar(char c, FILE *stream)
{
  (void) stream;
  uartSIM900SendByte(c);
  return 0;
}


int HC12PutCharFake(char c, FILE *stream)
{
  (void) stream;
  uartHC12SendByteFake(c);
  return 0;
}

//#include<avr/iox128a1.h>
void xSerialPortInitMinimal(void)
{
  ///VTY             - USART C0
  USARTC0.CTRLA = USART_RXCINTLVL_LO_gc;// | USART_DREINTLVL_LO_gc;                   // Włączenie przerwań Odebrano. By włączyć przerwanie pusty bufor nadawczy dodać: USART_DREINTLVL_LO_gc
  USARTC0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;           // Włączenie nadajnika i odbiornika
  USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;                    // Tryb 8 bitów
  // 115200 @ 32MHz
  USARTC0.BAUDCTRLA= 2094 & 0xFF;                          //BSEL = 2094  BSCALE = -7
  USARTC0.BAUDCTRLB= (-7 << USART_BSCALE0_bp)|(2094 >> 8);


  /// HC-12 / ZigBee - USART C1
  USARTC1.CTRLA = USART_RXCINTLVL_LO_gc;// | USART_DREINTLVL_LO_gc;                   // Włączenie przerwań Odebrano. By włączyć przerwanie pusty bufor nadawczy dodać: USART_DREINTLVL_LO_gc
  USARTC1.CTRLB = USART_RXEN_bm | USART_TXEN_bm;           // Włączenie nadajnika i odbiornika
  USARTC1.CTRLC = USART_CHSIZE_8BIT_gc;                    // Tryb 8 bitów
  // 9600 @ 32MHz
  USARTC1.BAUDCTRLA= 3317 & 0xFF;                          //BSEL = 3317  BSCALE = -4
  USARTC1.BAUDCTRLB= (-4 << USART_BSCALE0_bp)|(3317 >> 8);

  ///SIM900          - USART D0
  USARTD0.CTRLA = USART_RXCINTLVL_LO_gc;// | USART_DREINTLVL_LO_gc;      // Włączenie przerwań Odebrano. By włączyć przerwanie pusty bufor nadawczy dodać: USART_DREINTLVL_LO_gc
  USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;           // Włączenie nadajnika i odbiornika
  USARTD0.CTRLC = USART_CHSIZE_8BIT_gc;                    // Tryb 8 bitów
  // 115200 @ 32MHz
  USARTD0.BAUDCTRLA= 2094 & 0xFF;                          //BSEL = 2094  BSCALE = -7
  USARTD0.BAUDCTRLB= (-7 << USART_BSCALE0_bp)|(2094 >> 8);


  portENTER_CRITICAL();
  {
    xVtyRec    = xQueueCreate(64, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xVtyTx     = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));

    xHC12Rec   = xQueueCreate(64, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xHC12Tx    = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));

    xSIM900Rec = xQueueCreate(128,( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    xSIM900Tx  = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
  }
  portEXIT_CRITICAL();
  return;
}


/** VTY ---------------------------------------------------*/
ISR(USARTC0_RXC_vect)
{

  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  signed portCHAR cChar;

  cChar = USARTC0.DATA;
  //return;
  //USARTC0.DATA = cChar+1;

  xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xVtyRec, &cChar, &xHigherPriorityTaskWoken);
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

ISR(USARTC0_DRE_vect) // USART1_UDRE_vect
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  static char data;
  if(xQueueReceiveFromISR(xVtyTx, &data, &xHigherPriorityTaskWoken) == pdTRUE)
  {
    USARTC0.DATA = data;
  }
  else
  {
    xHigherPriorityTaskWoken = pdFALSE;
    vInterruptVtyOff();
  }
  //if( xHigherPriorityTaskWoken )
  //{
  //  taskYIELD();
  //}
}


/** HC12 ************************************/

ISR(USARTC1_RXC_vect)
{

  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  signed portCHAR cChar;

  cChar = USARTC1.DATA;
  //return;
  //USARTC0.DATA = cChar+1;

  xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xHC12Rec, &cChar, &xHigherPriorityTaskWoken);
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

void uartHC12SendByte(uint8_t data)
{
  xQueueSend(xHC12Tx, &data, 10);
  vInterruptHC12On();
}

void uartHC12SendByteFake(uint8_t data)
{
  xQueueSend(xHC12Rec, &data, 0);
}

ISR(USARTC1_DRE_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  static char data;
  if(xQueueReceiveFromISR(xHC12Tx, &data, &xHigherPriorityTaskWoken) == pdTRUE)
  {
    USARTC1.DATA = data;
  }
  else
  {
    xHigherPriorityTaskWoken = pdFALSE;
    vInterruptHC12Off();
  }
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

/** SIM900 ************************************/

ISR(USARTD0_RXC_vect)
{

  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  signed portCHAR cChar;

  cChar = USARTD0.DATA;
  //return;
  //USARTC0.DATA = cChar+1;

  xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(xSIM900Rec, &cChar, &xHigherPriorityTaskWoken);
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}

void uartSIM900SendByte(uint8_t data)
{
  xQueueSend(xSIM900Tx, &data, 10);
  vInterruptSIM900On();
}

ISR(USARTD0_DRE_vect)
{
  static signed portBASE_TYPE xHigherPriorityTaskWoken;
  static char data;
  if(xQueueReceiveFromISR(xSIM900Tx, &data, &xHigherPriorityTaskWoken) == pdTRUE)
  {
    USARTD0.DATA = data;
  }
  else
  {
    xHigherPriorityTaskWoken = pdFALSE;
    vInterruptSIM900Off();
  }
  if( xHigherPriorityTaskWoken )
  {
    taskYIELD();
  }
}
