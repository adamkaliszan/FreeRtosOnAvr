#ifndef AUTOMAT_H
#define AUTOMAT_H

#include "main.h"


typedef enum
{
  bezczynny,
  do_konca_w_gore,
  do_konca_w_dol,
  w_gore,
  w_dol
} t_czynnosc;

typedef struct
{
  uint8_t klGora_on;
  uint8_t klDol_on;

  uint8_t klGora_off;
  uint8_t klDol_off;
  t_czynnosc czynnosc;
}
t_stan_klawiszy;

uint8_t automatStanowKlawiszy(uint8_t klGoraOff, uint8_t klDolOff, t_stan_klawiszy *stan);

#endif
