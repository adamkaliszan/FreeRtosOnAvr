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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/crc16.h>


#include "FreeRTOS.h"
//#include "croutine.h"
#include "queue.h"
#include "task.h"
#include "queueStream.h"
#include "cli_task.h"
#include "loop_task.h"
#include "serial.h"

#include "hardwareConfig.h"
#include "softwareConfig.h"
#include "hardware.h"

#include "cmdline.h"
#include "memory_x.h"
#include "vty.h"


#include "FreeRTOS.h"

#include "main.h"


xTaskHandle   xHandleVTY;
xTaskHandle   xHandleLoop;

xQueueHandle  xVtyTx;
xQueueHandle  xVtyRec;
CliState_t    cliStateUSB;
FILE          usbStream;
uint8_t       timer100Hz = 0;


void vApplicationIdleHook( void );
void vApplicationTickHook( void );

portSHORT main( void )
{
  hardwareInit();

  xSerialPortInitMinimal();
  loadConfiguration();

  initQueueStreamUSB(&usbStream);
  VtyInit(&cliStateUSB, &usbStream);

  xTaskCreate(vTaskVTY,    NULL /*""    */, STACK_SIZE_VTY,       (void *)(&cliStateUSB),            1, &xHandleVTY);
  xTaskCreate(vTaskLoop,   NULL /*""    */, 0x80,                 NULL,                              1, &xHandleLoop);

  vTaskStartScheduler();
  return 0;
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
  for( ;; )
  {
    ;//vCoRoutineSchedule();
  }
}

void vApplicationTickHook( void )
{
  static uint8_t tickCntr = configTICK_RATE_HZ;
  if (--tickCntr == 0)
  {
    tickCntr = configTICK_RATE_HZ;
  }
}
