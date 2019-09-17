#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <avr/eeprom.h>
#include "enc28j60.h"
#include "hardware.h"
#include "ip.h"
#include "nic.h"
#include "udp.h"

uint8_t NoOfSerialPorts;
uint8_t NoOfSpiSlaves;

extern struct lockerSensor *lockSensors;

void loadConfiguration(void);
void saveConfiguration(void);

void spiLoadConfig(void);
void spiSaveConfig(void);

#endif
