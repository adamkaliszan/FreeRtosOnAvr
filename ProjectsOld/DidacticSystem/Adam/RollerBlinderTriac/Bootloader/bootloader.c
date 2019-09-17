/*
  Project:       AVR Rs485 bootloader
  Base on
                 Project:  AVR Universal BootLoader
                 Author:   Shaoziyang
                            Shaoziyang@gmail.com
                            http://avrubd.googlepages.com

  Author:           Adam Kaliszan
                    adam.kaliszan@gmail.com

  File:             bootldr.c
                    main program code

  Version:          0.9

  Date:          2010.4
*/

#include "bootloader.h"

uint8_t         helloBuf[]                    = {SYNC, 0, rHELLO, 8, 0x00 , 0x00, 0x00, 'b', '0', '.', '7', '0'};   //rHELLO response
uint8_t         pingBuf[HDR_LEN+PROT_BUF_LEN] = {SYNC, 0, rPING, 8};                                                //rPING  response
uint8_t         noCommandBuf[]                = {SYNC, 0, rUNKNOWN, 0};                                             //unknown command response

uint8_t         buf[BUFSIZE];                                                                                       // xModem receive buffer
uint8_t         bufptr;
uint8_t         pagptr;
uint8_t         ch;
uint8_t         cl;
uint16_t        FlashAddr;
uint8_t         cnt;

uint8_t         tmp;
uint8_t         adres;                         // Adres urządzenia zczytany na starcie
stanAutomatu_t  stan;                          // Stan automatu
uint16_t        crc;                           // Crc
uint16_t        crcObl;                        // Crc
uint8_t         protAddr;                      // Odebrany adres w magistrali Rs485
uint8_t         protRozkaz;                    // Odebrany kod rozkazu
uint8_t         protDlDanych;                  // Odebrana inf. o długości pola danych
uint8_t         protBuf[PROT_BUF_LEN];         // Odebrane dane do rozkazu
uint8_t         protCrcHi;                     // Odebrany bardziej znaczący bajt CRC
uint8_t         protCrcLo;                     // Odebrany mniej znaczący bajt CRC
uint8_t         protDaneWsk;                   // Licznik odebranych danych


inline void write_one_page(unsigned char *buf) //Uaktualnianie strony z pamięcią flash
{
  boot_page_erase(FlashAddr);                  //Kasowanie strony
  boot_spm_busy_wait();                        //Czekanie na wykonanie operacji
  
  uint16_t *tmpPtr = (uint16_t *)(buf);        //kompilator musi być little bit endian
  for(pagptr = 0; pagptr < SPM_PAGESIZE; pagptr += 2)
  {
    boot_page_fill(pagptr, *tmpPtr);          //brzydkie rzutowanie 2X uint8_t na uint16_t
    tmpPtr++;                                 //wskaźnik zwiększa się o 2
  }
  boot_page_write(FlashAddr);                  //Zapis strony
  boot_spm_busy_wait();                        //Czekanie na wykonanie operacji
}

//send data to comport
void WriteCom(unsigned char dat)
{
  RS485Enable();
  UDRREG(COMPORTNo) = dat;
  while(!(UCSR0A & (1<<TXCBIT(COMPORTNo))))
    ;    //wait for send finish

  UCSR0A |= (1 << TXCBIT(COMPORTNo));
  RS485Disable();
}

uint8_t ReadCom_withLimitedWaiting()
{
  uint8_t cnt2 = 30;
  while(!DataInCom())
  {
    if(TIFRREG & (1<<OCF1A))    // Sprawdzanie przepełnienie timera T1
    {
      TIFRREG |= (1 << OCF1A);  // Zerowanie flagi przepełnienia

      cnt2--;                    // Odliczanie w dół. Jak dojdzie do zera to nastąpi wyjście z xModemu
      if(cnt2 == 0)              // Sprawdzanie czy nie nastąpił timeout
      {
        quit();                 // Opuszczenie bootloadera
      }
    }
  }
  return UDR0;
}


