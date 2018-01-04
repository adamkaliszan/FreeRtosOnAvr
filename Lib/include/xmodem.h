/**
 * XModem Transmit/Receive z obsługą CRC.
 * Działa tylko z ram plikami. Wskaźnik z pliku jest jego podręcznym buforem.
 * Zapis i odczyt z ram pliku odbywa się bezpiśrednio z pominięciem api do zapisu i odczytu.
 * Użycie api ogranicza się do ustawiania wskaźnika na nowo przydzielone obszary pamięci.
 */
#ifndef XMODEM_H
#define XMODEM_H

#include "main.h"
#include "ramdysk.h"

//@{

/**
 * Struktura do obsługi protokołu Xmodem
 *
 */
struct xmodemUchwyt
{
  FILE                  strumien;               /// Strumień do obsługi transmisji danych.
  struct ramPlikFd      *fd;                    /// Deskryptor ram pliku. Tutaj dane są zapisywane przy odbiorze lub z tąd pobiera się dane do wysłania.
                                                /// Działa on też jako bufor tymczasowy.
//  uint8_t (*zapis)(uint8_t, void*, uint16_t);   /// Wskaźnik na funkcję obsługującą zapis do strumienia.
//  uint8_t (*odczyt)(uint8_t*, void*, uint16_t); /// Wskaźnik na funkcję czytającą ze strumienia.
  
//  void *strumienWy;                             /// Wskaźnik do strumienia przez który wysyłane są dane. Typ strumienia zależy od funkcji wysyłającej dane.
//  void *strumienWe;                             /// Wskaźnik do strumienia przez który odbierane są dane. Typ strumienia zależy od funkcji odbierającej dane.
  
  uint8_t               crcLo;                  /// Mniej znaczący bajt odebranej sumy kontrolnej.
  uint8_t               crcHi;                  /// Bardziej znaczący bajt odebranej sumy kontrolnej.
  uint8_t               nrBlokuZdalny;          /// Odczytany numer 128 bajtowego bloku. Pierwszy zaczyna się od 1.
  uint8_t               nrBlokuZdalnyNeg;       /// Odczytany, zanegowany numer 128 bajtowego bloku. Pierwszy zaczyna się od 1.
  uint8_t               nrBloku;                /// Lokalny numer bloku
  uint16_t              crcLokalne;             /// CRC obliczane przez funkcję lokalną.
};

/**
 * Tworzy uchwyt do obsługi Xmodemu.
 * @param [out] *uchwyt    - wskaźnik do struktury uchwytu protokołu xmodem, która zostanie odpowiednio skonfigorowana
 * @param [in]  *fd        - wskaźnik do deskryptora ram pliku
 * @return 0 - wszystko OK 
 */
//uint8_t xmodemUstawUchwyt(struct xmodemUchwyt *uchwyt, struct ramPlikFd *fd);

/**
 * Rozpoczyna odbiór danych, a następnie obsługuje pobieranie danych.
 * Wymagane jest wcześniej utworzenie struktury xmodemUchwyt, w której są funkcje
 * do odbierania i wysyłania znaków oraz deskryptor do ram pliku, w którym zapisane są dane.
 * Ram plik działą też jako bufor tymczasowy
 * @param [in,out] *uchwyt - wskaźnik do struktury uchwytu protokołu xmodem.
 * @return 0 - wszystko OK 
 */
//uint8_t xmodemOdbierz(struct xmodemUchwyt *uchwyt);

/**
 * Rozpoczyna i obsługuje wysyłanie danych.
 * Wymagane jest wcześniej utworzenie struktury xmodemUchwyt, w której są funkcje
 * do odbierania i wysyłania znaków oraz deskryptor do ram pliku, w którym zapisane są dane.
 * Ram plik działą też jako bufor tymczasowy
 * @param [in,out] *uchwyt - wskaźnik do struktury uchwytu protokołu xmodem.
 * @return 0 - wszystko OK 
 */
//uint8_t xmodemWyslij(struct xmodemUchwyt *uchwyt);
//@}
#endif

