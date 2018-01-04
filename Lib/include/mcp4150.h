/*****************************************************************************
* vim:sw=2:ts=2:si:et
*
* Title      : MCP3008 / MCP3004 A/C converter driver
* Author     : Adam Kaliszan adam.kaliszan@gmail.com
* Copyright  : GPL V2
*
*This driver provides:
* - read/set operation
*Driver uses mutexes and is condition race free. Function can be invoken by any thread.
*****************************************************************************/
//@{

#ifndef MCP4140_H
#define MCP4150_H

#include <inttypes.h>
#include "main.h"
#include "spiXmega.h"


// functions

void enableSpiMCP4150(void)  __attribute__ ((weak));
void disableSpiMCP4150(void) __attribute__ ((weak));

/**
 * Ustawia wartość rezystancji
 * @param inputNo - WARTOŚĆ OD 0 DO 255.
 */
void MCP4150_setValue(uint8_t value);

#endif
//@}