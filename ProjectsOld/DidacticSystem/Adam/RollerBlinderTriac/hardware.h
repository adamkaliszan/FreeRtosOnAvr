#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
#include "main.h"
#include "../../freeRtos/Lib/include/protocol1.h"


/*
 * Opis sprzętu.
 * PORTB 
 * - PB0 - sterowanie przekaźnikiem (tylko dla wersji z 2 triakami)
 * - PB1 - Kl2G    (Klawisz roleta 2 góra)
 * - PB2 - Kl2D    (Klawisz roleta 2 dół)
 * - PB3 - ADDR 2
 * - PB4 - ADDR 3
 * - PB5 - ADDR 4
 * PORTC
 * - PC0 - Addr 0
 * - PC1 - Addr 1
 * - PC2 - Triak 3 (Roleta 2 góra)
 * - PC3 - Triak 4 (Roleta 2 dół)
 * - PC4 - Triak 1 (Roleta 1 góra)
 * - PC5 - Triak 2 (Roleta 1 dół)
 * PORTD
 * - PD0 - RxD
 * - PD1 - TxD
 * - PD3 - TxEN
 * - PD6 - Kl1G    (Klawisz roleta 1 góra)
 * - PD7 - Kl1D    (Klawisz roleta 1 dół)
 */

#define TxStart() (PORTD |=  0x08)
#define TxStop()  (PORTD &=  0xF7)

#if X2
#define POWER_ON 
#define POWER_OFF
#else
#define POWER_ON    PORTB |= 0x01;
#define POWER_OFF   PORTB &= (~0x01);
#endif

struct funkcje
{
  uint8_t wlaczona;
  void (*stop)(void);
  void (*gora)(void);
  void (*dol)(void);
  uint8_t (*czytKlawiszGora)(void);
  uint8_t (*czytKlawiszDol)(void);
};


extern uint8_t adres;
extern uint8_t settings;
extern char bHelloResp[];
struct funkcje sterowanie[2];

/**
 * Inicjalizacja sprzętu
 */
void hardwareInit(void);

/**
 * Wczytuje ustawienia
 * @param konfiguracja Roleta 1 (bity 3-0), Roleta 2 (bity 7-4)
 *         - bit 7 (3) - włączona obsługa rolety 2 (1)
 *         - bit 6 (2) - zamiana klawiszy rolet roletę 1 obsługują klawisze od rolety 2 i odwrotnie
 *         - bit 5 (1) - odwrócenie faz silnika w rolecie 2 (1)
 *         - bit 4 (0) - odwrócenie klawisze góra/dół w rolecie 2 (1)
 */
void wczytajUstawienia(uint8_t konfiguracja);


/**
 * Włącznenie triaka - rozpoczęcie podnoszenia rolety 1
 */
void roleta1wGore(void);

/**
 * Włącznenie triaka - rozpoczęcie podnoszenia rolety 2
 */
void roleta2wGore(void);

/**
 * Włącznenie triaka - rozpoczęcie opuszczania rolety 1
 */
void roleta1wDol(void);

/**
 * Włącznenie triaka - rozpoczęcie opuszczania rolety 2
 */
void roleta2wDol(void);

/**
 * Wyłącznenie triaków - zatrzymanie rolety 1
 */
void roleta1Stop(void);

/**
 * Wyłącznenie triaków - zatrzymanie rolety 2
 */
void roleta2Stop(void);

/**
 * Odczyt stanu klawisza do podnoszenie rolety 1
 */
uint8_t czytKlawiszRol1wGore(void);

/**
 * Odczyt stanu klawisza do podnoszenie rolety 2
 */
uint8_t czytKlawiszRol2wGore(void);

/**
 * Odczyt stanu klawisza do opuszczania rolety 1
 */
uint8_t czytKlawiszRol1wDol(void);

/**
 * Odczyt stanu klawisza do opuszczania rolety 2
 */
uint8_t czytKlawiszRol2wDol(void);


void powerOn(void);
void powerOff(void);


#endif
