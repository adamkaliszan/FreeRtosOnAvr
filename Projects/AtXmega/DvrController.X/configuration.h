#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <avr/eeprom.h>
#include "hardware.h"

#ifdef USE_NET
#include "enc28j60.h"
#include "ip.h"
#include "nic.h"
#include "udp.h"
#endif

void loadConfiguration(void);
void saveConfiguration(void);


#endif
