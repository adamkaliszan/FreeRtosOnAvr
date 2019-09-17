#ifndef SERIAL_H
#define SERIAL_H

#include "main.h"
#include "semphr.h"

extern xQueueHandle xRxedChars;       /// Bufor, który przechowuje odebrane znaki z magistrali RS 485
extern xQueueHandle xCharsForTx;      /// Bufor, który przechowuje znaki do wysłania przez magistralę RS 485

#define BOOT_START 0x1C00

/* Constants for writing to UCSRB. */
#define serRX_INT_ENABLE        ( ( unsigned portCHAR ) 0x80 )
#define serTX_INT_ENABLE        ( ( unsigned portCHAR ) 0x40 )
#define serDATA_INT_ENABLE      ( ( unsigned portCHAR ) 0x20 )


#define serRX_ENABLE            ( ( unsigned portCHAR ) 0x10 )
#define serTX_ENABLE            ( ( unsigned portCHAR ) 0x08 )

/* Constants for writing to UCSRC. */
#define serUCSRC_SELECT         ( ( unsigned portCHAR ) 0x80 )
#define serEIGHT_DATA_BITS      ( ( unsigned portCHAR ) 0x06 )

#define ENABLE_RX() (UCSR0B |= (1<<RXEN0))

#define DISABLE_RX() (UCSR0B &= ~(1<<RXEN0))


#define vInterruptOn()           \
{                                \
  unsigned portCHAR ucByte;      \
                                 \
  ucByte = UCSR0B;                \
  ucByte |= serDATA_INT_ENABLE;    \
  UCSR0B = ucByte;                \
}
/*-----------------------------------------------------------*/

#define vIsInterruptOn()  (UCSR0B & serDATA_INT_ENABLE)
/*-----------------------------------------------------------*/


#define vInterruptOff()            \
{                                  \
  unsigned portCHAR ucInByte;      \
                                   \
  ucInByte = UCSR0B;               \
  ucInByte &= ~serDATA_INT_ENABLE; \
  UCSR0B = ucInByte;               \
}

#define MAX_DATA_LEN 14



typedef enum
{
  s_sync,
  s_addr,
  s_rozkaz,
  s_len,
  s_dane,
  s_CRC_HI,
  s_CRC_LO,
  s_CRC_OK
} t_serialState;

/**
 * Proces odpowiedzialny za obsługę protokołu komunikacyjnego po RS485
 * @param pvParameters ignorowane parametry
 */
void vProtocol(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex);

void xSerialPortInitMinimal(uint8_t uxQueueLength);


uint8_t xSerialPutChar(uint8_t cOutChar, portTickType xBlockTime);

#endif

