#include "cli_task.h"

#include <stdio.h>
#include <avr/pgmspace.h>

#include "cmdline.h"

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

