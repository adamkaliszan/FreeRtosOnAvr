#include "configuration.h"
#include <avr/eeprom.h>
#include "hardware.h"
#include "hardwareConfig.h"
#include "softwareConfig.h"



uint8_t  confEepHC12mode    __attribute__((section (".eeprom"))) = 1;    //1 - 3
uint16_t confEepHC12baud    __attribute__((section (".eeprom"))) = 9600; //
uint8_t  confEepHC12channel __attribute__((section (".eeprom"))) = 10;   //1-100
uint8_t  confEepHC12power   __attribute__((section (".eeprom"))) = 1;    //1-8

uint8_t  confEepPwmFreq     __attribute__((section (".eeprom"))) = 6;    //4 - 7   4: 40 kHZ, 5: 5kHz, 6:  7: 320 Hz


uint8_t  confHC12mode;  //1 - 3
uint16_t confHC12baud; //
uint8_t  confHC12channel;
uint8_t  confHC12power; //1-8


void loadConfiguration(void)
{
  confHC12mode    = eeprom_read_byte(&confEepHC12mode);
  confHC12baud    = eeprom_read_word(&confEepHC12baud);
  confHC12channel = eeprom_read_byte(&confEepHC12channel);
  confHC12power   = eeprom_read_byte(&confEepHC12power);

  uint8_t tmp = eeprom_read_byte(&confEepPwmFreq);
  if (tmp >= 4 && tmp <=7)
  {
    TCC0.CTRLA &= 0xF0;
    TCC0.CTRLA |= tmp;
  }
  //Preskaler 256  PWM @ 1.28 kHz
}

void saveConfiguration(void)
{
  eeprom_write_byte(&confEepHC12mode,    confHC12mode);
  eeprom_write_word(&confEepHC12baud,    confHC12baud);
  eeprom_write_byte(&confEepHC12channel, confHC12channel);
  eeprom_write_byte(&confEepHC12power,   confHC12power);

  eeprom_write_byte(&confEepPwmFreq,   (TCC0.CTRLA & 0x0F));
}