void resetStateMachine(void)
{
  stan        = sync;
  crc         = 0;
  protDaneWsk = 0;
//cnt         = TimeOutCnt; 
}

void sendBuf(uint8_t *buf, uint8_t len)
{
  uint8_t tmp2;
  crc=0;
  for (tmp=0; tmp<len; tmp++)
  {
    tmp2 = buf[tmp];
    WriteCom(tmp2);
    crc = _crc_xmodem_update(crc, tmp2);
  }
  WriteCom((uint8_t)(crc>>8));
  WriteCom((uint8_t)(crc & 0xFF));
  crc=0;
}


void wykonajRozkaz(void)
{
  uint8_t tmpCrcLo = (uint8_t)(crcObl & 0xFF);
  uint8_t tmpCrcHi = (uint8_t)(crcObl >>8);
  
  resetStateMachine();
  if ((tmpCrcHi != protCrcHi) ||(tmpCrcLo != protCrcLo))
  {       
    return;
  }
  stan = sync;
//cnt         = TimeOutCnt;
  if (adres != protAddr)
  {
    if (protRozkaz == rFLASH)
    {
      waitAndQuit();
    }        
    return;
  }
  if (protRozkaz == rFLASH)
  {
    stan = xModem;
    return;
  }
  if (protRozkaz == rHELLO)
  {
    sendBuf(helloBuf, HDR_LEN+8);
    return;
  }
  if (protRozkaz == rPING)
  {
    if (protDlDanych > PROT_BUF_LEN)
      protDlDanych = PROT_BUF_LEN;
    pingBuf[3] = protDlDanych;
    for (tmp=0; tmp<protDlDanych; tmp++)
      pingBuf[HDR_LEN+tmp] = protBuf[tmp];    
    sendBuf(pingBuf,  protDlDanych+HDR_LEN);
    return;
  }
  sendBuf(noCommandBuf,  4);
}

