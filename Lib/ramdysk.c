#include "ramdysk.h"
#include <string.h>

#define systemTime() 0; //Dodać w pliku hardware.h funkcje do odczytu czasu systemowego

static uint8_t         znajdzWolnyKlaster(void);
static uint8_t         nastepnyKlaster(uint8_t nrKlastra);
static uint8_t         znajdzKlasterN(uint8_t nrPierwszegoKlastra, uint8_t lPrzeskokow);
#if ( USUNKLASTER == 1)
static uint8_t         usunKlaster(uint8_t nrKlastra);
#endif
static uint8_t         wObrebiePliku(struct ramPlikFd *fd);
static void            uaktualnijRozmiarPliku(struct ramPlikFd *fd);
static struct ramPlik* znajdzPlik(const char *nazwa);
static struct ramPlik* znajdzMiejsceNaWpis(void);
static void            czyscKlaster(uint8_t nrKlastra);

static void            czyscKlaster(uint8_t nrKlastra)
{
  uint8_t *tmpPtr = dataPtr(nrKlastra, 0);
  memset (tmpPtr, 0, 256);
}

static uint8_t  wObrebiePliku(struct ramPlikFd *fd)
{
  if (fd->wpis->rozmiarHi > fd->IndHi)
    return 0;

  if ((fd->wpis->rozmiarHi == fd->IndHi) && (fd->wpis->rozmiarLo >= fd->IndLo))
    return 0;

  return 1;
}

static void uaktualnijRozmiarPliku(struct ramPlikFd *fd)
{
  if (fd->wpis->rozmiarHi == fd->IndHi)
  {
    if (fd->wpis->rozmiarLo < fd->IndLo)
      fd->wpis->rozmiarLo = fd->IndLo;
  }
  if (fd->wpis->rozmiarHi < fd->IndHi)
  {
      fd->wpis->rozmiarLo = fd->IndLo;
      fd->wpis->rozmiarHi = fd->IndHi;
  }
}

static uint8_t znajdzWolnyKlaster(void)
{
  uint8_t i;
  for (i=1; i<L_KLASTROW; i++)             //Pomijamy klaster 0, tam jest wpis katalogowy. Jeśli ma wartość 0, to oznacza to, że na nim się kończy wpis katalogowy.
  {                                        //Może być rozszerzony na inne klastry.
    if (klastry[i] == 0)                   //Sprawdzanie, czy klaster jest pusty.
    {
      klastry[i] = i;                      //Oznaczanie klastra jako ostatniego.
      czyscKlaster(i);                     //Czyszczenie zawartości klastra.
      return i;
    }
  }
  return 0;
}

static uint8_t nastepnyKlaster(uint8_t nrKlastra)
{
  uint8_t temp = klastry[nrKlastra];       //Temp oznacza na następny klaster.
  if (temp == nrKlastra)                   //Jeśli klaster wskazuje na samego siebie, co oznacza, że jest ostatni w pliku
  {                                        //Należy znaleźć jakiś wolny klaster i go dodać do łańcycha klastrów w pliku
    for (temp=1; temp <128; temp++)        //Przekglądamy wszystkie klastry za wyjątkiem klastra 0 (dla temp = 0 nie ma znaczenia instrukcja break)
    {                                      //Jeśli temp ma wartość 0 to oznacza, że nie ma wolnego klastra
      if (klastry[temp] == 0)              //Jeśli w tablicy klaster wskazuje na 0, to oznacza, że jest on pusty
        break;                             //Wtedy można przerwać szukanie kolejnych klastrów dla klastra
    }                                      //Taka implementacja z założenia powoduje defragmentację.

    if (temp != 0)                         //Znaleziono jakiś wolny klaster
    {
      klastry[nrKlastra] = temp;           //Ustawienie łańcucha z klastrami, dodanie do pliku kolejnego klastra
      klastry[temp]=temp;                  //Oznaczenie klastra jako zajętego (ostatniego w danym pliku)
      czyscKlaster(temp);                  //Czyszczenie klastra
    }
  }
  return temp;
}

