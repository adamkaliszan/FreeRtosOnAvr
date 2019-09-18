#/bin/bash

TARGET=dist/default/production/RoverController.X.production
MCU=atxmega128a4u

AVRDUDE=avrdude
AVRDUDE_PROGRAMMER=avrispmkII
AVRDUDE_PORT=usb
AVRDUDE_FLAGS="-p $MCU -P $AVRDUDE_PORT -c $AVRDUDE_PROGRAMMER"
AVRDUDE_WRITE_FLASH="-U flash:w:$TARGET"".hex"
AVRDUDE_WRITE_EEPROM="-U eeprom:w:$TARGET.eep"

AVRDUDE_READ_EEPROM="-U eeprom:r:$TARGET""readed.eep"
AVRDUDE_WRITE_FUSES_H="-U hfuse:w:0x90:m"
AVRDUDE_READ_FUSES_H="-U hfuse:r:$TARGET"".hfuse:i"
AVRDUDE_WRITE_FUSES_L="-U lfuse:w:0xff:m"
AVRDUDE_READ_FUSES_L="-U lfuse:r:$TARGET"".lfuse:i"
AVRDUDE_WRITE_FUSES_E="-U efuse:w:0xff:m"
AVRDUDE_READ_FUSES_E="-U efuse:r:$TARGET"".efuse:i"

avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex $TARGET.elf firmwareFromElf.eep
avr-objcopy -R .eeprom -O ihex $TARGET.elf firmwareFromElf.hex

echo "Start avr dude"
echo avrdude -p atxmega128a4u -P usb -c avrispmkII -U flash:w:firmwareFromElf.hex -U eeprom:w:firmwareFromElf.eep

avrdude -p atxmega128a4u -P usb -c avrispmkII -U flash:w:firmwareFromElf.hex -U eeprom:w:firmwareFromElf.eep