int main(void)
{
  unsigned char packNO;
  unsigned char crch, crcl;
  unsigned char li;

  cli();                             // disable interrupt
  
  OCR1A  = (uint16_t)(timeclk * (F_CPU  / (1024 * 1000.0f)));
  TCCR1A = 0;
  TCCR1B = (1 << WGM12)|(1 << CS12)|(1 << CS10);
  
  resetStateMachine();               // initialize state machine

  DDRB  = 0x01;
  PORTB = 0x3E; //3 - Adr2, 4 - Adr3, 5 - Adr4
  DDRC  = 0x3C;
  PORTC = 0x03;
  DDRD  = 0x0A; //0 - RXD, 1 - TXD, 3 - TxEn, 6 -  
  PORTD = 0x00;
  
  /**
   * Ustalanie adresu
   * bit 7, 6 = 0 dla sterowników rolet i światła
   */
  adres =  PINC & 0x03;
  adres |= ((PINB & 0x38)>>1);

  //initialize serial port UART0
  UCSR0A = 0;
  UCSR0B = (1 << RXENBIT(COMPORTNo))|(1 << TXENBIT(COMPORTNo));
  UCSR0C = (1 << USEURSEL)|(1 << UCSZ01)|(1 << UCSZ00);
  UBRR0H = 0;
  UBRR0L = 3;
  
  cnt = TimeOutCnt;             // Timeout (wg. częstotliwości zegara 1Hz)
  cl = 0;                       // Liczba poprawnych liter od hasła
  

//Wykonywanie poleceń. Tryb bootloadera.
  while(stan != xModem)         // Czekanie aż zostanie odebrany rozkaz rozpoczęcia transmisji xModemowej
  {
    if(TIFRREG & (1<<OCF1A))    // Sprawdzanie przepełnienie timera T1
    {
      TIFRREG |= (1 << OCF1A);  // Zerowanie

      cnt--;                    // Odliczanie w dół. Jak dojdzie do zera to nastąpi wyjście z xModemu
      PORTD ^= (uint8_t)(0x40); // Miganie diodą
      if(cnt == 0)              // Sprawdzanie czy nie nastąpił timeout
      {
        quit();                 // Opuszczenie bootloadera
      }
    }

    if(DataInCom())             // Sprawdzanie, czy doszedł jakiś rozkaz
    {
      tmp = ReadCom();
      crc = _crc_xmodem_update(crc, tmp);
      switch (stan)
      {
        case sync:
          if (tmp == SYNC)
            stan = addr;
          else
            crc=0;
          break;

        case addr:
          protAddr = tmp;
          stan = rozkaz;
          break;

        case rozkaz:
          protRozkaz = tmp;
          stan = dlDanych;
          break;

        case dlDanych:
          protDlDanych = tmp;
          stan = dane;
          if (protDlDanych == 0)
          {
            stan = crcHi;
            crcObl = crc;
          }
          break;

        case dane:
          if (protDaneWsk < PROT_BUF_LEN)
            protBuf[protDaneWsk] = tmp;
          protDaneWsk++;
          if (protDlDanych == protDaneWsk)
          {
            stan = crcHi;
            crcObl = crc;
          }
          break;

        case crcHi:
          protCrcHi = tmp;
          stan = crcLo;
          break;

        case crcLo:
          protCrcLo = tmp;
          wykonajRozkaz();
          break;
        default:
          break;
      }
    }
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
  packNO = 1;
  bufptr = 0;
  cnt = 0;
  FlashAddr = 0;
  while(1)
  {
    ch =  ReadCom_withLimitedWaiting();                          // get package number
    cl = ~ReadCom_withLimitedWaiting();
    if ((packNO == ch) && (packNO == cl))
    {
      uint16_t crc = 0;                                          // start calculate CRC
      for(li = 0; li < BUFFERSIZE; li++)                         // receive a full data frame
      {
        tmp = ReadCom_withLimitedWaiting();                      // read from uart
        buf[bufptr++] = tmp;                                     // write to temporary buffer
        crc = _crc_xmodem_update(crc, tmp);                      // calculate CRC
      }
      crch = ReadCom_withLimitedWaiting();                       // get checksum Hi
      crcl = ReadCom_withLimitedWaiting();                       // get checksum Lo
      ch = crc / 256;
      cl = crc % 256;
      if ((crch == ch) && (crcl == cl))
      {
        PORTD |= (uint8_t)(0x40);
        packNO++;
        while(bufptr > 0)                                        // receive one frame, write multi pages
        {
          write_one_page(&buf[BUFSIZE - bufptr]);
          FlashAddr += SPM_PAGESIZE;                             // modify Flash page address
          bufptr -= SPM_PAGESIZE;
          if (FlashAddr >= 2*BootStart)                          // BootStart is word number (not byte number)
            quit();
        }
        WriteCom(XMODEM_ACK);                                    // All OK send ACK
        cnt = 0;
      }
      else //CRC
      {
        //ask resend
        WriteCom(XMODEM_NAK);
        PORTD |= (uint8_t)(0x20);
        cnt++;
      }
    }
    else //PackNo
    {
      //ask resend
      WriteCom(XMODEM_NAK);
      PORTD |= (uint8_t)(0x20);
      cnt++;
    }
    //too much error, update abort
    if(cnt > 3)
    {
      PORTD |= (uint8_t)(0x20);
      break;
    }
    tmp = ReadCom_withLimitedWaiting();
    
    if (tmp == XMODEM_EOT)
      break;
  }
  WriteCom(XMODEM_NAK);

  quit();
  return 0;
}

//! quit boot mode and jump to user's application
void quit(void)
{
  boot_rww_enable();                           //enable application section
  (*((void(*)(void))PROG_START))();            //jump
}

//! wait 1 minute and jump to user's application
void waitAndQuit(void)
{
//  PORTD = 0x20;                               //LED 1 and 2 on
  cnt=120;
  while(1)
  {
    if(TIFRREG & (1<<OCF1A))                  //Sprawdzanie przepełnienia Timera T1
    {
      TIFRREG |= (1 << OCF1A);                //Kasowanie flagi przepełnienie timera T1
      cnt--;
      if(cnt == 0)
      {
        quit();
      }
    }
  }
}