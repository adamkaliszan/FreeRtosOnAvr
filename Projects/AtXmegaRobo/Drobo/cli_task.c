#include "cli_task.h"

void vTaskVTYusb(void *cliStatePtr)
{
  cmdState_t *state = (cmdState_t *)(cliStatePtr);
  fprintf_P(state->myStdInOut, PSTR("Restart\r\n"));
  cmdlineInputFunc('\r', state);

  char recSymbol;
  for( ;; )
  {
    if( xQueueReceive(xVtyRec, &recSymbol, portMAX_DELAY))
    {
      cmdlineInputFunc((char)recSymbol, state);
      cmdlineMainLoop(state);
    }
  }
}

