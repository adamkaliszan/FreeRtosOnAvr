#ifndef LANG_RS485_PROT
#define LANG_RS485_PROT EN

#include <avr/pgmspace.h>

const char statusRollerDescStr[]      PROGMEM = " %d roller driver: roller 1 position %d, roller 2 position %d";
const char statusRollerDescStrConf[]  PROGMEM = " config %x";
const char statusRollerDescStr2[]     PROGMEM = ", firmware %s\r\n";

#endif