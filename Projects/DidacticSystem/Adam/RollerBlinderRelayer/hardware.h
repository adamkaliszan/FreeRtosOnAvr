#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
#include "main.h"
#include "../../Lib/include/protocol1.h"

#define TxStart() (PORTD |=  0x0C)
#define TxStop()  (PORTD &=  0xF3)

extern uint8_t adres;
extern char bHelloResp[];

/**
 * Inicjalizacja sprzętu
 */
void hardwareInit(void);

void roletawGore(uint8_t nrRolety);
void roletaStop(uint8_t nrRolety);
void roletawDol(uint8_t nrRolety);

/**
 * Włącznenie przekaźnika - rozpoczęcie podnoszenia rolety 1
 */
void roleta1wGore(void);

/**
 * Włącznenie przekaźnika - rozpoczęcie opuszczania rolety 1
 */
void roleta1wDol(void);

/**
 * Wyłącznenie przekaźników - zatrzymanie rolety 1
 */
void roleta1Stop(void);

/**
 * Włącznenie przekaźnika - rozpoczęcie podnoszenia rolety 2
 */
void roleta2wGore(void);

/**
 * Włącznenie przekaźnika - rozpoczęcie opuszczania rolety 2
 */
void roleta2wDol(void);

/**
 * Wyłącznenie przekaźników - zatrzymanie rolety 2
 */
void roleta2Stop(void);

/**
 * Zapalenie diody kontrolej 1
 */
void Led1On(void);

/**
 * Zmiana stanu diody kontrolej 1
 */
void Led1Toggle(void);

/**
 * Gaszenie diody kontrolej 1
 */
void Led1Off(void);

/**
 * Zapalenie diody kontrolej 2
 */
void Led2On(void);

/**
 * Zmiana stanu diody kontrolej 2
 */
void Led2Toggle(void);

/**
 * Gaszenie diody kontrolej 2
 */
void Led2Off(void);

/**
 * Odczyt stanu klawisza do podnoszenie rolety 1
 */
char czytKlawiszRol1wGore(void);

/**
 * Odczyt stanu klawisza do opuszczania rolety 1
 */
char czytKlawiszRol1wDol(void);

/**
 * Odczyt stanu klawisza do podnoszenie rolety 2
 */
char czytKlawiszRol2wGore(void);

/**
 * Odczyt stanu klawisza do opuszczania rolety 2
 */
char czytKlawiszRol2wDol(void);
#endif
