
//@{
#ifndef SPI_H
#define SPI_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
//#include "hardware.h"
//#include "hardwareConfig.h"


xSemaphoreHandle  xSemaphoreSpiSS;    /// Flaga blokująca jednoczesny dostęp do magistrali wielu urządzeniom

/**
 * Inicjalizacja magistrali SPI, utworzenie semaforów
 */
void    spiInit( void (*disableAllSpiDevicesFun)(void));
void    spiInitENC( void (*disableAllSpiDevicesFun)(void));

void spiSetCPHA(void);
void spiSetCPHAENC(void);
void spiClearCPHA(void);
void spiClearCPHAENC(void);
void spiSetCPOL(void);
void spiSetCPOLENC(void);
void spiClearCPOL(void);
void spiClearCPOLENC(void);


/**
 * Wyślij do magistrali SPI. Po rozpoczęciu transmisji przełączany jest wątek.
 * Warto stosować w przypadku wątków o niskich priorytetach lub wolnej transmisji SPI
 * @param uint8_t bajt do wysłania
 * @return odebrany bajt z magistrali SPI
 */
uint8_t spiSend(uint8_t data) __attribute__ ((weak));



/**
 * Wyślij do magistrali SPI. Sotosowana jest blokada wirująca. 
 * Warto stosować gdy SPI działa z dużą szybkością 
 * (czas wysłania jest mniejszy od 2X czas przełączenia wątku)
 * @param uint8_t bajt do wysłania
 * @return odebrany bajt z magistrali SPI
 */
uint8_t spiSendSpinBlock(uint8_t data) __attribute__ ((weak));

/**
 * Zajmuje magistralę SPI.
 * Po zajęciu magistrali należy wybrać odpowiednią linię adresową do urządzenia z którym się komunikujemy
 * W momencie, gdy magistrala jest zajęta, inne zadania nie mogą z niej korzystać.
 * Uwaga na zakleszczenia.
 */
void spiTake(void);

/**
 * Zwalnia magistralę SPI.
 * Przed zwolnieniem maigistrali należy odłączyć od niej wszystkie urządzenia
 */
void spiGive(void);

#endif
//@}
