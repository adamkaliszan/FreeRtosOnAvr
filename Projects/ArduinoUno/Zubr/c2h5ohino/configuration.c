#include "configuration.h"
#include <avr/eeprom.h>
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"


void loadConfiguration(void)
{
//  eeprom_read_block(lockSensors, lockerSensorsEEP, 4*sizeof(struct lockerSensor));
}

void saveConfiguration(void)
{
  //saveNic();
  //ipSaveConfig();
  //udpSaveConfig();
}