static uint8_t znajdzKlasterN(uint8_t nrPierwszegoKlastra, uint8_t lPrzeskokow)
{
  uint8_t ind;                             //PIerwszy klaster ma indeks 0
  uint8_t wynik = nrPierwszegoKlastra;
  for (ind=0; ind < lPrzeskokow; ind++)    //Należy wykonać klasterN przeskoków na kolejne klastry (IndHi określa ile trzeba zrobić przeskoków od pierwszego klastra)
  {
    wynik = nastepnyKlaster(wynik);        //Jeśli nie ma kolejnego klastra, to zostanie automatycznie dodany
    if (wynik == 0)                        //Sprawdzany, czy znalazł się kolejny klaster
      break;                               //Błąd, zwrócony zostanie klaster 0 - zarezerwowany na początek tablicy plików
  }
  return wynik;
}

#if ( USUNKLASTER == 1)
static uint8_t usunKlaster(uint8_t nrKlastra)
{
  if (klastry[nrKlastra] == 0)
    return 1;                              //Klaster nie był zajęty
  uint8_t temp;
  for (temp = 0; temp <128; temp++)
    if (klastry[temp] == nrKlastra)        //Znaleziono klaster, który wskazuje na ten usuwany
    {
      if (klastry[nrKlastra] == nrKlastra) //Sprawdzanie, czy usuwany klaster nie jest ostatni.
	klastry[temp] = temp;              //Jeśli tak, to klaster poprzedni musi być ustawiony jako klaster ostatni
      else
	klastry[temp] = klastry[nrKlastra];//Klaster poprzedni wskazuje na klaster następny za tym usuwanym
    }
  klastry[nrKlastra] = 0;                  //Oznaczanie klastra jako pusty
  return 0;
}
#endif

static struct ramPlik* znajdzPlik(const char *nazwa)
{
  uint8_t dlNazwy = strlen(nazwa);        //Skracanie podanej nazwy do 8 znaków
  if (dlNazwy > 8)
    dlNazwy = 8;


  if (nazwa[dlNazwy-1] == 0)
    dlNazwy--;                            //Nie sprawdzamy czy string kończy się /0
  struct ramPlik *plik;
  uint8_t temp;
  uint8_t tempKlaster=0;
  uint8_t tempKlaster2;

  for (temp=1; temp < dlNazwy; temp ++)
  {
    if (nazwa[temp] == ' ')               //Pozbycie sie spacji
    {
      dlNazwy = temp;
      break;
    }
  }
  if (dlNazwy == 0)
    return NULL;

  do
  {
    plik = (struct ramPlik *)(dataPtr(tempKlaster, 0));  //Odczyt pierwszego nagłówka pliku w klastrze
    for (temp=0; temp <16; temp++)
    {
      if (strncmp(nazwa, plik->nazwa, dlNazwy) == 0)
	return plik;
      plik++;                                            //Przejście do kolejnego wpisu (w tym samym klastrze)
    }
    tempKlaster2 = tempKlaster;
    tempKlaster = klastry[tempKlaster];
  }
  while (tempKlaster2 != tempKlaster);
  return NULL;
}

static struct ramPlik* znajdzMiejsceNaWpis(void)
{
  struct ramPlik *plik;
  uint8_t temp;
  uint8_t tempKlaster=0;                                 //Przeszukiwanie głównego wpisu katalogowego, jest on zapisany w klastrze 0
  uint8_t tempKlaster2;                                  //Przeszukiwanie głównego wpisu katalogowego, jest on zapisany w klastrze 0
  do
  {
    plik = (struct ramPlik *)(dataPtr(tempKlaster, 0));  //Odczyt pierwszego nagłówka pliku w klastrze
    for (temp=0; temp <16; temp++)
    {
      if (plik->nazwa[0] == 0)
	return plik;
      plik++;                                            //Przejście do kolejnego wpisu (w tym samym klastrze)
    }
    tempKlaster2 = tempKlaster;
    tempKlaster = klastry[tempKlaster];
  }
  while (tempKlaster2 != tempKlaster);

  klastry[tempKlaster] = znajdzWolnyKlaster();
  plik = NULL;
  if (klastry[tempKlaster] != 0)
    plik = (struct ramPlik*) (dataPtr(klastry[tempKlaster], 0));

  return plik;
}


