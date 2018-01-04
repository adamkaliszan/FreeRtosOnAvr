# makefile, written by guido socher
MCU=atmega8
CC=avr-gcc
OBJCOPY=avr-objcopy
# optimize for size:
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues

TARGET = avrusb500

# Programming support using avrdude.
AVRDUDE = avrdude
AVRDUDE_PROGRAMMER = stk500v2
AVRDUDE_PORT = /dev/avrStk500v2prog	   # Stk500v2 on dedicated PCB
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_WRITE_FLASH_PRE = -U flash:w:avrusb500_pre.hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE += -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_FLAGS += -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude> 
# to submit bug reports.
#AVRDUDE_FLAGS += -v -v

#-------------------
all: avrusb500.hex test_1.hex test_2.hex ttydevinit1152
#-------------------
help: 
	@echo "Usage: make help"
	@echo "       Print this help"
	@echo " "
	@echo "Usage: make all|program|program_pre"
	@echo " "
	@echo "Usage: make clean"
	@echo "       delete all generated files except the pre-compiled ones"
#-------------------
avrusb500.hex : avrusb500.out 
	$(OBJCOPY) -R .eeprom -O ihex avrusb500.out avrusb500.hex 
	avr-size avrusb500.out
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "
avrusb500.out : main.o uart.o spi.o timeout.o
	$(CC) $(CFLAGS) -o avrusb500.out -Wl,-Map,avrusb500.map main.o uart.o spi.o timeout.o
main.o : main.c led.h avr_compat.h command.h spi.h uart.h timeout.h
	$(CC) $(CFLAGS) -Os -c main.c
#-------------------
timeout.o : timeout.c timeout.h avr_compat.h
	$(CC) $(CFLAGS) -Os -c timeout.c
#-------------------
spi.o : spi.c spi.h avr_compat.h timeout.h
	$(CC) $(CFLAGS) -Os -c spi.c
#-------------------
uart.o : uart.c uart.h avr_compat.h timeout.h
	$(CC) $(CFLAGS) -Os -c uart.c
#-------------------
# avrdude is better than uisp because it can verify the eeprom:
program_pre: avrusb500.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH_PRE) -U lfuse:w:0xff:m -U hfuse:w:0xd9:m 


# Program the device.  
program: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) -U lfuse:w:0xff:m -U hfuse:w:0xd9:m 

#-------------------
clean:
	rm -f *.o *.map *.out test_2.hex avrusb500.hex
#-------------------
