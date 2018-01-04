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

#ifndef MCP3008_H
#define MCP3008_H

#include <inttypes.h>
#include "main.h"
#include "spiXmega.h"


// functions

void enableSpiMCP3008(void)  __attribute__ ((weak));
void disableSpiMCP3008(void) __attribute__ ((weak));

/**
 * Zwraca watość próbki dla pojedynczego wejścia
 * @param inputNo - numer wejścia (0-7).
 */
uint16_t MCP3008_getSampleSingle(uint8_t inputNo);

/**
 * Zwraca watość próbki dla różnicowego wejścia
 * @param inputNo - numer wejścia (0-7).
 */
uint16_t MCP3008_getSampleDiff(uint8_t inputNo);

#endif
//@}
