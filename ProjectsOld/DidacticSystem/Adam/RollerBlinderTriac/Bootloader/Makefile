###############################################################################
# Makefile for the project bootloader
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
OBJECTS = bootloader.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET).elf bootloader.hex bootloader.eep size

## Compile
%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(INCLUDES) $(CFLAGS) $< -o $@


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
#AVRDUDE_PROGRAMMER  = stk500v2
#AVRDUDE_PORT = /dev/avrStk500v2prog	   # Stk500v2 on dedicated PCB
AVRDUDE_PROGRAMMER  = avrispmkII
AVRDUDE_PORT = usb
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_FLAGS       = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

AVR_DUDE_WRITE_FUSES = -U lfuse:w:0xff:m -U hfuse:w:0xdf:m -U efuse:w:0x00:m 

program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVR_DUDE_WRITE_FUSES) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) bootloader.elf dep/* bootloader.hex bootloader.eep

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)
