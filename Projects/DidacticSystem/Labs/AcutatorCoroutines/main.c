#include "main.h"

/**
 * Korutyna odpowiedzialna za sprawdzanie stanu przycisków.
 * @param xHandle uchwyt do korutyny umożliwiający zastosowanie makr. Programista nie może go używać bezpośrednio.
 * @param uxIndex indeks korutyny. Umożliwia wykonywanie tej samej funkcji przez kilka korutyn.
 *                W przypadku tej funkcji, parametr nie jest wykorzystywany.
 *                By uniknąć ostrzeżeń, należy na początku funkcji wpisać (void) uxIndex;
 */
static void vKlawisze(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex);

/**
 * Korutyna odpowiedzialna za sterowanie diodami.
 * @param xHandle uchwyt do korutyny umożliwiający zastosowanie makr. Programista nie może go używać bezpośrednio.
 * @param uxIndex indeks korutyny. Umożliwia wykonywanie tej samej funkcji przez kilka korutyn. W zależności od indeksu korytyna może sterować inną diodą
 */
static void vDioda(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex);

/**
 * Funkcja wykonywana w procesie bezczynności. W poniższym programie obsługuje ona korutyny. System wykonuje tylko 1 proces - proces bezczynności, nie ma innych zadań.
 */
void vApplicationIdleHook( void );


/*-----------------------------------------------------------*/

portSHORT main( void )
{
  /// Konfiguracja portów, określanie adresu urządzenia, konfiguracja portu szeregowego.
  hardwareInit();

  /// Dodawanie korutyn. Można tutaj uruchomić kolejne korutyny
  xCoRoutineCreate(vKlawisze, 0, 0);
  xCoRoutineCreate(vDioda, 0, 0);
  xCoRoutineCreate(vDioda, 0, 1);

  /// Uruchomienie planisty. Od tego momentu działa wielozadaniowy system operacyjny.
  vTaskStartScheduler();
  return 0;
}
/*-----------------------------------------------------------*/

static void vKlawisze(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  (void) uxIndex;           // get rid compiller worning. Theare is only one such coroutine. We don't need info about its index.

  crSTART( xHandle );
  for( ;; )
  {
    crDELAY( xHandle, 1);
  }
  crEND();
}

static void vDioda(xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex)
{
  crSTART( xHandle );
  for (;;)
  {
    ledOn(uxIndex);
    crDELAY(xHandle, 100);
  }
  crEND();
}

// Tej funkcji nie należy modyfikować.
void vApplicationIdleHook( void )
{
  for( ;; )
  {
    vCoRoutineSchedule();
  }
}
