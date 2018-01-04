/*****************************************************************************
* vim:sw=2:ts=2:si:et
*
* Title      : MPC23s17 parrarel I/O 16 bit port driver
* Author     : Adam Kaliszan
* Copyright: GPL V2
*
*This driver provides:
* - read/set operation
*Driver uses mutexex and is condition race free. Function can be invoken by any thread.
*****************************************************************************/
//@{
#include <avr/io.h>
#include <util/delay.h>
#include "mpc23s17.h"
#include "spiXmega.h"

void enableSpiMPC23S17(void)  {}
void disableSpiMPC23S17(void) {}


void MPC23s17SetDirA(uint8_t portAdir, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  enableSpiMPC23S17(); 
  spiSend(addr);
  spiSend(B0_IODIRA);
  spiSend(portAdir);  
  
  disableSpiMPC23S17();
  spiGive();
}

void MPC23s17SetDirB(uint8_t portBdir, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  enableSpiMPC23S17();
  
  spiSend(addr);
  spiSend(B0_IODIRB);
  spiSend(portBdir);  
  
  disableSpiMPC23S17();
  spiGive();
}

void MPC23s17SetPortA(uint8_t portAout, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  enableSpiMPC23S17();
  portA = portAout;  
  spiSend(addr);
  spiSend(B0_OLATA);
  spiSend(portA);  

  disableSpiMPC23S17();
  spiGive();
}

void MPC23s17SetBitsOnPortA(uint8_t portAout, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  portA |= portAout;  

  spiTake();
  enableSpiMPC23S17();
  spiSend(addr);
  spiSend(B0_OLATA);
  spiSend(portA);  
  disableSpiMPC23S17();
  spiGive();
}

void MPC23s17ClearBitsOnPortA(uint8_t portAout, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  portA &= (~portAout);

  spiTake();
  enableSpiMPC23S17();
  spiSend(addr);
  spiSend(B0_OLATA);
  spiSend(portA);  
  disableSpiMPC23S17();
  spiGive();
}

void MPC23s17SetPortB(uint8_t portBout, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  portB = portBout;

  spiTake();
  enableSpiMPC23S17();
  spiSend(addr);
  spiSend(B0_OLATB);
  spiSend(portB);  
  disableSpiMPC23S17();
  spiGive();
}

void MPC23s17SetBitsOnPortB(uint8_t portBout, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  portB |= portBout;

  spiTake();
  enableSpiMPC23S17();
  spiSend(addr);
  spiSend(B0_OLATB);
  spiSend(portB);  
  disableSpiMPC23S17();
  spiGive();
}

void MPC23s17ClearBitsOnPortB(uint8_t portBout, uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x40;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  portB &= (~portBout);

  spiTake();
  enableSpiMPC23S17();
  spiSend(addr);
  spiSend(B0_OLATB);
  spiSend(portB);  

  disableSpiMPC23S17();
  spiGive();
}

uint8_t MPC23s17ReadPortA(uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x41;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  enableSpiMPC23S17();

  spiSend(addr);
  spiSend(B0_GPIOA);
  uint8_t result = spiSend(addr);  

  disableSpiMPC23S17();
  spiGive();

  return result; 
}

uint8_t MPC23s17ReadPortB(uint8_t addr)
{
  addr = addr<<1;
  addr &= 0x0E;
  addr |= 0x41;        //OPCODE 0100 AAA 0 bit0: 0 - zapis, 1 - odczyt 

  spiTake();
  enableSpiMPC23S17();

  spiSend(addr);
  spiSend(B0_GPIOB);
  uint8_t result = spiSend(addr);  

  disableSpiMPC23S17();
  spiGive();

  return result; 
}
