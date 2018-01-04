#include "cli_task.h"

void vTaskVTY(void *cliStatePtr)
{
  cmdState_t *state = (cmdState_t *)(cliStatePtr);
  fprintf_P(state->myStdInOut, PSTR("Restart\r\n"));
  cmdlineInputFunc('\r', state);

  char znak;
  for( ;; )
  {
    if( xQueueReceive(xVtyRec, &znak, portMAX_DELAY))
    {
      cmdlineInputFunc((char)znak, state);
      cmdlineMainLoop(state);
    }
  }
}

