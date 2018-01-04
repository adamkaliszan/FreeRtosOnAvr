#ifndef RAMDYSK_H
#define RAMDYSK_H

#include "main.h"
#include <stdio.h>

#define L_KLASTROW     128
#define KLASTER_OFFSET 256-L_KLASTROW

#define dataPtr(KLASTER,OFFSET) (uint8_t *)((((KLASTER)+KLASTER_OFFSET)<<8) + (OFFSET)) //Makro zamienia lokalizację w pliku na adres pamięci

uint8_t klastry[128];        /// Rozmiar klastra 256 bajtów -- łącznie system plików obsługuje 32 kB

/**
 * Struktura zapisana w tablicy Fat dla każdego pliku.
 * Jest zapisana w klastrach. Im więcej struktur takich, tym mniej miejsca na zawartość plików
 * Rozmiar struktury ma 16 bajtów (16 struktor w 1 klastrze)
 */
struct ramPlik
{
  uint8_t  pierwszyKlaster;  /// Numer pierwszego klastra
  uint8_t  rozmiarLo;        /// Liczba zapisanych danych (mniej znaczący bajt)
  uint8_t  rozmiarHi;        /// Liczba zapisanych danych (bardziej znaczący bajt) 
  uint8_t  lAktOtw;          /// Liczba procesów, które otwały ten plik do odczytu

  char     nazwa[8];         /// Nazwa pliku
  uint32_t dataMod;          /// Data modyfikacji pliku
};

/**
 * Deskryptor dla otwartego pliku.
 * Kilka deskryptorów może być związanych z tym samym plikiem
 */
struct ramPlikFd
{
  uint8_t  *Wsk;             /// Wskaźnik z adresem pamięci, do którego będzie zapis lub odczyt
  uint8_t  IndLo;            /// Mniej znaczący bajt indeksu pliku (zapisu lub odczytu)
  uint8_t  IndHi;            /// Bardziej znaczący bajt indeksu pliku
//  uint8_t  ostatniKlaster;   /// Numer ostatniego klastera

  struct ramPlik *wpis;    
};

/**
 * Inicjalizacja systemu plików
 */
void ramDyskInit(void);

/**
 * Tworzy nową strukturę pliku odwzorowaną w pamięci i zwraca jego deskryptor.
 * Jeśli plik istnieje, to zwracany jest jego deskryptor.
 * @param  [in]  *nazwa     - wskaźnik do tablicy z nazwą pliku (do 8 znaków).
 * @return 0 - plik został utworzony, 1 - plik nie został utworzony (brak miejsca, 2 - istnieje już plik o takiej nazwie.
 */
uint8_t ramDyskUtworzPlik(const char *nazwa);

/**
 * Otwiera istniejącą strukturę pliku odwzorowaną w pamięci i zwraca jego deskryptor.
 * @param  [in]  *nazwa     - wskaźnik do tablicy z nazwą pliku (do 8 znaków).
 * @param  [out] *fd        - wskaźnik do deskryptora pliku, zostanie on odpowiednio zmodyfikowany.
 * @return 0 - plik został otwarty, 1 - brak pliku, 2 - błąd.
 */
uint8_t ramDyskOtworzPlik(const char *nazwa, struct ramPlikFd *fd);

/*!
 * Usuwa strukturę pliku odwzorowaną w pamięci.
 * Wcześniej muszą być zamknięte wszystkie deskryptory dla tego pliku.
 * @param  [in] *nazwa      - wskaźnik do tablicy z nazwą pliku (do 8 znaków).
 * @return 0 - plik usunięty, 1 - plik nie może być usunięty ponieważ jest otwarty przez inne deskryptory.
 */
uint8_t ramDyskUsunPlik(const char *nazwa);

/**
 * Zamyka plik o podanym deskryptorze, wskaźnik deskryptora ustawiany jest na NULL
 * @param [in,out] *fd      - wskaźnik do deskryptora z plikiem do zamknięcia.
 */
void ramDyskZamknijPlik(struct ramPlikFd *fd);


/**
 * Czyści zawartość pliku i zwalnia klastry
 * @param [in,out] *fd      - deskryptor pliku do wyczyszczenia
 * @return 0 - operacja zakończona sukcesem, 1 - błąd (prawdopodobnei w implementacji)
 */
uint8_t ramDyskCzyscPlik(struct ramPlikFd *fd);

/**
 * Zapisuje do pliku.
 * @param [in,out] *fd      - wskaźnik do deskryptora pliku, do którego zostanie dopisany znak
 * @param [in]     znak     - znak jaki zostanie dopisany
 * @return 0 - operacja zakończona sukcesem, 1 - błąd (brak pamięci)
 */
uint8_t ramDyskZapiszBajtDoPliku(struct ramPlikFd *fd, uint8_t znak);

/**
 * Czyta z pliku odwzorowanego w pamięci.
 * @param [in,out] *fd      - deskryptor pliku, z którego zostanie odczytany bajt
 * @param [out]    *bajt    - wskaźnik na bajt do którego zostnie zapisany rezultat odczytu znaku z pliku.
 * @return 0 - OK, 1 - czytamy poza plikiem
 */
