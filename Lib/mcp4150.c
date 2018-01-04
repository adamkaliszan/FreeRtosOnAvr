/*****************************************************************************
* vim:sw=2:ts=2:si:et
*
* Title      : MPC23s17 parrarel I/O 16 bit port driver
* Author     : Adam Kaliszan adam.kaliszan@gmail.com
* Copyright  : GPL V2
*
*This driver provides:
* - read/set operation
*Driver uses mutexes and is condition race free. Function can be invoken by any thread.
*****************************************************************************/
//@{
#include <avr/io.h>
#include <util/delay.h>
#include "mcp4150.h"
#include "spiXmega.h"

void enableSpiMCP4150(void)  {};
void disableSpiMCP4150(void) {};

/**
 * Ustawia wartość rezystancji
 * @param inputNo - WARTOŚĆ OD 0 DO 255.
 */
void MCP4150_setValue(uint8_t value)
{  
  spiTake();
  enableSpiMCP4150();
  
  spiSend(0x11);  
  spiSend(value);
  
  disableSpiMCP4150(); 
  spiGive();
}

//@}