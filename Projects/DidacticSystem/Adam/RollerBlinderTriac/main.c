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

	1 tab == 4 spaces!

	Please ensure to read the configuration and relevant port sections of the
	online documentation.

	http://www.FreeRTOS.org - Documentation, latest information, license and
	contact details.

	http://www.SafeRTOS.com - A version that is certified for use in safety
	critical systems.

	http://www.OpenRTOS.com - Commercial support, development, porting,
	licensing and training services.
*/

#include "main.h"
#include "../../freeRtos/Lib/include/protocol1.h"
#include <avr/eeprom.h>
#include "hardware.h"


/**
 * Proces odpowiedzialny za obsługę klawiszy
 * @param pvParameters ignorowane parametry
 */
static void vKlawisze(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex);

/**
 * Proces odpowiedzialny za obsługę rolety
 * @param pvParameters ignorowane parametry
 */
static void vRoleta(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex);

#if DO_INCR_RST
static void prvIncrementResetCount( void )
{
  unsigned portCHAR ucCount;
  eeprom_read_block( &ucCount, mainRESET_COUNT_ADDRESS, sizeof( ucCount ) );
  ucCount++;
  eeprom_write_byte( mainRESET_COUNT_ADDRESS, ucCount );
}
#endif

void vApplicationIdleHook( void );


/*-----------------------------------------------------------*/

/* Device address on RS 485 bus */

uint8_t settingsEep EEMEM = 0x88; 
uint8_t adres;
char bHelloResp[HELLO_RESP_LEN+HDR_LEN] = {SYNC, 0, rHELLO, HELLO_RESP_LEN, 0, 0, 0, 'v', '0', '.', '0', '2'};

t_stan_klawiszy	roleta[2] = {{0, 0, 0, 0, bezczynny}, {0, 0, 0, 0, bezczynny}};

extern xQueueHandle xRxedChars;
extern xQueueHandle xCharsForTx;
extern struct funkcje sterowanie[2];

xQueueHandle xRoleta[2];

portSHORT main( void )
{
#if DO_INCR_RST
  prvIncrementResetCount();
#endif
  hardwareInit();
  

  wczytajUstawienia(settings);
  settings = eeprom_read_byte(&settingsEep);
  
  POWER_ON
  
  xSerialPortInitMinimal(32);

  xRoleta[0] = xQueueCreate(4, 1);
#if X2
  xRoleta[1] = xQueueCreate(4, 1);
#endif
  xCoRoutineCreate(vProtocol, 0, 0);
  xCoRoutineCreate(vKlawisze, 0, 0);
  xCoRoutineCreate(vRoleta, 0, 0);
#if X2
  xCoRoutineCreate(vRoleta, 0, 1);
#endif
  vTaskStartScheduler();
  return 0;
}
/*-----------------------------------------------------------*/

static void vKlawisze(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  (void) uxIndex;
  static portBASE_TYPE xResult;

  crSTART( xHandle );
  for( ;; )
  {
    crDELAY( xHandle, 1);
    uint8_t wiadomosc;
    wiadomosc = (uint8_t) (automatStanowKlawiszy(czytKlawiszRol1wGore(), czytKlawiszRol1wDol(), &roleta[0]));
    if (wiadomosc)
    {
      crQUEUE_SEND(xHandle, xRoleta[0], &wiadomosc, 10, &xResult);
    }
#if X2
    wiadomosc = (uint8_t) (automatStanowKlawiszy(czytKlawiszRol2wGore(), czytKlawiszRol2wDol(), &roleta[1]));
    if (wiadomosc)
    {
      crQUEUE_SEND(xHandle, xRoleta[1], &wiadomosc, 10, &xResult);
    }
#endif
  }
  crEND();
}

static void vRoleta(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  static uint8_t       rozkaz;
  static uint16_t      czasAkcji;
  static portBASE_TYPE xResult;
  czasAkcji = portMAX_DELAY;

  crSTART( xHandle );
  for (;;)
  {
    crQUEUE_RECEIVE(xHandle, xRoleta[uxIndex], &rozkaz, czasAkcji, &xResult);

    if (xResult == pdTRUE)
    {
      uint8_t tmp = rozkaz & 0x3F;
      if (tmp == 0)
        czasAkcji = portMAX_DELAY;
      else
        czasAkcji = tmp*20;
      if (rozkaz & 0x40)
      {
        sterowanie[uxIndex].stop();
      }
      else
      {
        sterowanie[uxIndex].stop();
        
        bHelloResp[HDR_LEN+2]
        crDELAY(xHandle, 10);
        bHelloResp[HDR_LEN+uxIndex] &= 0x3F;

        if (rozkaz & 0x80)
        {
          bHelloResp[HDR_LEN+uxIndex] |= 0x80;
          sterowanie[uxIndex].gora();
        }
        else
        {
          bHelloResp[HDR_LEN+uxIndex] |= 0x40;
          sterowanie[uxIndex].dol();
        }
      }
    }
    else
    {
      bHelloResp[HDR_LEN+uxIndex] &= 0x3F;
      czasAkcji = portMAX_DELAY;
      sterowanie[uxIndex].stop();
    }
  }
  crEND();
}

void vApplicationIdleHook( void )
{
  for( ;; )
  {
    vCoRoutineSchedule();
  }
}