uint8_t ramDyskCzytajBajtZPliku(struct ramPlikFd *fd, uint8_t *bajt);

/**
 * Zapisuje blok danych do pliku.
 * @param [in,out] *fd      - wskaźnik do deskryptora pliku, do którego zostanie dopisany blok danych
 * @param [in]     *znaki   - wskaśnik do tablicy, jaka zostanie zapisana do pliku
 * @param [in,out] *dlugosc - wskaźnik do długości tablicy danych, po zapisie jego wartość określa liczbę zapisanych danych.
 * @return 0 - operacja zakończona sukcesem, 1 - błąd (brak pamięci)
 */
uint8_t ramDyskZapiszBlokDoPliku(struct ramPlikFd *fd, uint8_t *znaki, uint16_t *dlugosc);

/**
 * Czyta z pliku odwzorowanego w pamięci i zapisuje do tablicy
 * @param [in,out] *fd      - deskryptor pliku, z którego zostanie odczytany bajt
 * @param [out]    *znaki   - wskaźnik do tablicy, w której zostaną zapisane odczytane z pliku znaki
 * @param [in,out] *dlugosc - wskaźnik do liczby danych, jaką chcemy odczytać. Po wykonanej operacji pod wskaźnikiem zapisana jest liczba odczytanych danych.
 * @return 0 - OK, 1 - Plik jest pusty, 2 - Źle ustawiony indeks odczytu/zapisu (poza obszarem pliku), 3 - Nie udało się znaleźć odpowiedniego klastra
 */
uint8_t ramDyskCzytajBlokZPliku(struct ramPlikFd *fd, uint8_t *znaki, uint16_t *dlugosc);

/**
 * Ustawia indeks odczytu/zapisu dla deskryptora
 * @param [in,out] *fd      - deskryptor pliku, w którym zostanie przestawiony wskaźnik odczytu/zapisu
 * @param [in]     indeks   - pozycja wskaźnika odczytu/zapisu
 * @return 0 - OK, 1 - konieczne było zwiększenie rozmiaru pliku, lecz zabrakło pamięci
 */
uint8_t ramDyskUstawWskaznik(struct ramPlikFd *fd, uint16_t indeks);

/**
 * Ustawia indeks odczytu/zapisu na koniec pliku
 * @param [in,out] *fd      - deskryptor pliku, w którym zostanie przestawiony wskaźnik odczytu/zapisu
 * @return 0 - OK, 1 - zły deskryptor pliku
 */
uint8_t ramDyskUstawWskaznikNaKoniec(struct ramPlikFd *fd);

/**
 * Zapewnia, że w pliku jest miejce do zapisu 128 bajtowego bloku danych o numerze nrBloku
 * odebranego przez xModem. Indeks oraz wskaźnik pliku ustawiony jest na następny bajt za przydzielonym blokiem.
 * Również uaktualniony jest rozmiar pliku.
 * Funkcja zwraca wskaźnik na początek przydzielonego bloku.
 * Funkcja obsługująca transmisję xModemową może bezpośrednio pisać do pamięci, bez wykoszystania funkcji obsługujących pliki.
 * Rozwiązanie to jest mało eleganckie lecz wydajne.
 * @param [in,out] *fd      - deskryptor pliku, w którym zostanie przestawiony wskaźnik odczytu/zapisu
 * @param [in]     nrBloku  - jaki ma być przesłany. Uwaga pierwszy blok ma nr 1, a nie 0).
 * @return wskaźnik do pamięci, pokazujący na pocżątek bloku.
 */
uint8_t* ramDyskDodajBlokXmodem(struct ramPlikFd *fd, uint16_t nrBloku);


/**
 * Wyświetla pliki z głównego katalogu
 * @param [in] *putchar_func - wskaźnik do funkcji wypisującej znaki.
 */
void ramDyskDir(FILE *ostream);

/**
 * Zwraca liczbę wolnych klastrów (od 0 do 255). Klaster 0 jest zawsze traktowany jako zajęty.
 * @return liczba wolnych klastrów.
 */
uint8_t ramDyskLiczbaWolnychKlastrow(void);

/**
 * Otwiera strumień dla biblioteki StdIO
 * @param nazwa  - nazwa pliku do otwarcia
 * @param fd     - wskaźnik do deskryptora pliku. Należy wcześniej umieścić obiekt w pamięci lub na stosie
 * @param stream - wskaźnik do struktury FILE biblioteki StdIo
 * @param flags  - flagi
 */
uint8_t ramDyskOtworzPlikStdIo(const char *nazwa, struct ramPlikFd *fd, FILE *stream, uint8_t flags);

/**
 * Zamyka strumień wejścia/wyjścia obsługujący działający na Ram Pliku. Ważne by nie wywoływać tej funkcji z innym strumieniem.
 * @param stream - wskaźnik do struktury FILE biblioteki StdIo
 * @return liczba pozostałych deskryptorów związanych z zamykanym plikiem
 */
uint8_t ramDyskZamknijPlikStdIo(FILE *stream);


#endif /* RAMDYSK_H */
