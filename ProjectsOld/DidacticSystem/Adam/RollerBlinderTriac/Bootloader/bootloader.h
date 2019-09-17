/*
  Project:       AVR Rs485 bootloader
  Base on
                 Project:  AVR Universal BootLoader
                 Author:   Shaoziyang
                            Shaoziyang@gmail.com
                            http://avrubd.googlepages.com

  Author:           Adam Kaliszan
                    adam.kaliszan@gmail.com

  File:             bootldr.h
                    main program code

  Version:          0.9

  Date:          2010.4
*/

#ifndef _BOOTLDR_H_
#define _BOOTLDR_H_        1

#include <avr/io.h>

#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/crc16.h>

#include "../../../freeRtos/Lib/include/protocol1.h"
#include "../../../freeRtos/Lib/include/xModemCommands.h"

#define PROT_BUF_LEN      32                  /// Rs485 rec buffer length


#ifndef F_CPU                                 ///system clock(Hz)
#define F_CPU              7372800UL
#endif

#define PROG_START         0x0000             /// User's application start address
#define BootStart          0x1C00             /// Boot section address start

#define BUFFERSIZE         128                /// Xmodem buffer length
#define BAUDRATE           115200             /// Rs485 baudrate


#define timeclk            500                /// basic timer interval(ms)
#define TimeOutCnt         10                 /// max wait for password time = TimeOutCnt * timeclk
#define TimeOutCntC        40                 /// numbers of sending C char for start xModem


#define COMPORTNo          0                  /// comport number: 0/1/2..

#define RS485RXDis         2                  /// Pin that disables Rs485 receiver
#define RS485TXEn          3                  /// Pin that enables Rs485 transmitter
#define LEDPORTNo          5                  /// Pin that is connected to the diode 1
#define LED2PORTNo         6                  /// Pin that is connected to the diode 2


#define RS485Enable()      (PORTD |=  0x08)
#define RS485Disable()     (PORTD &=  0xF7)

//receive buffer' size will not smaller than SPM_PAGESIZE
#if BUFFERSIZE < SPM_PAGESIZE
#define BUFSIZE SPM_PAGESIZE
#else
#define BUFSIZE BUFFERSIZE
#endif


//certain version compiler missing this in head files
#ifndef SPM_PAGESIZE
#error "Not define SPM_PAGESIZE, please define below or update your WinAVR"
//#define SPM_PAGESIZE       XXX
#endif

//certain version compiler missing this in head files
#ifndef FLASHEND
#error "Not define FLASHEND, please define below or update your WinAVR"
//#define FLASHEND           XXX
#endif

//two buffer size must be multiple or submultiple relation
#if BUFFERSIZE >= SPM_PAGESIZE
#if (BUFFERSIZE / SPM_PAGESIZE * SPM_PAGESIZE) != BUFFERSIZE
#error "Result of (BUFFERSIZE / SPM_PAGESIZE) is not a Integer!"
#error "Please check and set 'BUFFERSIZE/SPM_PAGESIZE' Macro again!"
#endif
#else
#if (SPM_PAGESIZE /BUFFERSIZE * BUFFERSIZE) != SPM_PAGESIZE
#error "Result of (BUFFERSIZE / SPM_PAGESIZE) is not a Integer!"
#error "Please check and set 'BUFFERSIZE/SPM_PAGESIZE' Macro again!"
#endif
#endif

//calculate baudrate register
#define BAUDREG            ((unsigned int)((F_CPU * 10) / (16UL * BAUDRATE) - 5) / 10)

//check baudrate register error
//mocro below maybe not same in different C compiler
#define FreqTemp           (16UL * BAUDRATE * (((F_CPU * 10) / (16 * BAUDRATE) + 5)/ 10))
#if ((FreqTemp * 50) > (51 * F_CPU) || (FreqTemp * 50) < (49 * F_CPU))
#error "BaudRate error > 2% ! Please check BaudRate and F_CPU value."
#endif

#define True               1
#define False              0
#define TRUE               1
#define FALSE              0

//internal use macro
#define CONCAT(a, b)       a ## b
#define CONCAT3(a, b, c)   a ## b ## c

//register of PORT and bit define
#define PORTREG(No)        CONCAT(PORT, No)
#define PINREG(No)         CONCAT(PIN, No)
#define UDRREG(No)         CONCAT(UDR, No)
#define DDRREG(No)         CONCAT(DDR, No)
#define TXCBIT(No)         CONCAT(TXC, No)
#define RXCBIT(No)         CONCAT(RXC, No)
#define RXENBIT(No)        CONCAT(RXEN, No)
#define TXENBIT(No)        CONCAT(TXEN, No)
#define URSELBIT(No)       CONCAT(URSEL, No)

//some kind of AVR need URSEL define when comport initialize
#define USEURSEL           0


//timer1 overflow register
#ifdef TIFR
#define TIFRREG            TIFR
#else
#define TIFRREG            TIFR1
#endif

//Xmoden control command
#define XMODEM_NUL         0x00
#define XMODEM_SOH         0x01
#define XMODEM_STX         0x02
#define XMODEM_EOT         0x04
#define XMODEM_ACK         0x06
#define XMODEM_NAK         0x15
#define XMODEM_CAN         0x18
#define XMODEM_EOF         0x1A
#define XMODEM_RWC         'C'



//#define DataInCom()        (UCSRAREG(COMPORTNo) & (1 << RXCBIT(COMPORTNo)))
#define DataInCom()        (UCSR0A & (1 << RXC0))
#define ReadCom()          UDR0

//@{
/**
 * Stan automatu:
 * sync, addr, rozkaz, dlDanych, dane, crcHi, crcLo, xModem
 */
enum stanAutomatu
{
  sync,
  addr,
  rozkaz,
  dlDanych,
  dane,
  crcHi,
  crcLo,
  xModem
};
typedef enum stanAutomatu stanAutomatu_t;

/**
 * Leave bootloader mode
 */
void quit(void)         __attribute__((noreturn));

/**
 * Wait for a tame of different device flashing and leave next bootloader mode
 */
void waitAndQuit(void)  __attribute__((noreturn));

uint8_t ReadCom_withLimitedWaiting(void);

//@}
#endif

//End of file: bootldr.h
