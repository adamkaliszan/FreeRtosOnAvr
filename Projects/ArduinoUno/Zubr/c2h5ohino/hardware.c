#include "hardware.h"

void hardwareInit(void)
{
  DDRB  = 0x20;
  PORTB = 0x20;

  DDRD  = 0x02;
}

