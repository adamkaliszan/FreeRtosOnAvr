#include "hardware.h"


void hardwareInit(void)
{
  DDRB  = 0x07; //0 - P3  , 1 - P2  , 2 - P1
  PORTB = 0x38; //3 - Adr3, 4 - Adr4, 5 - Adr5
  DDRC  = 0x00; //0 - Adr0, 1 - Adr1, 2 - KlRolety1Up, 3 - klRolety1Down
  PORTC = 0x3F; //4 - KlRolety2Up, 5 - KlRolety2Down
  DDRD  = 0xEE; //0 - RXD, 1 - TXD, 2 - !RxEn, 3 - TxEn, 
  PORTD = 0x00; //5 - Led1, 6 - Led2, 7 - P4)
  /*Ustalanie adresu
    bit 7, 6 = 0 dla sterowników rolet i światła
   */
  adres =  (PINB & 0x38) >> 1;
  adres |= (PINC & 0x03);
}

void roletawGore(uint8_t nrRolety)
{
  if (nrRolety == 0)
    roleta1wGore();
  else
    roleta2wGore();
}

void roletawDol(uint8_t nrRolety)
{
  if (nrRolety == 0)
    roleta1wDol();
  else
    roleta2wDol();
}

void roletaStop(uint8_t nrRolety)
{
  if (nrRolety == 0)
    roleta1Stop();
  else
    roleta2Stop();
}

inline void roleta1wGore(void)
{
  PORTB &= ~0x02;
  PORTB |=  0x04;
}

inline void roleta1wDol(void)
{
  PORTB &= ~0x04;
  PORTB |=  0x02;	
}

inline void roleta1Stop(void)
{
  PORTB &= ~0x06;
}

inline void roleta2wGore(void)
{
  PORTD &= ~0x80;
  PORTB |=  0x01;
}

inline void roleta2wDol(void)
{
  PORTB &= ~0x01;
  PORTD |=  0x80;
}

inline void roleta2Stop(void)
{
  PORTB &= ~0x01;
  PORTD &= ~0x80;
}

inline void Led1On(void)
{
  PORTD |= 0x20;
}

inline void Led1Toggle(void)
{
  PORTD ^= 0x20;
}

inline void Led1Off(void)
{
  PORTD &= ~0x20;
}

inline void Led2On(void)
{
  PORTD |= 0x40;	
}

inline void Led2Toggle(void)
{
  PORTD ^= 0x40;
}

inline void Led2Off(void)
{
  PORTD &= ~0x40;	
}

inline char czytKlawiszRol1wGore(void)
{
  return PINC & 0x04;
}

inline char czytKlawiszRol1wDol(void)
{
  return PINC & 0x08;
}

inline char czytKlawiszRol2wGore(void)
{
  return PINC & 0x10;
}

inline char czytKlawiszRol2wDol(void)
{
  return PINC & 0x20;
}

