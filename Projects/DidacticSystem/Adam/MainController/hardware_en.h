#ifndef LANG_HARDWARE
#define LANG_HARDWARE EN

#include<avr/pgmspace.h>
const char statusLockerSensDescStr[]           PROGMEM = " locker %d";
const char statusLockerOpenStr[]               PROGMEM = " open   ";
const char statusLockerCloseStr[]              PROGMEM = " locked ";
const char statusLockerSensAdditionalDescStr[] PROGMEM = " (threshold %d, AC value %d)\r\n";

#endif
