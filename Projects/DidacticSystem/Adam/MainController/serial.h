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


// ******************************* Serial 0 RS485 *********************

/*
 * Włączenie przerwania pusty bufor nadawczy dla magistrali Rs485
 */
#define vInterruptRs485On()        \
{                                  \
  unsigned portCHAR ucByte;        \
                                   \
  ucByte = UCSR0B;                 \
  ucByte |= serDATA_INT_ENABLE;    \
  UCSR0B = ucByte;                 \
}

#define vIsInterruptRs485On()  (UCSR0B & serDATA_INT_ENABLE)

/*
 * Wyłączenie przerwania pusty bufor nadawczy dla magistrali Rs485
 */
#define vInterruptRs485Off()       \
{                                  \
  unsigned portCHAR ucInByte;      \
                                   \
  ucInByte = UCSR0B;               \
  ucInByte &= ~serDATA_INT_ENABLE; \
  UCSR0B = ucInByte;               \
}


// ******************************* Serial 1 USB ***********************

/*
 * Włączenie przerwania pusty bufor nadawczy dla VTY
 */

#define vIsInterruptVtyOn()  (UCSR1B & serDATA_INT_ENABLE)

/*
 * Wyłączenie przerwania pusty bufor nadawczy dla VTY
 */
#define vInterruptVtyOff()            \
{                                  \
  unsigned portCHAR ucInByte;      \
                                   \
  ucInByte = UCSR1B;               \
  ucInByte &= ~serDATA_INT_ENABLE; \
  UCSR1B = ucInByte;               \
}

#define vInterruptVtyOn()             \
{                                  \
  unsigned portCHAR ucByte;        \
                                   \
  ucByte = UCSR1B;                 \
  ucByte |= serDATA_INT_ENABLE;    \
  UCSR1B = ucByte;                 \
}

/**
 * Serial 1 (VTY) receiver que
 */
extern xQueueHandle         xVtyRec;
extern xQueueHandle         xVtyTx;

/**
 * Serial 0 (Rs485) receiver que
 */
extern xQueueHandle         xRs485Rec;
extern xQueueHandle         xRs485Tx;

xSemaphoreHandle            xSemaphoreRs485;    /// Flaga blokująca jednoczesny dostęp do magistrali wielu zadaniom


void initQueueStreamUSB(FILE *stream);

int     VtyPutChar(char c, FILE *stream);
int     VtyGetChar(FILE *stream);

void    xSerialPortInitMinimal(void);
void    uartVtySendByte(uint8_t data);

void    uartRs485SendByte(uint8_t data);
uint8_t rs485Receive(uint8_t *c, uint8_t timeout);
uint8_t flushRs485RecBuffer(void);
void    takeRs485(void);
void    releaseRs485(void);

void    InterruptVtyOn(void);


#endif
