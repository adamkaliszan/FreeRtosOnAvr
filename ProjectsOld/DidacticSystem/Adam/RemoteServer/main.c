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

xQueueHandle xRs1Tx;
xQueueHandle xRs1Rec;

xQueueHandle xRsLanRec;
xQueueHandle xRsLanTx;

xQueueHandle xSpi2SerialTx[16];
xQueueHandle xSpi2SerialRx[16];
xQueueHandle xRsLanTx;


void vApplicationIdleHook( void );

/**
 * RTC clock support
 */
void vApplicationTickHook( void );

xTaskHandle xHandleVTY_Rs1;
xTaskHandle xHandleEnc;

xTaskHandle xHandleXpReceiver;
xTaskHandle xHandleXpTransmitter;


void initExternalMem(void)
{

  MCUCR |= _BV(SRE);          //Włączenie pamięci zewnętrznej
  MCUCR |= 0x0E;

#if testZewPamiec == 1
#define SND(DTA) UDR1=DTA; \
  while( ! (UCSR1A & (1<<UDRE))); \

  UBRR1L = 8;
  UBRR1H = 0;
  UCSR1B = 1<<TXEN1;

  SND('\r') SND('\n') SND('M') SND('e') SND('m') SND('o') SND('r') SND('y') SND('t') SND('e') SND('s') SND('t') SND('\r') SND('\n')

  uint8_t hiAddr;
  for (hiAddr = 0x11; hiAddr < 0xFF; hiAddr++)
  {
    uint8_t znHi = (hiAddr>>4 & 0x0F);
    uint8_t znLo = (hiAddr & 0x0F);

    znHi = (znHi < 10) ? znHi + '0' : znHi + 'A' - 10;
    znLo = (znLo < 10) ? znLo + '0' : znLo + 'A' - 10;

    SND('\r') SND('0') SND('x') SND(znHi) SND(znLo) SND('*') SND('*') SND(' ')

    uint16_t addr;
    uint16_t startAddr = hiAddr<<8;
    uint16_t stopAddr = startAddr + 0xFF;

    for (addr = startAddr; addr <= stopAddr; addr++)
       *((uint8_t *) addr) = (uint8_t)((addr>>1) & 0xFF);

    uint8_t isOK=1;
    for (addr = startAddr; addr <= stopAddr; addr++)
      if (*((uint8_t *) addr) != (uint8_t)((addr>>1) & 0xFF))
        isOK = 0;

    if (isOK == 1) { SND ('O') SND ('K') SND('\n') }
    else           { SND ('F') SND ('A') SND ('I') SND('L') SND('\n') }
  }
  SND ('\r') SND ('\n')
#undef SND
#endif
}

cmdState_t *CLIStateSerialRs1;
//cmdState_t *CLIStateSerialUdp;
FILE rs1Stream;
FILE udpStream;

streamBuffers_t udpBuffers;

portSHORT main( void )
{
  //ramDyskInit();              //Inicjalizacja Ram dysku
  hardwareInit();
  spiInit(disableAllSpiDevices);

// VTY on serial
  xSerialPortInitMinimal();
  CLIStateSerialRs1  = xmalloc(sizeof(cmdState_t));
  //CLIStateSerialUdp  = xmalloc(sizeof(cmdState_t));



  initQueueRs1Stream(&rs1Stream);
  VtyInit(CLIStateSerialRs1, &rs1Stream);


  initQeuesSpi2Serial();

  udpInit_0();
  loadConfiguration();

  //initQueueStream(&udpStream, &udpBuffers, udpSocket->Rx, udpSocket->Tx);
  //VtyInit(CLIStateSerialUdp, &udpStream);

  xTaskCreate(spiTask,              NULL /*"ENC"    */, STACK_SIZE_ENC,       NULL,                                   0, &xHandleEnc);
  xTaskCreate(vTaskVtyRs1,          NULL /*"VTY"    */, STACK_SIZE_VTY,       (void *)(CLIStateSerialRs1),            1, &xHandleVTY_Rs1);
  xTaskCreate(XpNetReceiverTask,    NULL /*"ENC"    */, 128,                  NULL,                                   0, &xHandleXpReceiver);
  xTaskCreate(XpNetTransmitterTask, NULL /*"nTX"    */, 128,                  NULL,                                   0, &xHandleXpTransmitter);
//xTaskCreate(vTaskVTYsocket, NULL /*"VTY"    */, STACK_SIZE_VTY,       (void *)(CLIStateSerialUdp),            1, &xHandleVTY_UDP);

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