void ramDyskInit(void)
{
  memset (klastry, 0, 128);              //Czyszczenie tablicy klastrów (wszystkie są puste)
  memset (dataPtr(0,0), 0, 256);         //Czyszczenie zerowego klastra z tablicą plików
}
uint8_t ramDyskUtworzPlik(const char *nazwa)
{                                        //Nowo utworzony plik nie zajmuje żadnego klastra
  uint8_t dlNazwy = strlen(nazwa);
  uint8_t i;
  if (dlNazwy > 8)
    dlNazwy = 8;

  for (i=0; i<dlNazwy; i++)
  {  if (nazwa[i] == ' ')
     {
       dlNazwy = i;
       break;
     }
  }
  if (dlNazwy == 0)
    return 0;

  struct ramPlik *plik;
  if ((plik = znajdzPlik(nazwa)) != NULL)
  {
    return 0;
  }

  if ((plik = znajdzMiejsceNaWpis()) != NULL) //Szukanie pustego wpisu (nagłówka) po skasowanym pliku
  {
    memset(plik, 0, 12);                      //Czyszczenie wpisu (pola: pierwszyKlaster, rozmiarLo, rozmiarHi, lAktOtw ustawione na wartość 0)
    strncpy(plik->nazwa, nazwa, dlNazwy);     //Ustawianie odpowiedniej nazwy pliku
    plik -> dataMod = systemTime();           //Ustawianie czasu modyfikacji
    return 0;
  }
  return 1;
}

uint8_t ramDyskOtworzPlik(const char *nazwa, struct ramPlikFd *fd)
{
  uint8_t wynik = 1;
  struct ramPlik *plik;
  if ((plik = znajdzPlik(nazwa)) != NULL)
  {
    memset(fd, 0, 3);                                      //Zerowanie struktury deskryptora pliku
    fd->wpis = plik;                                       //Ustawianie w deskryptorze wskaźnika na wpis pliku do katalogu głównego
    plik->lAktOtw++;                                       //Uaktualnienie licznika otwarć plików
    wynik = 0;
  }
  return wynik;
}

uint8_t ramDyskUsunPlik(const char *nazwa)
{
  struct ramPlik *plik;
  if ((plik = znajdzPlik(nazwa)) == NULL)
    return 1;                                              //Nie znaleziono pliku
  if (plik->lAktOtw != 0)
    return 2;                                              //Plik jest otwarty

  uint8_t usuwanyKlaster;
  while(plik->pierwszyKlaster != 0)                        //Już na samym początku może się okazać, że plik nie miał klastrów
  {
    usuwanyKlaster = plik->pierwszyKlaster;
    if (klastry[usuwanyKlaster] == usuwanyKlaster)         //Sprawdzanie, czy nie usuwamy ostatniego klastra
      plik->pierwszyKlaster = 0;                           //Ok można już zakończyć usuwanie pliku
    else
      plik->pierwszyKlaster = klastry[usuwanyKlaster];     //Przejście do następnego klastra
    klastry[usuwanyKlaster] = 0;                           //Usunięcie klastra
  }

  memset (plik, 0, 16);                                    //Wpis katalogowy zostaje. Jest pusty. Inny plik może być tam wpisany
//relokacjaTablicyWpisow()
  return 0;
}

void ramDyskZamknijPlik(struct ramPlikFd *fd)
{
  if (fd -> wpis -> lAktOtw > 0)                           //Sprawdzanie, czy licznik otwarć jest poprawny
  {
    fd->wpis->lAktOtw--;                                   //Zmniejszanie licznika otwarć pliku
    memset(fd, 0, sizeof(struct ramPlikFd));               //CZyszczenie struktury deskryptora plików
  }
}

uint8_t ramDyskCzyscPlik(struct ramPlikFd *fd)
{
  uint8_t usuwanyKlaster;
  while(fd->wpis->pierwszyKlaster != 0)                        //Już na samym początku może się okazać, że plik nie miał klastrów
  {
    usuwanyKlaster = fd->wpis->pierwszyKlaster;
    if (klastry[usuwanyKlaster] == usuwanyKlaster)             //Sprawdzanie, czy nie usuwamy ostatniego klastra
      fd->wpis->pierwszyKlaster = 0;                           //Ok można już zakończyć usuwanie pliku
    else
      fd->wpis->pierwszyKlaster = klastry[usuwanyKlaster];     //Przejście do następnego klastra
    klastry[usuwanyKlaster] = 0;                               //Usunięcie klastra
  }
  fd->wpis->rozmiarLo       = 0;
  fd->wpis->rozmiarHi       = 0;
  fd->wpis->dataMod         = systemTime();
  memset (fd, 0, 4);
  return 0;
}

