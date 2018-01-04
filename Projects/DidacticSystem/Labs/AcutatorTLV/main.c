#include "main.h"
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
  
  hardwareInit();
  
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

  static uint8_t czasSwiecenia = 200;                /// Dioda zostanie zapalona na 4 sekundy
  static uint8_t klawiszNr = 0;
  static int16_t result;
  
  crSTART( xHandle );
  for( ;; )
  {
    if (readKey(klawiszNr) == 0)
    {                                                /// 0 oznacza, że klawisz został wciśnięty
      crQUEUE_SEND(xHandle, xDiodesOn[klawiszNr], (void *) (&czasSwiecenia), 0, &result); ///Wysyłanie wiadomości do odpowiedniej kolejki
    }                                                /// by zapalić diodę. Operacja ta z parametrem ticksToWait = 0 nie przełącza korutyny

    klawiszNr++;                                     /// Nie ma potrzeby w pętli for robić kolejnej pętli
    klawiszNr &= 0x03;                               /// Operacja %4 zrealizowana za pomoca iloczynu bitowego (klawiszNr = klawiszNr % 4)
    
    crDELAY( xHandle, 0);                            /// Wymuszenie przełączenia korutyny.
  }                                                  /// Makro crQUEUE_SEND z parametrem ticksToWait równym 0 nie przełącza korutyny
  crEND();
}

static void vDioda(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  static uint8_t czasy[4];
  static int16_t czasyCzekania[4] = {0, 0, 0, 0};    // UWAGA: Te zmienne muszą być 16 bitowe
  static int16_t odebrano[4];                        // UWAGA: Te zmienne muszą być 16 bitowe

  crSTART( xHandle );
  for (;;)
  {
    if (czasyCzekania[uxIndex] > 0)
      ledOn(uxIndex);
    else
      ledOff(uxIndex);

    crQUEUE_RECEIVE(xHandle, xDiodesOn[uxIndex], (void *) (&czasy[uxIndex]), czasyCzekania[uxIndex], &odebrano[uxIndex]);
    if (odebrano[uxIndex] == pdPASS)
      czasyCzekania[uxIndex] = czasy[uxIndex];
    else
      czasyCzekania[uxIndex] = 0;

    crDELAY(xHandle, 0);                             // Wymuszenie przełączenia korutyny
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
