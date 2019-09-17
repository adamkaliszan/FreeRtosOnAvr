###############################################################################
# Makefile for the project Bootldr
###############################################################################

## General Flags
PROJECT = Bootloader
MCU     = atmega168
TARGET  = bootloader
CC      = avr-gcc
CPP     = avr-g++

## Options common to compile, link and assembly rules
COMMON  = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -DF_CPU=7372800UL -Os -fsigned-char
#CFLAGS += -gdwarf-2 
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-section-start=.text=0x3800

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

## Objects that must be built in order to link
OBJECTS = bootldr.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET).elf Bootldr.hex Bootldr.eep size

## Compile
bootldr.o: bootldr.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET).elf

%.hex: $(TARGET).elf
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS) $< $@

%.eep: $(TARGET).elf
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET).elf
	avr-objdump -h -S $< > $@

size: $(TARGET).hex
	@echo
	@avr-size -A $(TARGET).elf

# Program the device.
AVRDUDE             = avrdude
AVRDUDE_PROGRAMMER  = stk500v2
#AVRDUDE_PORT = /dev/avrStk500v2prog	   # Stk500v2 on dedicated PCB
AVRDUDE_PORT = /dev/avrMultiTool          # Stk500v2 on multi PCB
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex -U lfuse:w:0xff:m -U hfuse:w:0xdf:m 
#-U efuse:w:0xf8:m 
AVRDUDE_FLAGS       = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

AVR_DUDE_WRITE_FUSES = -U lfuse:w:0xff:m -U hfuse:w:0xdf:m 
#-U efuse:w:0xf8:m something is not working with fuse extended. Please enable bootloader manualy.

program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM) $(AVR_DUDE_WRITE_FUSES)

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Bootldr.elf dep/* Bootldr.hex Bootldr.eep

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)