uint8_t ramDyskZapiszBajtDoPliku(struct ramPlikFd *fd, uint8_t znak)
{
  uint8_t tmpKlaster;
  if (fd->wpis->pierwszyKlaster == 0)
  {
    if ((tmpKlaster = znajdzWolnyKlaster()) == 0)
      return 1;                                         //Nie można było przydzielić pierwszego klastra do pliku
    fd->wpis->pierwszyKlaster = tmpKlaster;
  }
  if (fd->IndLo == 0)
  {
    tmpKlaster = znajdzKlasterN(fd->wpis->pierwszyKlaster, fd->IndHi); //Wyszukanie lub dodanie następnego klastra. Na podstawie rozmiaru zostanie stwierdzone, czy klaster był dodany
    if (tmpKlaster == 0)
    {
      return 2;                                       //Nie można było przydzielić kolejnego klastra
    }
    fd->Wsk = dataPtr(tmpKlaster, 0);                 //Ustawianie wskaźnika na początek nowego klastra
  }

  *(fd->Wsk) = znak;                                  //Zapis bajtu do pliku

  fd->IndLo++;                                        //Zwiększanie indeksu odczytu/zapisu
  if (fd->IndLo == 0)                                 //Jeśli ma on wartość 0, to oznacza to, że czły klaster jest zapisany
    fd->IndHi++;                                      //Należy zwiększyć bardziej znaczący bajt indeksu. Nie utworzono nowego klastra, zatem nie uaktualniamy wskaźnika
  else                                                //Wziąż działamy na tym samym klastrze.
    fd->Wsk++;                                        //Można uaktualnić wskaźnik

  uaktualnijRozmiarPliku(fd);                         //Uaktualnianie rozmiaru
  return 0;
}

uint8_t ramDyskCzytajBajtZPliku(struct ramPlikFd *fd, uint8_t *bajt)
{
  if (wObrebiePliku(fd) != 0)                         //Sprawdzanie, czy jesteśmy w obrębie pliku
    return 1;                                         //1 - eof

  if (fd->IndLo == 0)                                 //Sprawdzanie, czy działamy na początku nowego klastra
  {
    uint8_t nrKlastra = znajdzKlasterN(fd->wpis->pierwszyKlaster, fd->IndHi);
    fd->Wsk = dataPtr(nrKlastra, 0);                  //Ustawianie wskaźnika na początek nowego klastra
  }

  *bajt = *(fd->Wsk);                                 //Odczyt z pliku
  fd->IndLo++;                                        //Zwiększenie indeksu o 1
  if (fd->IndLo == 0)                                 //Sprawdzamy, czy przeszliśmy do nowego klastra
    fd->IndHi++;                                      //Tak: uaktualniamy liczbę przeskokół względem pierwszego klastra (bardziej znaczący bajt indeksu)
  else                                                //Działamy na tym samym klastrze
    fd->Wsk++;                                        //Uaktualniamy wskaźnik do tego klastra
  return 0;
}

