#include "main.h"
#include "serial.h"
#include "hardware.h"

/**
 * Tablica buforów, za pomocą których komunikują się korutyny obsługujące diody
 */
xQueueHandle xDiodesOn[4];

/**
 * Deklaracje funkcji wykonywanych przez korutyny.
 */
static void vKlawisze(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex);
static void vDioda(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex);

void vApplicationIdleHook( void );


portSHORT main( void )
{
  /// Utworzenie kolejek
  xDiodesOn[0] = xQueueCreate( 4, 1);
  xDiodesOn[1] = xQueueCreate( 4, 1);
  xDiodesOn[2] = xQueueCreate( 4, 1);
  xDiodesOn[3] = xQueueCreate( 4, 1);

  ///Konfiguracja portów
  hardwareInit();
  ///Inicjacja portu szeregowego. Utworzenie kolejek do komunikacji z portem szeregowym
  xSerialPortInitMinimal(8);

  /// Utworzenie korutyn
  xCoRoutineCreate(vKlawisze, 0, 0);
  xCoRoutineCreate(vDioda, 0, 0);
  xCoRoutineCreate(vDioda, 0, 1);
  xCoRoutineCreate(vDioda, 0, 2);
  xCoRoutineCreate(vDioda, 0, 3);
  xCoRoutineCreate(vProtocol, 0, 0);

  /// Uruchomienie planisty. Rozpoczyna się praca systemu FreeRtos
  vTaskStartScheduler();
  return 0;
}

static void vKlawisze(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  /**
   * Jest tylko jedna korutyna do obsługi klawiszy.
   * Zatem nie wykorzystyjemy zmiennej uxIndex.
   * By pozbyć się ostrzeżenia po kompilacji należy rzutować tą zmienną na void.
   */
  (void) uxIndex;

  static uint8_t klawiszNr = 0;
  static int16_t result;

  crSTART( xHandle );
  for( ;; )
  {
    if (readKey(klawiszNr) == 0)
    {
                                                    /// 0 oznacza, że klawisz został wciśnięty
    }
    klawiszNr++;                                     /// Nie ma potrzeby w pętli for robić kolejnej pętli
    klawiszNr &= 0x03;                               /// Operacja %4 zrealizowana za pomoca iloczynu bitowego (klawiszNr = klawiszNr % 4)

    crDELAY( xHandle, 0);                            /// Wymuszenie przełączenia korutyny.
  }                                                  /// Makro crQUEUE_SEND z parametrem ticksToWait równym 0 nie przełącza korutyny
  crEND();
}

static void vDioda(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  crSTART( xHandle );
  for (;;)
  {

    crDELAY(xHandle, 0);                             /// Wymuszenie przełączenia korutyny, makro do odbioru wiadomości z czasem 0 nie przełącza korutyn
  }
  crEND();
}

void vProtocol(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  (void) uxIndex;

  crSTART( xHandle );
  for( ;; )
  {
    crDELAY(xHandle, 100);
  }
  crEND();
}

void vApplicationIdleHook( void )
{
  for( ;; )
  {
    vCoRoutineSchedule();
  }
}
