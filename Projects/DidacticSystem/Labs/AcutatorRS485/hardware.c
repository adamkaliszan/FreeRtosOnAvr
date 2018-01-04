#include "hardware.h"


void hardwareInit(void)
{
  DDRB  = 0x07; //0 - Led3  , 1 - Led2  , 2 - Led1
  PORTB = 0x38; //3 - Adr3, 4 - Adr4, 5 - Adr5
  DDRC  = 0x00; //0 - Adr0, 1 - Adr1, 2 - Key1, 3 - Key2
  PORTC = 0x3F; //4 - Key3, 5 - Key4
  DDRD  = 0xEE; //0 - RXD, 1 - TXD, 2 - !RxEn, 3 - TxEn,
  PORTD = 0x00; //5 - ExtLed1, 6 - ExtLed2, 7 - Led4)
}

void ledOn(uint8_t ledNo)
{
  switch(ledNo)
  {
    case 0:
      led1on();
      break;
    case 1:
      led2on();
      break;
    case 2:
      led3on();
      break;
    case 3:
      led4on();
      break;
    default:
      break;
  }
}

void ledOff(uint8_t ledNo)
{
  switch(ledNo)
  {
    case 0:
      led1off();
      break;
    case 1:
      led2off();
      break;
    case 2:
      led3off();
      break;
    case 3:
      led4off();
      break;
    default:
      break;
  }
}
void ledToggle(uint8_t ledNo)
{
  switch (ledNo)
  {
    case 0:
      led1toggle();
      break;
    case 1:
      led2toggle();
      break;
    case 2:
      led3toggle();
      break;
    case 3:
      led4toggle();
      break;
    default:
      break;
  }
}

char readKey(uint8_t keyNo)
{
  switch (keyNo)
  {
    case 0: return readKey1();
    case 1: return readKey2();
    case 2: return readKey3();
    case 3: return readKey4();
    default: return 1;
  }
}

inline void led1on(void)
{
  PORTB |=  0x04;
}
inline void led1off(void)
{
  PORTB &= ~0x04;
}
inline void led1toggle(void)
{
  PORTB ^= 0x04;
}

inline void led2on(void)
{
  PORTB |=  0x02;
}
inline void led2off(void)
{
  PORTB &= ~0x02;
}
inline void led2toggle(void)
{
  PORTB ^= 0x02;
}

inline void led3on(void)
{
  PORTB |=  0x01;
}
inline void led3off(void)
{
  PORTB &= ~0x01;
}
inline void led3toggle(void)
{
  PORTB ^= 0x01;
}

inline void led4on(void)
{
  PORTD |=  0x80;
}
inline void led4off(void)
{
  PORTD &= ~0x80;
}
inline void led4toggle(void)
{
  PORTD ^= 0x80;
}


inline char readKey1(void)
{
  return PINC & 0x04;
}

inline char readKey2(void)
{
  return PINC & 0x08;
}

inline char readKey3(void)
{
  return PINC & 0x10;
}

inline char readKey4(void)
{
  return PINC & 0x20;
}

