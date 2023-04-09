#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <avr/eeprom.h>
#include "hardware.h"

extern struct lockerSensor *lockSensors;

void loadConfiguration(void);
void saveConfiguration(void);


#endif
