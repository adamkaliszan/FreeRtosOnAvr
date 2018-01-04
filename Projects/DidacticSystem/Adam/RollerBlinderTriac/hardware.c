#include "hardware.h"

void hardwareInit(void)
{
  DDRB  = 0x01;
  PORTB = 0x3E; //3 - Adr2, 4 - Adr3, 5 - Adr4
  DDRC  = 0x3C;
  PORTC = 0x03;
  DDRD  = 0x0A; //0 - RXD, 1 - TXD, 3 - TxEn, 6 -  
  PORTD = 0x00;
  /*Ustalanie adresu
    bit 7, 6 = 0 dla sterowników rolet i światła
   */
  adres =  PINC & 0x03;
  adres |= ((PINB & 0x38)>>1);
}

void roleta1wGore(void)
{
  PORTC &= ~0x20;
  PORTC |=  0x01;
}

void roleta1wDol(void)
{
  PORTC &= ~0x10;
  PORTC |=  0x20;
}

void roleta1Stop(void)
{
  PORTC &= ~0x30;
}

uint8_t czytKlawiszRol1wGore(void)
{
  return PIND & 0x40;
}

uint8_t czytKlawiszRol1wDol(void)
{
  return PIND & 0x80;
}

void roleta2wGore(void)
{
  PORTC &= ~0x08;
  PORTC |=  0x04;
}

void roleta2wDol(void)
{
  PORTC &= ~0x08;
  PORTC |=  0x04;	
}

void roleta2Stop(void)
{
  PORTC &= ~0x0C;
}

uint8_t czytKlawiszRol2wGore(void)
{
  return PINB & 0x02;
}

uint8_t czytKlawiszRol2wDol(void)
{
  return PINB & 0x04;
}

void wczytajUstawienia(uint8_t konfiguracja)
{
  if (konfiguracja & 0x80)
    sterowanie[1].wlaczona = 1;
  else
  {
    sterowanie[1].wlaczona = 0;
    roleta1Stop();
  }
  
  if (konfiguracja & 0x08)
    sterowanie[0].wlaczona = 1;
  else
  {
    sterowanie[0].wlaczona = 0;
    roleta2Stop();
  } 

  sterowanie[0].stop = roleta1Stop;
  sterowanie[1].stop = roleta2Stop;
  
  if (konfiguracja & 0x44 == 0)
  {
    if (konfiguracja & 0x01)
    {
      sterowanie[0].czytKlawiszGora = czytKlawiszRol1wGore;
      sterowanie[0].czytKlawiszDol  = czytKlawiszRol1wDol;
    }
    else
    {
      sterowanie[0].czytKlawiszGora = czytKlawiszRol1wDol;
      sterowanie[0].czytKlawiszDol  = czytKlawiszRol1wGore;      
    }
    if (konfiguracja & 0x10)
    {
      sterowanie[1].czytKlawiszGora = czytKlawiszRol2wGore;
      sterowanie[1].czytKlawiszDol  = czytKlawiszRol2wDol;
    }
    else
    {
      sterowanie[1].czytKlawiszGora = czytKlawiszRol2wDol;
      sterowanie[1].czytKlawiszDol  = czytKlawiszRol2wGore;
    }
  }
  else
  {
    if (konfiguracja & 0x01)
    {
      sterowanie[0].czytKlawiszGora = czytKlawiszRol2wGore;
      sterowanie[0].czytKlawiszDol  = czytKlawiszRol2wDol;
    }
    else
    {
      sterowanie[0].czytKlawiszGora = czytKlawiszRol2wDol;
      sterowanie[0].czytKlawiszDol  = czytKlawiszRol2wGore;
    }
    if (konfiguracja & 0x10)
    {
      sterowanie[1].czytKlawiszGora = czytKlawiszRol1wGore;
      sterowanie[1].czytKlawiszDol  = czytKlawiszRol1wDol;
    }
    else
    {
      sterowanie[1].czytKlawiszGora = czytKlawiszRol1wDol;
      sterowanie[1].czytKlawiszDol  = czytKlawiszRol1wGore;
    }
  }
  
  if (konfiguracja & 0x02)
  {
    sterowanie[0].gora = roleta1wGore;
    sterowanie[0].dol  = roleta1wDol;
  }
  else
  {
    sterowanie[0].gora = roleta1wDol;
    sterowanie[0].dol  = roleta1wGore;
  }
  
  if (konfiguracja & 0x20)
  {
    sterowanie[1].gora = roleta2wGore;
    sterowanie[1].dol  = roleta2wDol;
  }
  else
  {
    sterowanie[1].gora = roleta2wDol;
    sterowanie[1].dol  = roleta2wGore;
  }
}

