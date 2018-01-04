#include "loop_task.h"

void vTaskLoop(void *arg)
{
  (void) arg;

  for( ;; )
  {
    vTaskDelay(100);
    PORTB = 0;

    vTaskDelay(100);
    PORTB |= 0x20;
  }
}