uint8_t ramDyskZapiszBlokDoPliku(struct ramPlikFd *fd, uint8_t *znaki, uint16_t *dlugosc)
{
  if (fd->wpis->pierwszyKlaster == 0)
    fd->wpis->pierwszyKlaster = znajdzWolnyKlaster();

  uint16_t dlBloku = 256 - fd->IndLo;             //Obliczanie liczby bajtów, jaką da się zapisać w aktualnym klastrze
  uint16_t doZapisu = *dlugosc;
  *dlugosc = 0;                                   //Jak do tąd jeszcze nic nie zapisano
  uint8_t tmpKlaster = 0;
  while (doZapisu > 0)
  {
    if (fd->IndLo == 0)                           //Jeśli indeks pokazuje na początek klastra, to należy odczytać jego numer oraz ustawić na niego wskaźnik
    {
      if (tmpKlaster == 0)                        //Pierwsza operacja zapisu, nie odczytano jeszcze numeru klastra
        tmpKlaster = znajdzKlasterN(fd->wpis->pierwszyKlaster, fd->IndHi); //Odczyt numeru klastra na podstawie informacji o liczbie przeskoków od pierwszego
      else                                        //Znamy poprzedni klaster
        tmpKlaster = nastepnyKlaster(tmpKlaster); //Wystarczy przejść do następnego
      if (tmpKlaster == 0)                        //Sprawdzanie, czy udało się znaleźć klaster
        return 1;                                 //1 - Brak wolnego klastra

      fd->Wsk = dataPtr(tmpKlaster, 0);           //Ustawianie wskaźnika na początek klastra. Teraz można do niego już pisać
    }
    if (doZapisu > dlBloku)                       //Sprawdzanie, czy wszystko uda się zapisać w bieżącym klastrze
    {                                             //Nie uda się, teraz zapiszemy cały klastr do końca
      memcpy(fd->Wsk, znaki, dlBloku);            //Zapis do końca aktualnego klastra
      znaki +=dlBloku;
      fd->IndLo = 0;                              //Mniej znaczący bajt odczytu wskazuje na początek nowego klastra. Kolejna iteracja go utworzy
      doZapisu -= dlBloku;                        //Uaktualnianie informacji o liczbie bajtów jaka pozostałą do zapisujemy
      *dlugosc += dlBloku;                        //Uaktualnianie informacji o liczbie zapisanych danych
      fd->IndHi++;                                //Ustawienie bardziej znaczącego bajtu indeksu. Oznacza to przejście do kolejnego klastra
      dlBloku = 256;                              //Do następnego klastra możemy zapisać do 256 bajtów
    }
    else                                          //Jest to ostatni zapis. Całe dane zostaną skopiowane
    {
      memcpy(fd->Wsk, znaki, doZapisu);           //Ostatnia operacja zapisu do klasrta.
      fd->IndLo += doZapisu;                      //Uaktualnianie indeksu (wystarczy uaktualnić mneij znaczący bajt).
      *dlugosc  += doZapisu;                      //Uaktualnianie informacji o liczbie zapisanych danych.
      doZapisu  =  0;                             //Równie dobrze można tutaj wstawić break;
      fd->Wsk = dataPtr(tmpKlaster, fd->IndLo);   //Ustawianie wskaźnika w odpowiednie miejsce klastra.
    }
  }
  uaktualnijRozmiarPliku(fd);                     //Uaktualnianie rozmiaru pliku
  return 0;
}

uint8_t ramDyskCzytajBlokZPliku(struct ramPlikFd *fd, uint8_t *znaki, uint16_t *dlugosc)
{
  if (fd->wpis->pierwszyKlaster == 0)
  {
    *dlugosc = 0;
    return 1;                         //1 - Plik jest pusty
  }
  if (wObrebiePliku(fd) != 0)
  {
    *dlugosc = 0;
    return 2;                         //2 - Źle ustawiony indeks odczytu/zapisu (poza obszarem pliku)
  }

  uint16_t lDanych = (fd->wpis->rozmiarHi - fd->IndHi);
  lDanych +=fd->wpis->rozmiarLo;      //Obliczanie liczby bajtów jaka zostałą zapisana jeszcze za wskaźnikiem.
  lDanych -=fd->IndLo;                //Na podstawie wcześniej sprawdzonych warunków jest to zawsze liczba dodatnia

  uint16_t doOdczytania = (lDanych < *dlugosc)?  //Sprawdzenie liczby zapisanych bajtół w pliku i określenie ile bajtów zdoła się odczytać
    lDanych :                          //W pliku jest mniej bajtów do odczytu niż chcemy odczytać
    *dlugosc;                          //W pliku jest więcej bajtów niż chcemy odczytać
  *dlugosc = 0;                        //Jak do tąd odczytano 0 bajtów
  uint16_t dlBloku = 256 - fd->IndLo;  //Określanie liczby bajtół jaka zostałą do końca aktualnego klastra`
  uint8_t tmpKlaster = 0;
  while (doOdczytania > 0)
  {
    if (fd->IndLo == 0)                //Indeks odczytu wskazuje na początek klastra. Oznacza to, że wskaźnik nie jest jeszcze ustawiony
    {                                  //Bardziej znaczący bajt indeksu określa o ile klastrów (od początkowego) należy się przesunąć do przodu
      if (tmpKlaster == 0)
        tmpKlaster = znajdzKlasterN(fd->wpis->pierwszyKlaster, fd->IndHi);
      else
        tmpKlaster = nastepnyKlaster(tmpKlaster);
      if (tmpKlaster != 0)             //Jeśli znaleziono odpowiedni klaster, to
        fd->Wsk = dataPtr(tmpKlaster, 0); //przestaw wskaźnik na początek tego klastra
      else
        return 3;                      //3 - Nie udało się znaleźć odpowiedniego klastra
    }

    if (doOdczytania > dlBloku)        //Odczyt do końca zawartości klastra
    {                                  //dlBloku określa ile zostało jeszcze bajtów do końca klasrta
      memcpy(znaki, fd->Wsk, dlBloku); //Odczyt zawartości Klastra
      znaki +=dlBloku;                 //Przestawienie wskaźnika do tablicy, w której zapisujemy odczytane bajty
      fd->IndLo = 0;                   //Indeks wskazuje na początek nowego klastra
      fd->IndHi++;                     //Bardziej znaczący bajt określa zmianę klastra. Teraz nie ma potrzeby ustawienia wskaźnika odczytu na jego początek
      doOdczytania -= dlBloku;         //Uaktualnienie liczby bajtół jaką należy odczytać
      *dlugosc += dlBloku;             //Uaktualnienie
      dlBloku = 256;                   //Kolejny dostępny blok do odczytania, to długość całego klastra.
    }
    else                               //Ostatnia operacja odczytu
    {
      memcpy(znaki, fd->Wsk, doOdczytania);
      fd->Wsk   += doOdczytania;       //Po zakończeniu operacji odczytu nadal działamy w tym samym klastrze, zatem trzeba teraz uaktualnić wzkaźnik
      fd->IndLo += doOdczytania;       //Uaktualnianie indeksu. Jesteśmy w tym samym klastrze, zatem nie trzeba zmieniać IndHi
      *dlugosc  += doOdczytania;       //Uaktualnianie liczby odczytanych bajtów
      doOdczytania = 0;                //Tutaj równie dobrze może być brake
    }
  }
  return 0;
}

