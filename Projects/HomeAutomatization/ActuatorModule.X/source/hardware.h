#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>

#define TxStart() (PORTD |=  0x0C)
#define TxStop()  (PORTD &=  0xF3)

extern uint8_t address;
extern char bHelloResp[];

/**
 * Inicjalizacja sprzętu
 */
void hardwareInit(void);

void rollerBlinderUp(uint8_t nrRolety);
void rollerBlinderStop(uint8_t nrRolety);
void rollerBlinderDown(uint8_t nrRolety);

/**
 * Włącznenie przekaźnika - rozpoczęcie podnoszenia rolety 1
 */
void roller1up(void);

/**
 * Włącznenie przekaźnika - rozpoczęcie opuszczania rolety 1
 */
void roller1down(void);

/**
 * Wyłącznenie przekaźników - zatrzymanie rolety 1
 */
void roller1stop(void);

/**
 * Włącznenie przekaźnika - rozpoczęcie podnoszenia rolety 2
 */
void roller2up(void);

/**
 * Włącznenie przekaźnika - rozpoczęcie opuszczania rolety 2
 */
void roller2down(void);

/**
 * Wyłącznenie przekaźników - zatrzymanie rolety 2
 */
void roller2stop(void);

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
char readKeyRoller1up(void);

/**
 * Odczyt stanu klawisza do opuszczania rolety 1
 */
char readKeyRoller1down(void);

/**
 * Odczyt stanu klawisza do podnoszenie rolety 2
 */
char readKeyRoller2up(void);

/**
 * Odczyt stanu klawisza do opuszczania rolety 2
 */
char readKeyRoller2down(void);
#endif
