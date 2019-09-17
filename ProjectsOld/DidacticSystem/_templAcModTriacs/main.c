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

static void prvIncrementResetCount( void );

void vApplicationIdleHook( void );


/*-----------------------------------------------------------*/

/* Device address on RS 485 bus */
uint8_t adres;
char bHelloResp[HELLO_RESP_LEN+HDR_LEN] = {SYNC, 0, rHELLO, HELLO_RESP_LEN, 'r', 0, 'v', '0', '.', '5', '1'};

t_stan_klawiszy	roleta1 = {0, 0, 0, 0, bezczynny};
t_stan_klawiszy	roleta2 = {0, 0, 0, 0, bezczynny};

extern xQueueHandle xRxedChars;
extern xQueueHandle xCharsForTx; 

xQueueHandle xRoleta[2];

portSHORT main( void )
{
//prvIncrementResetCount();

  hardwareInit();
  xSerialPortInitMinimal(16);

  xRoleta[0] = xQueueCreate(4, 1);
  xRoleta[1] = xQueueCreate(4, 1);

  xCoRoutineCreate(vProtocol, 0, 0);
  xCoRoutineCreate(vKlawisze, 0, 0);
  xCoRoutineCreate(vRoleta, 0, 0);
  xCoRoutineCreate(vRoleta, 0, 1);

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
    wiadomosc = (uint8_t) (automatStanowKlawiszy(czytKlawiszRol1wGore(), czytKlawiszRol1wDol(), &roleta1));
    if (wiadomosc)
    {
      crQUEUE_SEND(xHandle, xRoleta[0], &wiadomosc, 10, &xResult);
    }
    wiadomosc = (uint8_t)(automatStanowKlawiszy(czytKlawiszRol2wGore(), czytKlawiszRol2wDol(), &roleta2));
    if (wiadomosc)
    {
      crQUEUE_SEND(xHandle, xRoleta[1], &wiadomosc, 10, &xResult);
    }
  }
  crEND();
}

static void vRoleta(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  static uint8_t	rozkaz[2];
  static uint16_t	czasAkcji[2];
  czasAkcji[uxIndex]  = portMAX_DELAY;
  static portBASE_TYPE xResult[2];
  crSTART( xHandle );
  for (;;)
  {
    crQUEUE_RECEIVE(xHandle, xRoleta[uxIndex], &rozkaz[uxIndex], czasAkcji[uxIndex], &xResult[uxIndex]);

    if (xResult[uxIndex] == pdTRUE)
    {
      uint8_t tmp = rozkaz[uxIndex] & 0x3F;
      if (tmp == 0)
        czasAkcji[uxIndex] = portMAX_DELAY;
      else
        czasAkcji[uxIndex] = tmp*20;
      if (rozkaz[uxIndex] & 0x40)
      {
        roletaStop(uxIndex);
      }
      else
      {
        if (rozkaz[uxIndex] & 0x80)
          roletawGore(uxIndex);
        else
          roletawDol(uxIndex);
      }
    }
    else
    {
      czasAkcji[uxIndex] = portMAX_DELAY;
      roletaStop(uxIndex);
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

#if 0
static void prvIncrementResetCount( void )
{
	unsigned portCHAR ucCount;
	eeprom_read_block( &ucCount, mainRESET_COUNT_ADDRESS, sizeof( ucCount ) );
	ucCount++;
	eeprom_write_byte( mainRESET_COUNT_ADDRESS, ucCount );
}
#endif