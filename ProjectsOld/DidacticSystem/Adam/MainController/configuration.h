#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <avr/eeprom.h>
#include "enc28j60.h"
#include "hardware.h"
#include "ip.h"
#include "nic.h"
#include "sensors_task.h"
#include "udp.h"

extern struct lockerSensor *lockSensors;

void loadConfiguration(void);
void saveConfiguration(void);

void saveNic(void);

#endif
