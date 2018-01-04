cmake_minimum_required(VERSION 2.8)
PROJECT("Moduł wykonawczy triaki")
SET(CMAKE_SYSTEM_NAME "Moduł wykonawczy triaki")

SET(CMAKE_C_COMPILER   avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(MCU             atmega168)
#SET(PROGRAMMER_DEV  /dev/avrStk500v2prog)
SET(PROGRAMMER_DEV  usb)

SET(IMAGENAME       "firmware")


SET(${CMAKE_SYSTEM_NAME}_BINARY_DIR "./build")


SET(TARGET_NAME     ${IMAGENAME})


SET(SOURCE_DIR      "../../freeRtos/Source")
SET(LIB_DIR         "../../freeRtos/Lib")
SET(LIB_NET_DIR     "../../freeRtos/Lib/net")
SET(PORT_DIR        "../../freeRtos/portable/GCC/ATMega168")
SET(PORT_MEM        "../../freeRtos/portable/MemMang")

include_directories("./" "${SOURCE_DIR}/include" "${LIB_DIR}/include" "${LIB_NET_DIR}/include" "${PORT_DIR}")

SET(CSTANDARD       "-std=gnu99")
SET(CDEBUG          "-gstabs")
SET(CWARN           "-Wall -Wstrict-prototypes")
SET(CTUNING         "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT            "-Os")
SET(CINCS           "-I. -I${SOURCE_DIR}/include -I${LIB_DIR}/include -I${LIB_NET_DIR}/include")
SET(CMCU            "-mmcu=${MCU}")
SET(CDEFS           "-DF_CPU=7372800")


SET(CFLAGS          "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA} -Wl,-Map=${IMAGENAME}.map,--cref -lm")
SET(CXXFLAGS        "${CMCU} ${CDEFS} ${CINCS} ${COPT}")

SET(CMAKE_C_FLAGS   ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})
SET(FORMAT          "ihex")


add_executable(${IMAGENAME}.elf
                    main.c automat.c hardware.c serial.c 
                    ${SOURCE_DIR}/tasks.c ${SOURCE_DIR}/queue.c ${SOURCE_DIR}/list.c ${SOURCE_DIR}/croutine.c 
                    ${PORT_MEM}/heap_avr.c
                    ${PORT_DIR}/port.c )


add_custom_target(FreeRtos        DEPENDS
                                                            ${SOURCE_DIR}/include/FreeRTOS.h  
                                                            ${SOURCE_DIR}/include/semphr.h
                                                            ${SOURCE_DIR}/include/portable.h 
                    ${SOURCE_DIR}/list.c                    ${SOURCE_DIR}/include/list.h  
                    ${SOURCE_DIR}/queue.c                   ${SOURCE_DIR}/include/queue.h  
                    ${SOURCE_DIR}/tasks.c                   ${SOURCE_DIR}/include/task.h
                    ${LIB_DIR}/queueStream.c                ${LIB_DIR}/include/queueStream.h        # obsługa strumienia FILE na potrzeby biblioteki libc
                 )

ADD_CUSTOM_COMMAND(
    OUTPUT ${IMAGENAME}.hex
    COMMENT "Create hex file"
    COMMAND avr-objcopy -O ${FORMAT} -R .eeprom ${IMAGENAME}.elf ${IMAGENAME}.hex
    DEPENDS ${IMAGENAME}.elf
)
 
ADD_CUSTOM_COMMAND(
    OUTPUT ${IMAGENAME}.eep
    COMMENT "Create eep file"
    COMMAND avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ${FORMAT} ${IMAGENAME}.elf ${IMAGENAME}.eep
    DEPENDS ${IMAGENAME}.hex
)

add_custom_target(install
    COMMAND @avr-size -A ${IMAGENAME}.elf
    COMMAND avrdude -p ${MCU} -P ${PROGRAMMER_DEV} -c stk500v2 -U flash:w:${IMAGENAME}.hex -U eeprom:w:${IMAGENAME}.eep
    COMMENT "Programming device"
    DEPENDS ${IMAGENAME}.elf
    DEPENDS ${IMAGENAME}.hex
    DEPENDS ${IMAGENAME}.eep
    COMMENT "Upload firmware to microcontroller"
)
