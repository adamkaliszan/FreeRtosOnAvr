#include "main_task.h"

void vTaskMain(void *Ptr)
{
  (void) Ptr;

  for( ;; )
  {
    vTaskDelay(10);
  }
}

