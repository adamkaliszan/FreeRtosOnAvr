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


/**
 * Konfiguracja portu szeregowego
 */
void xSerialPortInitMinimal(unsigned portBASE_TYPE uxQueueLength )
{
  portENTER_CRITICAL();
  {
    /**
     * Utworzenie buforów, które służą do przesyłania wiadomości pomiędzy korutynami a portem szeregowym.
     */
    xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
    xCharsForTx = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );

    /**
     * Konfiguracja pracy portu szeregowego
     */
    UBRR0L = 3;            /// Szybkość transmisji 115 kb/s
    UBRR0H = 0;            /// wartość rejestru zależy od taktowania procesora

    UCSR0C = ( serUCSRC_SELECT | serEIGHT_DATA_BITS );    /// Długość przesyłanego słowa: 8 bitów

    /**
     * Włączenie obsługi przerwań portu szeregowego:
     * - odebrano wiadomość
     * - zakończono wysyłanie.
     * Włączenie Nadajnika i odbiornika portu szeregowego.
     * Uwaga: Włączoy nadajnik nie wystarczy do rozpoczęcia transmisji.
     * Musi on dodatkowo zostać podłączony do magistrali RS 485.
     */
    UCSR0B = ((1<<RXCIE0)|(1<<TXCIE0)|(1<<TXEN0)|(1<<RXEN0));
  }
  portEXIT_CRITICAL();
  return;
}

/**
 * Obsługa przerwania "Odebrano znak"
 *
 * To przerwanie wywoływane jest, gdy zostanie odebrany znak przez port szeregowy
 * i umieszczony jest on w sprzętowym buforze odbiorczym.
 **/
ISR(USART_RX_vect)
{
  signed portCHAR tempToRx;
  tempToRx = UDR0;     /// Odczyt odebranego bajtu ze sprzętowego bufora.

  /**
   * Umieszczenie odebranej wiadomości w buforze odbiorczym
   * Uwaga: do wysyłania i odbierania wiadomości z buforów przez funkcje obsługujące przerwania
   * służą osobne makra: crQUEUE_SEND_FROM_ISR i xQueueReceiveFromISR.
   */
  crQUEUE_SEND_FROM_ISR( xRxedChars, &tempToRx, pdFALSE );
}

/**
 * Obsługa przerwania "Miejsce w sprzętowym buforze nadawczym"
 *
 * To przerwanie wywoływane jest wtedy, gdy w sprzętowym buforze nadawczym
 * portu szeregowego jest miejsce na kolejny bajt.
 * Obsługa przerwania polega na podbraniu kolejnego bajtu z bufora cyklicznego
 * i umieszczeniu go w sprzętowym buforze nadawczym portu szeregowego.
 */
ISR(USART_UDRE_vect)
{
  signed portCHAR tempToSend;

  if( xQueueReceiveFromISR(xCharsForTx, &tempToSend, NULL) == pdTRUE )
  {                    /// Odczytana wiadomość zapisane jest w zmiennej tempToSend
    UDR0 = tempToSend; /// Umieszczenie bajtu z danymi w sprzętowym buforze nadawczym
  }
  else                 /// Bufor cykliczny jest pusty
  {                    /// Nie ma kolejnych danych, jakie można umieścić w SPRZĘTOWYM buforze
    vInterruptOff();   /// nadawczym portu szeregowego, zatem wyłączona została obsługa
  }                    /// przerwania pusty bufor nadawczym.
}                      /// W przeciwnym wypadku cały czas byłoby ono wykonywane


/**
 * Obsługa przerwania "wysłano bajt"
 *
 * To przerwanie wywoływane jest wtedy, gdy wszystkie dane z bufora UDR0 zostały wysłane.
 * Po wysłaniu wszystkich danych można odłączyć nadajnik od magistrali RS 485.
 */
ISR(USART_TX_vect)
{                      /// Wyłączenie nadajnika.
  TxStop();            /// Mimo braku transmisji, włączony
}                      /// nadajnik zakłóca działanie magistrali RS485
