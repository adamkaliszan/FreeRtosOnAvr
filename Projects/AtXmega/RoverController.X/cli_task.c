#include "cli_task.h"

#include "adxl345.h"
#include "bmp085.h"
#include "hardware.h"

extern HardwarePAL_t hardwarePAL;

void vTaskVTYusb(void *cliStatePtr)
{
  CliState_t *state = (CliState_t *)(cliStatePtr);

  CMD_msg("Restart\r\n");
  adxl345_init(&hardwarePAL.adxl, &hardwarePAL.twiSensors, ADXL345_RANGE_16G, state->myStdInOut);
  
  bmp085_init(&hardwarePAL.bmp, &hardwarePAL.twiSensors,  state->myStdInOut, BMP085_ULTRAHIGHRES);

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

