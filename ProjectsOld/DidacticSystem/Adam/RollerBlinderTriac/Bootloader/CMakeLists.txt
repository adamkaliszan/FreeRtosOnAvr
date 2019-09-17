cmake_minimum_required(VERSION 2.8)
PROJECT("Bootloader do modułu wykonawczego na triakach")
SET(CMAKE_SYSTEM_NAME "Bootloader do modułu wykonawczego na triakach")

SET(CMAKE_C_COMPILER   avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(MCU             atmega168)
SET(PROGRAMMER_DEV  /dev/avrStk500v2prog)

SET(IMAGENAME       "bootloader")


SET(TARGET_NAME     ${IMAGENAME})


SET(CSTANDARD       "-std=gnu99")
SET(CDEBUG          "-gstabs")
SET(CWARN           "-Wall -Wstrict-prototypes")
SET(CTUNING         "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT            "-Os")
SET(CINCS           "-I.")
SET(CMCU            "-mmcu=${MCU}")
SET(CDEFS           "-DF_CPU=7372800")


SET(CFLAGS          "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS        "${CMCU} ${CDEFS} ${CINCS} ${COPT}")

SET(CMAKE_C_FLAGS   ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})
SET(FORMAT          "ihex")


add_executable(${IMAGENAME}.elf bootloader.c flash.c serial.c)


ADD_CUSTOM_COMMAND(
    OUTPUT ${IMAGENAME}.hex
    COMMAND avr-objcopy -O ${FORMAT} -R .eeprom ${IMAGENAME}.elf ${IMAGENAME}.hex
    DEPENDS ${IMAGENAME}.elf
    COMMENT "Create hex file"
)
 
ADD_CUSTOM_COMMAND(
    OUTPUT ${IMAGENAME}.eep
    COMMENT "Create eep file"
    COMMAND avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ${FORMAT} ${IMAGENAME}.elf ${IMAGENAME}.eep
    DEPENDS ${IMAGENAME}.hex
)

add_custom_target(install
    COMMAND @avr-size -A ${IMAGENAME}.elf
    COMMAND avr-objcopy -O ${FORMAT} -R .eeprom ${IMAGENAME}.elf ${IMAGENAME}.hex
    COMMAND @avrdude -p ${MCU} -P ${PROGRAMMER_DEV} -c stk500v2 -U flash:w:${IMAGENAME}.hex
#    COMMAND @avrdude -p ${MCU} -P ${PROGRAMMER_DEV} -c stk500v2 -U lfuse:w:0xfD:m -U hfuse:w:0xdf:m
#    COMMAND @avrdude -p ${MCU} -P ${PROGRAMMER_DEV} -c stk500v2 -U efuse:w:0xf8:m
    DEPENDS ${IMAGENAME}.elf
    COMMENT "Upload firmware to microcontroller"
    COMMENT "Programming device"
)

add_custom_target(fuse
    COMMAND @avrdude -p ${MCU} -P ${PROGRAMMER_DEV} -c stk500v2 -U lfuse:w:0xfD:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m
)
