#include "configuration.h"
#include <avr/eeprom.h>
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"
#include "nic.h"


static struct lockerSensor lockerSensorsEEP[4] __attribute__((section (".eeprom"))) = {{LOCK_SENS_1_ENA, LOCK_SENS_1_THR, 0, 0}, 
                                                                                       {LOCK_SENS_2_ENA, LOCK_SENS_2_THR, 0, 0},
                                                                                       {LOCK_SENS_3_ENA, LOCK_SENS_3_THR, 0, 0},
                                                                                       {LOCK_SENS_4_ENA, LOCK_SENS_4_THR, 0, 0}};

void loadConfiguration(void)
{
  eeprom_read_block(lockSensors, lockerSensorsEEP, 4*sizeof(struct lockerSensor));
}

void saveConfiguration(void)
{
  saveNic();
  ipSaveConfig();
  udpSaveConfig();
}
