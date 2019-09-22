#include "cli_task.h"

void vTaskVTYusb(void *cliStatePtr)
{
  CmdState_t *state = (CmdState_t *)(cliStatePtr);
#ifdef USE_XC8
  fprintf_P(state->myStdInOut, PSTR("Restart\r\n"));
#else
  fprintf_P(state->myStdInOut, PSTR("Restart\r\n"));
#endif
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
