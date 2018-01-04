#include "cli_task.h"

void vTaskVtyRs1(void *cliStatePtr)
{
  cmdState_t *state = (cmdState_t *)(cliStatePtr);
  fprintf_P(state->myStdInOut, PSTR("Restart\r\n"));
  cmdlineInputFunc('\r', state);

  char znak;
  for( ;; )
  {
    if( xQueueReceive(xRs1Rec, &znak, portMAX_DELAY))
    {
      cmdlineInputFunc((char)znak, state);
      cmdlineMainLoop(state);
    }
  }
}

void vTaskVTYsocket(void *cliStatePtr)
{
  cmdState_t *state = (cmdState_t *)(cliStatePtr);

  char znak;
  for( ;; )
  {
    znak = 0;
    znak = fgetc(state->myStdInOut);
    cmdlineInputFunc((char)znak, state);
    cmdlineMainLoop(state);
  }
}
