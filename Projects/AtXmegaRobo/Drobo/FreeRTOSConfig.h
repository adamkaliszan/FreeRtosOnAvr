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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <avr/io.h>
//#include<avr_compiler.h>

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		0 //1 jakiego planisty wybieramy 1- preemptive  0 - cooperative (wspó³dzielony )

#define configUSE_IDLE_HOOK			0 //1

#define configUSE_TICK_HOOK			0 //0

#define configCPU_CLOCK_HZ			( ( unsigned long ) 32000000 )//2MHz is default value for xmega.//16MHz

//If you you want another frequency don't forget to modify period of timer counter used for tick interrupt

#define configTICK_RATE_HZ			( ( portTickType ) 100 )// czestotliwoæ RTOS przerwania tick, sluzy do odmierzania czasu

#define configMAX_PRIORITIES		( ( unsigned portBASE_TYPE ) 3 )//4 maksymalna liczba priorytetów obs³ugiwana przez zadanie

#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 150 )//85 // min rozmiar stosu, który wykorzystywany jest przez zadanie IDle

#define configTOTAL_HEAP_SIZE		( (size_t ) ( 7000 ) )//1500// całkowita ilość pamięci RAM dostępna dla jądra RTOS

#define configSUPPORT_DYNAMIC_ALLOCATION 1

#define configUSE_MUTEXES           1

#define configMAX_TASK_NAME_LEN		( 10 )

#define configUSE_TRACE_FACILITY	1 //Set to 1 if you wish to include additional structure members and functions to assist with execution visualisation and tracing

#define configUSE_16_BIT_TICKS		1//Time is measured in 'ticks' Defining configUSE_16_BIT_TICKS as 1 causes portTickType to be defined (typedef'ed) as an unsigned 16bit type. Defining configUSE_16_BIT_TICKS as 0 causes portTickType to be defined (typedef'ed) as an unsigned 32bit type.

#define configIDLE_SHOULD_YIELD		1//This parameter controls the behaviour of tasks at the idle priority.

#define configQUEUE_REGISTRY_SIZE	0



/* Debug */

//#define configCHECK_FOR_STACK_OVERFLOW 1//0



/* Co-routine definitions. */

#define configUSE_CO_ROUTINES 		0 //1

#define configMAX_CO_ROUTINE_PRIORITIES ( 1 )//2



/* Set the following definitions to 1 to include the API function, or zero

to exclude the API function. */



#define INCLUDE_vTaskPrioritySet        1

#define INCLUDE_uxTaskPriorityGet       1

#define INCLUDE_vTaskDelete             0

#define INCLUDE_vTaskCleanUpResources   0

#define INCLUDE_vTaskSuspend            1

#define INCLUDE_vTaskDelayUntil         1

#define INCLUDE_vTaskDelay              1



#define STACK_SIZE_VTY           1000


#endif /* FREERTOS_CONFIG_H */
