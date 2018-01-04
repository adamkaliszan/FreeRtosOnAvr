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
#include "mcp3008.h"
#include "spiXmega.h"

void enableSpiMCP3008(void)   {};
void disableSpiMCP3008(void)  {};


uint16_t MCP3008_getSampleDiff(uint8_t inputNo)
{
  uint8_t resultLo;
  uint8_t resultHi;
  inputNo = inputNo << 4;
  inputNo &= 0x70;
  
  spiTake();
  enableSpiMCP3008();
  
  spiSend(0x01);                               //Start
  resultHi = spiSend(inputNo);  //DIFF/!SGL A2 A1 A0 X X X X
  resultLo = spiSend(0);        //X X X X X X X X
  
  disableSpiMCP3008(); 
  spiGive();
  
  resultHi &= 0x03;
  return resultHi << 8 | resultLo;
}

uint16_t MCP3008_getSampleSingle(uint8_t inputNo)
{
  uint8_t resultLo;
  uint8_t resultHi;
  inputNo = inputNo << 4;
  inputNo &= 0x70;
  inputNo |= 0x80;
  
  spiTake();
  enableSpiMCP3008();
  
  spiSend(0x01);                //Start
  resultHi = spiSend(inputNo);  //DIFF/!SGL A2 A1 A0 X X X X
  resultLo = spiSend(0);        //X X X X X X X X
  
  disableSpiMCP3008(); 
  spiGive();
  
  resultHi &= 0x03;
  return resultHi << 8 | resultLo;
}
//@}