/*
  Project:       AVR AtMega128 bootloader with Xmodem protocol

  Author:           Adam Kaliszan
                    adam.kaliszan@gmail.com

  File:             bootldr.h
                    main program code

  Version:          0.91

  Date:          2010.5
*/

#ifndef _BOOTLDR_H_
#define _BOOTLDR_H_        1

//#define ATMETA64          1


#ifndef F_CPU                                 ///system clock(Hz)
#define F_CPU              14745600UL
#endif

#define PROG_START         0x0000             /// User's application start address

#ifdef ATMEGA64
 #define BootStart         0x7E00             /// Boot section address start
#else
 #define BootStart         0xFE00             /// Boot section address start
#endif

#define BUFFERSIZE         128                /// Xmodem buffer length
#define BAUDRATE           115200             /// USB -> Rs232 baudrate

#define timeclk            500                /// basic timer interval(ms)
#define TimeOutCnt         10                 /// max wait for password time = TimeOutCnt * timeclk
#define TimeOutCntC        40                 /// numbers of sending C char for start xModem


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

#define DataInCom()        (UCSR1A & (1 << RXC1))
#define ReadCom()          UDR1

void    quit               (void)              __attribute__((noreturn));
uint8_t ReadCom_withWaiting(void);
void    WriteCom           (uint8_t dat);

#endif

//End of file: bootldr.h
