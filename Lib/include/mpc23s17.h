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


#ifndef MPC32S17_H
#define MPC32S17_H

#include <stdint.h>
#include "main.h"
#include "spiXmega.h"

#define B0_IODIRA 0x00
#define B0_IODIRB 0x01

#define B0_GPIOA  0x12
#define B0_GPIOB  0x13
#define B0_OLATA  0x14
#define B0_OLATB  0x15


volatile uint8_t portA;
volatile uint8_t portB;

// functions

void enableSpiMPC23S17(void)  __attribute__ ((weak));
void disableSpiMPC23S17(void) __attribute__ ((weak));

/**
 * Ustawia kierunek na porcie A
 * @param  portAdir    - kierunak dla każdej linni w porcie 0 - wejście, 1 - wyjście
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 */
void MPC23s17SetDirA(uint8_t portAdir, uint8_t addr);

/**
 * Ustawia kierunek na porcie B
 * @param  portBdir    - stan portu wyjściowego
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 */
void MPC23s17SetDirB(uint8_t portBdir, uint8_t addr);

/**
 * Ustawia wyjście na porcie A
 * @param  portAout    - kierunak dla każdej linni w porcie 0 - wejście, 1 - wyjście
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 */
void MPC23s17SetPortA(uint8_t portAout, uint8_t addr);

/**
 * Ustawia wybrane bity na wyjściu portu A
 * @param  portAout    - kierunak dla każdej linni w porcie 0 - wejście, 1 - wyjście
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 */
void MPC23s17SetBitsOnPortA(uint8_t portAout, uint8_t addr);

/**
 * Zeruje wybrane bity na wyjściu portu A
 * @param  portAout    - kierunak dla każdej linni w porcie 0 - wejście, 1 - wyjście
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 */
void MPC23s17ClearBitsOnPortA(uint8_t portAout, uint8_t addr);

/**
 * Ustawia wyjście na porcie B
 * @param  portBout    - stan portu wyjściowego
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 */
void MPC23s17SetPortB(uint8_t portBout, uint8_t addr);

/**
 * Ustawia wybrane bity na wyjściu portu B
 * @param  portAout    - kierunak dla każdej linni w porcie 0 - wejście, 1 - wyjście
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 */
void MPC23s17SetBitsOnPortB(uint8_t portAout, uint8_t addr);

/**
 * Zeruje wybrane bity na wyjściu portu B
 * @param  portAout    - kierunak dla każdej linni w porcie 0 - wejście, 1 - wyjście
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 */
void MPC23s17ClearBitsOnPortB(uint8_t portAout, uint8_t addr);


/**
 * Czyta wejście portu A
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 * @return stan na wejściu portu A
 */
uint8_t MPC23s17ReadPortA(uint8_t addr);

/**
 * Czyta wejście portu B
 * @param  addr        - adres (0-8) portu. Kilka scalaków MPC... może działać pod tą samą linią adresową CS w magistrali SPI
 * @return stan na wejściu portu B
 */
uint8_t MPC23s17ReadPortB(uint8_t addr);


#endif
//@}
