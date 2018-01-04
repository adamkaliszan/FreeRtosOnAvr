#include "hc12.h"

void HC12setAtMode()
{
  PORTD.OUTCLR = PIN0_bm;
}

void HC12setTransparentMode()
{
  PORTD.OUTSET =  PIN0_bm;
}