uint8_t ramDyskUstawWskaznik(struct ramPlikFd *fd, uint16_t indeks)
{
  if (indeks == 0)                                        //Sprawdzanie, czy wskaźnik nie pokazuje na początek pliku.
  {                                                       //Jeśli tak, to nie ma potzeby tworzenia klastrów. Plik może nadal nei mieć żadnego klastra.
    fd->IndLo = 0;                                        //Ustawianie na 0 indeksów (mniej i bardziej znaczącego bajtu)
    fd->IndHi = 0;
    return 0;
  }
  indeks--;                                               //Zmniejszamy indeks o 1, by odpowiednio ustawić rozmair pliku.
                                                          //Jeśli indeks jest większy niż rozmiar pliku, to plik zostanie rozciągnięty do zadanej wartości indeksu -1

  if (fd->wpis->pierwszyKlaster == 0)                     //Sprawdzanie, czy plik ma już przydzielony pierwszy klaster
    fd->wpis->pierwszyKlaster = znajdzWolnyKlaster();     //Przydzielanie pierwszego klastra dla pliku. Jest to konieczne, ponieważ klaster ma wartość niezerową

  uint8_t klasterN = indeks >> 8;                         //Obliczanie liczby klastrów, jaką należy dodać do pierwszego klastra`

  fd->IndLo = indeks & 0xFF;                              //Obliczanie na podstawie wartości 16 mniej znaczącego bajtu indeksu pomniejszonej o 1
  fd->IndHi = klasterN;                                   //oraz bardziej znaczącego bajtu indeksu w deskryptorze.

  uint8_t nrKlastra = znajdzKlasterN(fd->wpis->pierwszyKlaster, klasterN);
  if (nrKlastra == 0)
    return 1;                                             //Brak klastrów

  uaktualnijRozmiarPliku(fd);                             //Uaktualnianie rozmiaru pliku

  fd->IndLo++;                                            //Powrót do zadanej wartości indeksu
  if (fd->IndLo == 0)                                     //Jeśli jesteśmy na początku klastra, to jeszcze on nie iestnieje
    fd->IndHi++;                                          //Uaktualniamy tylko bardziej znaczący bajt indeksu
  else                                                    //Jesteśmy w obszarze utworzonego klastra, można uaktualnić wskaźnik
    fd->Wsk=dataPtr(nrKlastra, fd->IndLo);                //ustawić wskaźnik na odpowiednie miejsce klastra

  return 0;
}

uint8_t ramDyskUstawWskaznikNaKoniec(struct ramPlikFd *fd)
{
  if (fd == NULL)
    return 1;
  fd->IndLo = fd->wpis->rozmiarLo;
  fd->IndHi = fd->wpis->rozmiarHi;
//  fd->IndLo++;
  uint8_t tmpKlaster = 0;
  if (fd->IndLo != 0)
  {
    tmpKlaster = znajdzKlasterN(fd->wpis->pierwszyKlaster, fd->wpis->rozmiarHi);
    fd->Wsk=dataPtr(tmpKlaster, fd->IndLo);
  }
  return 0;
}

