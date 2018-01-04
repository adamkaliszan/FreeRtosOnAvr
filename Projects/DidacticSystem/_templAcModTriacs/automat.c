#include "automat.h"

#define T_START       5
#define T_START_CONT  40
#define T_STOP        5

uint8_t automatStanowKlawiszy(uint8_t klGoraOff, uint8_t klDolOff, t_stan_klawiszy *stan)
{
  uint8_t wiadomosc = 0;

  if (klGoraOff)
  {
    if ((stan->czynnosc == w_gore) && (stan->klGora_off == T_STOP))
    {
      stan->czynnosc = bezczynny;
      wiadomosc = 0x40;
    }
    stan->klGora_on = 0;
    stan->klGora_off++;
  }
  else
  {
    if (stan->klGora_on == T_START)
    {
      if (stan->czynnosc != bezczynny)
      {
        stan->czynnosc = bezczynny;
        wiadomosc = 0x40;
      }
      else
      {
        stan->czynnosc = do_konca_w_gore;
        wiadomosc = 0xBF;
      }
    }
    if (stan->klGora_on == T_START_CONT)
    {
      stan->czynnosc = w_gore;
    }
    stan->klGora_on++;
    stan->klGora_off = 0;
  }

  if (klDolOff)
  {
    if ((stan->czynnosc == w_dol) && (stan->klDol_off == T_STOP))
    {
      stan->czynnosc = bezczynny;
      wiadomosc = 0x40;
    }
    stan->klDol_on = 0;
    stan->klDol_off++;
  }
  else
  {
    if (stan->klDol_on == T_START)
    {
      if (stan->czynnosc != bezczynny)
      {
        stan->czynnosc = bezczynny;
        wiadomosc = 0x40;
      }
      else
      {
        stan->czynnosc = do_konca_w_dol;
        wiadomosc = 0x3F;
      }
    }
    if (stan->klDol_on == T_START_CONT)
    {
      stan->czynnosc = w_dol;
    }
    stan->klDol_on++;
    stan->klDol_off = 0;
  }
  
  if (stan->klDol_on == 255)
	  stan->klDol_on = 254;
  if (stan->klGora_on == 255)
	  stan->klGora_on = 254;
  
  if (stan->klDol_off == 255)
	  stan->klDol_off = 254;
  if (stan->klGora_off == 255)
	  stan->klGora_off = 254;

  return wiadomosc;
}
