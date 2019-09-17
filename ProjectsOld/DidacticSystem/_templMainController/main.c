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

xQueueHandle xRs485Tx;
xQueueHandle xRs485Rec;


volatile uint8_t temperature;
volatile uint8_t voltage;


void vApplicationIdleHook( void );

void vBlinkLed(void *pvParameters);

/**
 * RTC clock support
 */
void vApplicationTickHook( void );

xTaskHandle xHandleVTY_USB;
xTaskHandle xHandleVTY_UDP;
xTaskHandle xHandleEnc;
xTaskHandle xHandleSensors;
xTaskHandle xHandleBlinkLed;

void initExternalMem(void)
{
  MCUCR |= _BV(SRE);          //Włączenie pamięci zewnętrznej
  MCUCR |= 0x0E;
}

cmdState_t *CLIStateSerialUsb;
cmdState_t *CLIStateSerialUdp;
FILE usbStream;
FILE udpStream;

streamBuffers_t udpBuffers;

portSHORT main( void )
{
  ramDyskInit();              //Inicjalizacja Ram dysku
  hardwareInit();
  spiInit(disableAllSpiDevices);

// VTY on serial  
  xSerialPortInitMinimal(); 
  CLIStateSerialUsb  = xmalloc(sizeof(cmdState_t));
  CLIStateSerialUdp  = xmalloc(sizeof(cmdState_t));


//  cmdStateClear(newCmdState);
  DDRF = 0x0F;    //JTAG and A/C
  PORTF |= 0x06; 
  
  sensorsTaskInit();
  loadConfiguration();

  initQueueStreamUSB(&usbStream);
  VtyInit(CLIStateSerialUsb, &usbStream);

  udpInit();
  socketInit();
  initQueueStream(&udpStream, &udpBuffers, udpSocket->Rx, udpSocket->Tx);
  VtyInit(CLIStateSerialUdp, &udpStream);
  
  xTaskCreate(encTask,        NULL /*"ENC"    */, STACK_SIZE_ENC,       (void *)CLIStateSerialUsb->myStdInOut,  0, &xHandleEnc);
  xTaskCreate(vTaskVTYusb,    NULL /*"VTY"    */, STACK_SIZE_VTY,       (void *)(CLIStateSerialUsb),            1, &xHandleVTY_USB);
  xTaskCreate(vTaskVTYsocket, NULL /*"VTY"    */, STACK_SIZE_VTY,       (void *)(CLIStateSerialUdp),            1, &xHandleVTY_UDP);
  xTaskCreate(sensorsTask,    NULL /*"Sensors"*/, STACK_SIZE_SENSORS,   NULL,                                   1, &xHandleSensors);
  xTaskCreate(vBlinkLed,      NULL /*"ENC"    */, 0, 			NULL, 					2, &xHandleBlinkLed);
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
  static uint8_t tickCntr = configTICK_RATE_HZ;
  if (--tickCntr == 0)
  {
    tickCntr = configTICK_RATE_HZ;
    arpTimer();    
  }
}

void vBlinkLed(void *pvParameters)
{
  //cmdState_t *state = (cmdState_t *)(pvParameters);
    
  //uint8_t f = 0;
  for(;;)
  {
      portTickType xDelay2 = 1000 / portTICK_RATE_MS;
      vTaskDelay(xDelay2);
      PORTF ^= 0x04; 
//      f = xTaskGetTickCount();
      //fprintf( (FILE *)&state->myStdInOut, "B");
      //fprintf(&state->myStdInOut, "TickTime: %d\r\n", f);
  }
}
