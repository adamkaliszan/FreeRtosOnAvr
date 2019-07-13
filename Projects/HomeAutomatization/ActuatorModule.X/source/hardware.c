#include "hardware.h"

void hardwareInit(void)
{
  DDRB  = 0x07; //0 - P3  , 1 - P2  , 2 - P1
  PORTB = 0x38; //3 - Adr3, 4 - Adr4, 5 - Adr5
  DDRC  = 0x00; //0 - Adr0, 1 - Adr1, 2 - key roller 1 up, 3 - key roller 1 down
  PORTC = 0x3F; //4 - key roller 2 up, 5 - key roller 2 down
  DDRD  = 0xEE; //0 - RXD, 1 - TXD, 2 - !RxEn, 3 - TxEn, 
  PORTD = 0x00; //5 - Led1, 6 - Led2, 7 - P4)

  /**
   *  Update device address
   *  bits 7, 6 = 0  for rollers and light drivers
   */
  address =  (PINB & 0x38) >> 1;
  address |= (PINC & 0x03);
}

void rollerBlinderUp(uint8_t rollerNo)
{
  if (rollerNo == 0)
    roller1up();
  else
    roller2up();
}

void rollerBlinderDown(uint8_t rollerNo)
{
  if (rollerNo == 0)
    roller1down();
  else
    roller2down();
}

void rollerBlinderStop(uint8_t rollerNo)
{
  if (rollerNo == 0)
    roller1stop();
  else
    roller2stop();
}

inline void roller1up(void)
{
  PORTB &= ~0x02;
  PORTB |=  0x04;
}

inline void roller1down(void)
{
  PORTB &= ~0x04;
  PORTB |=  0x02;	
}

inline void roller1stop(void)
{
  PORTB &= ~0x06;
}

inline void roller2up(void)
{
  PORTD &= ~0x80;
  PORTB |=  0x01;
}

inline void roller2down(void)
{
  PORTB &= ~0x01;
  PORTD |=  0x80;
}

inline void roller2stop(void)
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

inline char readKeyRoller1up(void)
{
  return PINC & 0x04;
}

inline char readKeyRoller1down(void)
{
  return PINC & 0x08;
}

inline char readKeyRoller2up(void)
{
  return PINC & 0x10;
}

inline char readKeyRoller2down(void)
{
  return PINC & 0x20;
}

