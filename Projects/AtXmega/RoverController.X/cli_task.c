#include "cli_task.h"

#include "adxl345.h"
#include "bmp085.h"
#include "mhc5883l.h"
#include "l3g4200d.h"
#include "hardware.h"

extern HardwarePAL_t hardwarePAL;

void vTaskVTYusb(void *cliStatePtr)
{
  CliState_t *state = (CliState_t *)(cliStatePtr);

  CMD_msg("Restart\r\n");  

  adxl345_init(&hardwarePAL.adxl, &hardwarePAL.twiSensors, ADXL345_RANGE_16G, state->myStdInOut);
  bmp085_init(&hardwarePAL.bmp, &hardwarePAL.twiSensors,  state->myStdInOut, BMP085_ULTRALOWPOWER);
  mhc5883l_init(&hardwarePAL.mhc, &hardwarePAL.twiSensors,  state->myStdInOut);
  l3g_init(&hardwarePAL.l3g, &hardwarePAL.twiSensors, L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_100HZ_12_5, state->myStdInOut);
  
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

