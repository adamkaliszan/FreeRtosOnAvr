/*****************************************************************************
* vim:sw=2:ts=2:si:et
*
* Title      : Ds1305 RTC freeRtos Driver
* Author     : Adam Kaliszan
* Copyright: GPL V2
*
*This driver provides:
* - time read/set operation
* - alarm handling
* - chip memory read/write
* - api for Fat32 library
*Driver uses mutexex and is condition race free. Function can be invoken in any thread.
*****************************************************************************/
//@{


#ifndef DS1305_H
#define DS1305_H

#include <inttypes.h>
#include "spiXmega.h"
#include "hardwareConfig.h"


void spiEnableDS1305(void)  __attribute__ ((weak));
void spiDisableDS1305(void) __attribute__ ((weak));

struct timeBCD
{
  uint8_t seconds;                    /// Sekundy:        Odczyt(0x00), Zapis(0x80)
  uint8_t minutes;                    /// Minuty:         Odczyt(0x01), Zapis(0x81)
  uint8_t hour;                       /// Godzina:        Odczyt(0x02), Zapis(0x82) (bit 6 oznacza system 12h/24h)
  uint8_t day;                        /// Dzień(1-7):     Odczyt(0x03), Zapis(0x83) (pierwsze 3 bity, pozostałe mają wartość 0)
  uint8_t dateDay;                    /// Dzień miesiąca: Odczyt(0x04), Zapis(0x84)
  uint8_t dateMonth;                  /// Miesiąc:        Odczyt(0x05), Zapis(0x85)
  uint8_t dateYear;                   /// Rok:            Odczyt(0x06), Zapis(0x86) (należy dopisać na początek 20)
};

#if USE_DECODED_TIME_STRUCT
struct timeDecoded
{
  struct                              /// Sekundy:        Odczyt(0x00), Zapis(0x80)
  {
    uint8_t cJedn  :4;                // Cyfra jednostek             
    uint8_t cDzies :3;                // Cyfra dziesiętna (3 bity)
  } seconds;
  
  struct                              /// Minuty:         Odczyt(0x01), Zapis(0x81)
  {
    uint8_t cJedn  :4;                // Cyfra jednostek             
    uint8_t cDzies :3;                // Cyfra dziesiętna (3 bity)
  } minutes;
  
  union                               /// Godzina:        Odczyt(0x02), Zapis(0x82)
  {
    struct                            // System 12 godzinny                           
    {
      uint8_t cJedn  :4;              //   Cyfra jednostek             
      uint8_t cDzies :1;              //   Cyfra dziesiętna (1 bit)
      uint8_t AmPm   :1;              //   0 - Am, 1 - Pm
      uint8_t syst24 :1;              //   0 - system 12 godzinny, 1 - system 24 godzinny
    } syst12;
    struct                            // System 24 godzinny
    {
      uint8_t cJedn  :4;              //   Cyfra jednostek             
      uint8_t cDzies :2;              //   Cyfra dziesiętna (2 bity)
      uint8_t syst24 :1;              //   0 - system 12 godzinny, 1 - system 24 godzinny      
    } syst24;
  } hours;
  
  uint8_t day;                        /// Dzień(1-7):     Odczyt(0x03), Zapis(0x83) (pierwsze 3 bity, pozostałe mają wartość 0)
  
  struct                              /// Dzień miesiąca: Odczyt(0x04), Zapis(0x84)
  {
    uint8_t cJedn  :4;                // Cyfra jednostek             
    uint8_t cDzies :2;                // Cyfra dziesiętna (2 bity)
  } dateDay;
  
  struct                              /// Miesiąc:        Odczyt(0x05), Zapis(0x85)
  {
    uint8_t cJedn  :4;                // Cyfra jednostek             
    uint8_t cDzies :2;                // Cyfra dziesiętna (2 bity ??? tak było w specyfikacji)
  } dateMonth; 

  struct                              /// Rok:            Odczyt(0x06), Zapis(0x86) (należy dopisać na początek 20)
  {
    uint8_t cJedn  :4;                // Cyfra jednostek             
    uint8_t cDzies :4;                // Cyfra dziesiętna (4 bity)    
  } dateYear;
};

union time
{
  struct timeBCD     BCD;
  struct timeDecoded Decoded;
};
#endif /* USE_DECODED_TIME_STRUCT */

typedef struct timeBCD     timeBCD_t;
#if USE_DECODED_TIME_STRUCT
typedef struct timeDecoded timeDecoded_t;
typedef union  time        time_t;
#endif /* USE_DECODED_TIME_STRUCT */


//------------------ Functions       ---------------------------------

void readTimeBCD     (timeBCD_t     *time);

#if USE_DECODED_TIME_STRUCT
void readTimeDecoded (timeDecoded_t *time);
void readTime        (time_t        *time);
#endif /* USE_DECODED_TIME_STRUCT */

void setTimeBCD      (timeBCD_t     *time);
#if USE_DECODED_TIME_STRUCT
void setTimeDecoded  (timeDecoded_t *time);
void setTime         (time_t        *time);
#endif /* USE_DECODED_TIME_STRUCT */

/**
 * Uruchamia kwarc, wyłącza blokadę przed zapisem do pamięci
 */
void ds1305start(void);

/**
 * Zapisuje do pamięci nieulotnej układu RTC.
 * @param addr   - adres pierwszej komórki do zapisu (0-95)
 * @param length - liczba bajtów do zapisu
 * @param *data  - wskaźnik do tablicy z bajtami do zapisu
 */
uint8_t ds1305writeMem     (uint8_t addr, uint8_t length, uint8_t *data);

/**
 * Odczytuje z pamięci nieulotnej układu RTC.
 * @param addr   - adres pierwszej komórki do odczytu (0-95)
 * @param length - liczba bajtów do odczytu
 * @param *data  - wskaźnik do tablicy, w której zostaną zapisane odczytane bajty 
 */
uint8_t ds1305readMem      (uint8_t addr, uint8_t length, uint8_t *data);

#endif
//@}
