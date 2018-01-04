#include "configuration.h"
#include <avr/eeprom.h>
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"
#include "nic.h"

uint32_t udpIpDst_eep   __attribute__((section (".eeprom"))) = ((uint32_t)UDP_DST_IP4   << 24) + ((uint32_t)UDP_DST_IP3   <<16) + ((uint32_t)UDP_DST_IP2   <<8) + UDP_DST_IP1;
uint16_t udpPortDstEep  __attribute__((section (".eeprom"))) = HTONS(UDP_DST_PORT);
uint16_t udpPortSrcEep  __attribute__((section (".eeprom"))) = HTONS(UDP_SRC_PORT);

static uint8_t  NoOfSerialPortsEep __attribute__((section (".eeprom"))) = 8;//HTONS(UDP_SRC_PORT);


void loadConfiguration()
{
  nicLoadConfig();
  ipLoadConfig();
  udpLoadConfig();
  spiLoadConfig();
}

void saveConfiguration()
{
  nicSaveConfig();
  ipSaveConfig();
  udpSaveConfig();
  spiSaveConfig();
}

void spiLoadConfig()
{
  NoOfSerialPorts       = eeprom_read_byte(&NoOfSerialPortsEep);
  NoOfSpiSlaves         = (NoOfSerialPorts+1)>>1;
}

void spiSaveConfig()
{
  eeprom_update_byte(&NoOfSerialPortsEep,  NoOfSerialPorts);
}
