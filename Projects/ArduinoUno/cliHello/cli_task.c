#include "cli_task.h"

void vTaskVTY(void *cliStatePtr)
{
    CliState_t *state = (CliState_t *)(cliStatePtr);
    CMD_msg("Restart\r\n");  

    cmdlineInputFunc('\r', state);

    char recSymbol;
    for( ;; )
    {
        if( xQueueReceive(xVtyRec, &recSymbol, portMAX_DELAY))
        {
            cmdlineInputFunc((char)recSymbol, state);
            cliMainLoop(state);
        }
    }
}