uint8_t* ramDyskDodajBlokXmodem(struct ramPlikFd *fd, uint16_t nrBloku)
{
  if (nrBloku == 0)
    return NULL;
  nrBloku --;

  uint8_t indHi = (uint8_t)(nrBloku / 2);
  //uint8_t indLo = 0;
  uint8_t *wynik;

  if (fd->wpis->pierwszyKlaster == 0)
    fd->wpis->pierwszyKlaster = znajdzWolnyKlaster();
  if (fd->wpis->pierwszyKlaster == 0)
    return NULL;

  uint8_t tmpKlaster = znajdzKlasterN(fd->wpis->pierwszyKlaster , indHi);
  if (tmpKlaster == 0)
    return NULL;
  if ((nrBloku & 0x0001) == 0x0001)           // Druga część klastra
  {
    //indLo = 128;
    if (fd->wpis->rozmiarHi <= indHi)
    {
      fd->wpis->rozmiarHi = indHi+1;
      fd->wpis->rozmiarLo = 0;
    }
    wynik=dataPtr(tmpKlaster, 128);
  }
  else
  {
    if (fd->wpis->rozmiarHi < indHi)
    {
      fd->wpis->rozmiarHi = indHi;
      fd->wpis->rozmiarLo = 128;
    }
    else if ((fd->wpis->rozmiarHi == indHi) && (fd->wpis->rozmiarLo < 128))
      fd->wpis->rozmiarLo = 128;

    wynik=dataPtr(tmpKlaster, 0);
  }
  return wynik;
}

void ramDyskDir(FILE *ostream)
{
  fprintf(ostream, "nazwa\t\trozmiar\totwarty\r\n");
  struct ramPlik *plik;
  uint8_t tmpKlaster = 0;
  uint8_t tmpKlaster2;
  uint8_t tmp, tmp2, tmp3;
  do
  {
    plik = (struct ramPlik *)(dataPtr(tmpKlaster, 0));
    for (tmp=0; tmp<16; tmp++)
    {
      tmp3=plik->nazwa[0];
      if (tmp3 == 0)
        break;                                            //Ten wpis jest pusty.
      fputc(tmp3         , ostream);
      for (tmp2=1; tmp2<8; tmp2++)
      {
        if (tmp3 != 0)
          tmp3=plik->nazwa[tmp2];

        if (tmp3 != 0)
          fputc(tmp3     , ostream);
        else
          fputc(' '      , ostream);
      }
      fprintf(ostream, "\t%d\t%d\r\n", 256*plik->rozmiarHi+plik->rozmiarLo,  plik->lAktOtw);
      plik++;
    }
    tmpKlaster2 = tmpKlaster;
    tmpKlaster = klastry[tmpKlaster];
  }
  while (tmpKlaster != tmpKlaster2);
}

uint8_t ramDyskLiczbaWolnychKlastrow(void)
{
  uint8_t wynik=0;
  uint8_t temp;
  for (temp = L_KLASTROW-1; temp > 0; temp--)
    if (klastry[temp] == 0)
      wynik++;
  return wynik;
}


static int getSTD(FILE *stream)
{
  uint8_t wynik;
  struct ramPlikFd *fd = (struct ramPlikFd *)(fdev_get_udata(stream));

  if (ramDyskCzytajBajtZPliku(fd, &wynik) == 0)
    return wynik;
  return EOF;
}

static int putSTD(char c, FILE *stream)
{
  struct ramPlikFd *fd = (struct ramPlikFd *)(fdev_get_udata(stream));
  return ramDyskZapiszBajtDoPliku(fd, c);
}

uint8_t ramDyskOtworzPlikStdIo(const char *nazwa, struct ramPlikFd *fd, FILE *stream, uint8_t flags)
{
  uint8_t wynik = ramDyskOtworzPlik(nazwa, fd);
  if (wynik != 0)
    return wynik;

  fdev_setup_stream(stream, putSTD, getSTD, flags);
  fdev_set_udata(stream, fd);
  return 0;
}

uint8_t ramDyskZamknijPlikStdIo(FILE *stream)
{
  struct ramPlikFd *fd = (struct ramPlikFd *)(fdev_get_udata(stream));
  ramDyskZamknijPlik(fd);
  fclose(stream);
  return fd->wpis->lAktOtw;
}
