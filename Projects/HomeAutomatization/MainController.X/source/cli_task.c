#include "cli_task.h"

void vTaskVTYusb(void *cliStatePtr)
{
  CliState_t *state = (CliState_t *)(cliStatePtr);
  fprintf_P(state->myStdInOut, PSTR("Restart\r\n"));
  cmdlineInputFunc('\r', state);
  
  char znak;
  for( ;; )
  {
    if( xQueueReceive(xVtyRec, &znak, portMAX_DELAY))
    {
      cmdlineInputFunc((char)znak, state);
      cliMainLoop(state);
    }  
  }
}

void vTaskVTYsocket(void *cliStatePtr)
{
  CliState_t *state = (CliState_t *)(cliStatePtr);
  
  char znak;
  for( ;; )
  {
    znak = 0;
    znak = fgetc(state->myStdInOut);
    cmdlineInputFunc((char)znak, state);
    cliMainLoop(state);
  }
}
