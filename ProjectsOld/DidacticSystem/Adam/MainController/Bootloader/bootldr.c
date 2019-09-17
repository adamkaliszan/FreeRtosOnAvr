#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/crc16.h>

#include "bootldr.h"

uint8_t         KEY[]                        = {'f', 'l', 'a', 's', 'h'};
uint8_t         buf[BUFFERSIZE];             // xModem receive buffer

void main(void)
{
  uint8_t         ch;
  uint8_t         cl;
  uint8_t         crch;
  uint8_t         crcl;
  uint8_t         packNO;
  uint8_t         li;
  uint8_t         tmp;
  uint16_t        crc;
  uint32_t        FlashAddr;
  uint8_t         pagptr;
  uint8_t         cnt;

  cli();                             // disable interrupt
  
  //Configure timer T1
  OCR1A       = (uint16_t)(timeclk * (F_CPU  / (1024 * 1000.0f)));
  TCCR1A      = 0;
  TCCR1B      = (1 << WGM12)|(1 << CS12)|(1 << CS10);

  //Configure ports

  //initialize serial port UART1
  UCSR1A      = 0;
  UCSR1B      = (1 << RXEN1)|(1 << TXEN1);
  UCSR1C      = (1 << UCSZ11)|(1 << UCSZ10);
  UBRR1H      = 0;
  UBRR1L      = 7;
  
  cnt         = TimeOutCnt;     // Timeout 1 = 1s
  cl          = 0;              // Number of matching password's letters
  
  WriteCom('B');
  //Waiting for sequence "flash" to start flashing
  while(1)
  {
    if(TIFRREG & (1<<OCF1A))    // Checking T1 overflow
    {
      TIFRREG |= (1 << OCF1A);  // clearing the overflow flag

      cnt--;                    // Counting down number of retries
      if(cnt == 0)              // Checking of time expiration
      {
        quit();
      }
    }

    if(DataInCom())             // Checking data in buffer
    {
      tmp = ReadCom();
      if (KEY[cl] == tmp)
        cl++;
      else
        cl=0;
    }
    if (cl == 5)                // password is correct
      break;
  }

  cnt = TimeOutCntC;
  while(1)
  {
    if(TIFRREG & (1 << OCF1A))  //T1 overflow
    {
      TIFRREG |= (1 << OCF1A);
      WriteCom(XMODEM_RWC) ;    //send "C"

      cnt--;
      if(cnt == 0)
        quit();
    }
    if(DataInCom())
      if(ReadCom() == XMODEM_SOH)  //XMODEM command <soh>
        break;
  }

  //begin xModem transmission - receive data
  packNO    = 1;
  cnt       = 0;
  FlashAddr = 0;
  pagptr    = 0;
  
  while(1)
  {
    ch =  ReadCom_withWaiting();                          // get package number
    cl = ~ReadCom_withWaiting();
    if ((packNO == ch) && (packNO == cl))
    {
      uint16_t crc = 0;                                   // start calculate CRC
      for(li = 0; li < BUFFERSIZE; li++)                  // receive a full data frame
      {
        tmp     = ReadCom_withWaiting();                  // read from uart
        buf[li] = tmp;                                    // write to temporary buffer
        crc     = _crc_xmodem_update(crc, tmp);           // calculate CRC
      }

      crch = ReadCom_withWaiting();                       // get checksum Hi
      crcl = ReadCom_withWaiting();                       // get checksum Lo
      ch = crc / 256;
      cl = crc % 256;
      if ((crch == ch) && (crcl == cl))
      {
        packNO++;
        cnt = 0;
        uint16_t *tmpPtr = (uint16_t *)(buf);             // Compiller have to be little bit endian
        for (tmp=0; tmp < BUFFERSIZE/2; tmp++)
        {
//#define TEST 1
#ifndef TEST
          if (pagptr == 0)
          {
            eeprom_busy_wait();
            boot_page_erase(FlashAddr);                   // Kasowanie strony
            boot_spm_busy_wait();                         // Czekanie na wykonanie operacji
          }
          boot_page_fill(FlashAddr+pagptr, *tmpPtr);      //brzydkie rzutowanie 2X uint8_t na uint16_t
#endif
          tmpPtr++;                                       //wskaźnik zwiększa się o 2
          pagptr+=2;

#if SPM_PAGESIZE != 256	  
          if (pagptr == SPM_PAGESIZE)
#else
          if (pagptr == 0)
#endif
          {
#ifndef TEST 
            boot_page_write(FlashAddr);                   //Zapis strony
            boot_spm_busy_wait();                         //Czekanie na wykonanie operacji
#endif
            FlashAddr += SPM_PAGESIZE;                    // modify Flash page address
#if SPM_PAGESIZE != 256
            pagptr = 0;
#endif
          }

          if (FlashAddr > 2*BootStart)                    // BootStart is word number (not byte number)
          {
            WriteCom(XMODEM_CAN);                         // Stop transmission
            quit();
          }
        }
        WriteCom(XMODEM_ACK);                             // All OK send ACK
      }
      else                                                // CRC error
      {
        WriteCom(XMODEM_NAK);                             // Ask resend
        cnt++;
      }
    }
    else                                                  // Wrong PackNo
    {
      WriteCom(XMODEM_NAK);                               // Ask resend
      cnt++;
    }
    if(cnt > 3)                                           // Too much error, update abort
    {
      WriteCom(XMODEM_CAN);                               // Cancel      
      quit();
    }
    
    tmp = ReadCom_withWaiting();
    
    if (tmp == XMODEM_EOT)
    {
#ifndef TEST 
      if (pagptr != 0)
      {
        boot_page_write(FlashAddr);                       //Zapis ostatniej strony. Nie jest ona w całości zapełniona
        boot_spm_busy_wait();                             //Czekanie na wykonanie operacji
      }
#endif
      WriteCom(XMODEM_ACK);                               // Firmware downloaded. Send NAK to stop transmission
      break;
    }
    if (tmp == XMODEM_CAN)
      break;    
  }
  //while(1);
  quit();
}

//! quit boot mode and jump to user's application
void quit(void)
{
  boot_rww_enable();                                      // Enable application section
  (*((void(*)(void))PROG_START))();                       // Jump to aplication code
}

//! wait for uart data and read it next
uint8_t ReadCom_withWaiting(void)
{
//  uint8_t tmp=3;
  while (1)
  {
    if (UCSR1A & (1 << RXC1))
    {
      return UDR1;
    }    
//    if(TIFRREG & (1<<OCF1A))    // Checking T1 overflow
//    {
//      TIFRREG |= (1 << OCF1A);  // clearing the overflow flag

//      tmp--;                    // Counting down number of retries
//      if (tmp == 0)             // Checking of time expiration
//      {
//        return 0;
//      }
//    }
  }
}

//send data to comport
void WriteCom(uint8_t dat)
{
  UDR1 = dat;
  while(!(UCSR1A & (1<<TXC1)))
    ;    //wait for send finish

  UCSR1A |= (1 << TXC1);
}
