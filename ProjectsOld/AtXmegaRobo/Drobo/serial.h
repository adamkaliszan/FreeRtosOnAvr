#ifndef SERIAL_H
#define SERIAL_H

#include "main.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "hardware.h"

/* Constants for writing to UCSRB. */
#define serRX_INT_ENABLE        ( ( unsigned portCHAR ) 0x80 )
#define serTX_INT_ENABLE        ( ( unsigned portCHAR ) 0x40 )
#define serDATA_INT_ENABLE      ( ( unsigned portCHAR ) 0x20 )


#define serRX_ENABLE            ( ( unsigned portCHAR ) 0x10 )
#define serTX_ENABLE            ( ( unsigned portCHAR ) 0x08 )

/* Constants for writing to UCSRC. */
#define serUCSRC_SELECT         ( ( unsigned portCHAR ) 0x80 )
#define serEIGHT_DATA_BITS      ( ( unsigned portCHAR ) 0x06 )


// ******************************* Serial USB ***********************

/*
 * Włączenie przerwania pusty bufor nadawczy dla VTY
 */

#define vIsInterruptVtyOn()  (USARTC0.CTRLA & USART_DREINTLVL_LO_gc)

/*
 * Wyłączenie przerwania pusty bufor nadawczy dla VTY
 */

#define vInterruptVtyOff()            \
{                                     \
  unsigned portCHAR ucInByte;         \
                                      \
  ucInByte = USARTC0.CTRLA;           \
  ucInByte &= ~USART_DREINTLVL_LO_gc; \
  USARTC0.CTRLA = ucInByte;           \
}

#define vInterruptVtyOn()             \
{                                     \
  unsigned portCHAR ucByte;           \
                                      \
  ucByte = USARTC0.CTRLA;             \
  ucByte |= USART_DREINTLVL_LO_gc;    \
  USARTC0.CTRLA = ucByte;             \
}

// ******************************* Serial HC12

/*
 * Włączenie przerwania pusty bufor nadawczy dla HC12
 */

#define vIsInterruptHC12On()  (USARTC1.CTRLA & USART_DREINTLVL_LO_gc)

/*
 * Wyłączenie przerwania pusty bufor nadawczy dla HC12
 */

#define vInterruptHC12Off()            \
{                                     \
  unsigned portCHAR ucInByte;         \
                                      \
  ucInByte = USARTC1.CTRLA;           \
  ucInByte &= ~USART_DREINTLVL_LO_gc; \
  USARTC1.CTRLA = ucInByte;           \
}

#define vInterruptHC12On()             \
{                                     \
  unsigned portCHAR ucByte;           \
                                      \
  ucByte = USARTC1.CTRLA;             \
  ucByte |= USART_DREINTLVL_LO_gc;    \
  USARTC1.CTRLA = ucByte;             \
}

#define vInterruptSIM900Off()            \
{                                     \
  unsigned portCHAR ucInByte;         \
                                      \
  ucInByte = USARTD0.CTRLA;           \
  ucInByte &= ~USART_DREINTLVL_LO_gc; \
  USARTD0.CTRLA = ucInByte;           \
}

#define vInterruptSIM900On()             \
{                                     \
  unsigned portCHAR ucByte;           \
                                      \
  ucByte = USARTD0.CTRLA;             \
  ucByte |= USART_DREINTLVL_LO_gc;    \
  USARTD0.CTRLA = ucByte;             \
}

extern xQueueHandle         xVtyRec;
extern xQueueHandle         xVtyTx;
extern xQueueHandle         xHC12Rec;
extern xQueueHandle         xHC12Tx;
extern xQueueHandle         xSIM900Rec;
extern xQueueHandle         xSIM900Tx;

// ******************************* RS485 *********************

/*********************************************/

void         initQueueStreamUSB(FILE *stream);
void         initQueueStreamHC12(FILE *stream);
void         initQueueStreamHC12fake(FILE *stream);

int          VtyPutChar(char c, FILE *stream);
int          VtyGetChar(FILE *stream);

int          HC12PutChar(char c, FILE *stream);
int          HC12PutCharFake(char c, FILE *stream);
int          HC12GetChar(FILE *stream);

int          SIM900PutChar(char c, FILE *stream);
int          SIM900GetChar(FILE *stream);

void         uartVtySendByte(uint8_t data);
void         uartHC12SendByte(uint8_t data);
void         uartHC12SendByteFake(uint8_t data);
void         uartSIM900SendByte(uint8_t data);

void         xSerialPortInitMinimal(void);

#endif
