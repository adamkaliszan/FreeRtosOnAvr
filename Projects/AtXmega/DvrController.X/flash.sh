#/bin/bash

TARGET=dist/default/production/DvrController.X.production
MCU=atxmega128a4u

AVRDUDE=avrdude
AVRDUDE_PROGRAMMER=avrispmkII
AVRDUDE_PORT=usb
AVRDUDE_FLAGS="-p $MCU -P $AVRDUDE_PORT -c $AVRDUDE_PROGRAMMER"
AVRDUDE_WRITE_FLASH="-U flash:w:$TARGET"".elf"
AVRDUDE_WRITE_EEPROM="-U eeprom:w:$TARGET.elf"

AVRDUDE_READ_EEPROM="-U eeprom:r:$TARGET""readed.eep"
AVRDUDE_WRITE_FUSES_H="-U hfuse:w:0x90:m"
AVRDUDE_READ_FUSES_H="-U hfuse:r:$TARGET"".hfuse:i"
AVRDUDE_WRITE_FUSES_L="-U lfuse:w:0xff:m"
AVRDUDE_READ_FUSES_L="-U lfuse:r:$TARGET"".lfuse:i"
AVRDUDE_WRITE_FUSES_E="-U efuse:w:0xff:m"
AVRDUDE_READ_FUSES_E="-U efuse:r:$TARGET"".efuse:i"


make -f MakefileOld clean
make -f MakefileOld program

#first command:
#avr-objcopy -j .eeprom -O ihex $TARGET.hex main.eep

#avr-objcopy -j .text -j .progmemx -j .bss -j .data -O ihex dist/default/production/DvrController.X.production.elf firmware.hex
#avr-objcopy -j .eeprom -O ihex dist/default/production/DvrController.X.production.elf firmware.eep

#second try:
#avr-objcopy --change-section-lma .eeprom=0 -O ihex main main.eep
#third try:
#avr-objcopy --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex main main.eep

#I used the first command for month... and it currently works correctly on my laptop.

#yes... I know... I forgot -j .eeprom :P
#tryed now and it works also on desktop. I don't understand why it works like a sharm on my laptop and why until last time I used it worked on my desktop too. I updated avr-gcc and other library the other day...

#$AVRDUDE $AVRDUDE_FLAGS $AVRDUDE_WRITE_FLASH $AVRDUDE_WRITE_EEPROM

#echo $AVRDUDE $AVRDUDE_FLAGS $AVRDUDE_WRITE_FLASH $AVRDUDE_WRITE_EEPROM

