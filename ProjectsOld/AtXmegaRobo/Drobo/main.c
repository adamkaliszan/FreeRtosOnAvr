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


uint8_t timer100Hz = 0;

xQueueHandle xVtyTx;
xQueueHandle xVtyRec;

xQueueHandle xHC12Tx;
xQueueHandle xHC12Rec;
xQueueHandle xMainRec;

xQueueHandle xSIM900Rec;
xQueueHandle xSIM900Tx;

cmdState_t *CLIStateSerialUsb;
tlvInterpreter_t *TLVstate;

FILE usbStream;
FILE hc12Stream;
FILE hc12FakeStream;


xSemaphoreHandle Hc12semaphore;

xTaskHandle xHandleVTY_USB;
xTaskHandle xHandleTLV;
xTaskHandle xHandleMain;


void vApplicationIdleHook( void );

/**
 * RTC clock support
 */
void vApplicationTickHook( void );



//xTaskHandle xHandleTranslator;

/**
  * konfiguracja zegara. Zegar wewnętrzny 32 MHz
  */
void my_init_clock(void)
{// Configure clock to 32MHz
    OSC.CTRL |= OSC_RC32MEN_bm | OSC_RC32KEN_bm;  /* Enable the internal 32MHz & 32KHz oscillators */
    while(!(OSC.STATUS & OSC_RC32KRDY_bm));       /* Wait for 32Khz oscillator to stabilize */
    while(!(OSC.STATUS & OSC_RC32MRDY_bm));       /* Wait for 32MHz oscillator to stabilize */
    DFLLRC32M.CTRL = DFLL_ENABLE_bm ;             /* Enable DFLL - defaults to calibrate against internal 32Khz clock */
    CCP = CCP_IOREG_gc;                           /* Disable register security for clock update */
    CLK.CTRL = CLK_SCLKSEL_RC32M_gc;              /* Switch to 32MHz clock */
    OSC.CTRL &= ~OSC_RC2MEN_bm;                   /* Disable 2Mhz oscillator */
}


#ifdef USENET
cmdState_t *CLIStateSerialUdp;
FILE udpStream;
#endif

streamBuffers_t udpBuffers;

//#include <avr/iox128a1.h>

// Define CPU clock frequency (if not already defined)
#ifndef F_CPU
   // Enable/Disable internal oscillators
   //#define F_CPU 2000000
   #define F_CPU 32000000
   // Enable/Disable external 14.7456 MHz oscillator
   //#define F_CPU 14745600
#endif


portSHORT main( void )
{
  hardwareInit();
  loadConfiguration();
  xSerialPortInitMinimal();

  xMainRec = xQueueCreate(16, 1);


  CLIStateSerialUsb  = xmalloc(sizeof(cmdState_t));
  TLVstate = xmalloc(sizeof(tlvInterpreter_t));

  Hc12semaphore = xSemaphoreCreateMutex();

  initQueueStreamUSB(&usbStream);
  initQueueStreamHC12(&hc12Stream);
  initQueueStreamHC12fake(&hc12FakeStream);

  VtyInit(CLIStateSerialUsb, &usbStream);
  tlvIinitializeInterpreter(TLVstate, &hc12Stream, &usbStream, tlvCmdList);

  sei();

  xTaskCreate(vTaskVTYusb,    NULL /*"VTY"            */, STACK_SIZE_VTY,         (void *)(CLIStateSerialUsb),            1, &xHandleVTY_USB);
  xTaskCreate(vTaskTLV,       NULL /*"TLV"            */, STACK_SIZE_VTY,         (void *)(TLVstate),                     1, &xHandleTLV);
  xTaskCreate(vTaskMain,      NULL /*"TLV"            */, STACK_SIZE_VTY,         NULL,                                   1, &xHandleMain);

  vTaskStartScheduler();

  return 0;
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
  for( ;; )
  {
    vCoRoutineSchedule();
  }
}

void vApplicationTickHook( void )
{
  static uint16_t tickCntr = configTICK_RATE_HZ;
  if (--tickCntr == 0)
  {
    tickCntr = configTICK_RATE_HZ;
#ifdef USENET
    arpTimer();
#endif
  }
}
