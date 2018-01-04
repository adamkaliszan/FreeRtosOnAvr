/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Ethernet remote device and sensor
 * UDP and HTTP interface
        url looks like this http://baseurl/password/command
        or http://baseurl/password/
 *
 * Chip type           : Atmega88 or Atmega168 or Atmega328 with ENC28J60
 * Note: there is a version number in the text. Search for tuxgraphics
 *********************************************/
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "sensors_task.h"
#include "memory_x.h"
#include "main.h"
#include "Rs485_prot.h"
#include "protocol1.h"
#include "mpc23s17.h"



void sensorsTaskInit(void)
{
  LockersMemInit();
  rollersMemInit();
}

void sensorsTask(void* pvParameters)
{
  (void) pvParameters;
  uint8_t addr = 255;
//  uint8_t i;

  MPC23s17SetDirA(0x00, 0);

  MPC23s17SetDirB(0x00, 0);

  for( ; ; )
  {
    uint16_t tmp;
    //Read power suply voltage
    tmp = MCP3008_getSampleSingle(0);
    voltage = (uint8_t)(tmp>>5);
    vTaskDelay(10);

    //Read temperature inside chasis
    tmp = MCP3008_getSampleSingle(1);
    tmp *=10;
    temperature = (uint8_t)(tmp / 24);
    vTaskDelay(10);

    //read lock
    checkLockerSensors();

    for (addr = FIRST_ROLLER_DRIVER_ADDR; addr <= LAST_ROLLER_DRIVER_ADDR; addr++)
    {
      rs485rollerHello(addr);
      vTaskDelay(10);
    }

    for (addr = FIRST_LIGHT_DRIVER_ADDR; addr <= LAST_LIGHT_DRIVER_ADDR; addr++)
    {
      ;
      //vTaskDelay(10);
    }

    for (addr = FIRST_SENSOR_ADDR; addr <= LAST_SENSOR_ADDR; addr++)
    {
      ;
      //vTaskDelay(10);
    }
  }
}
