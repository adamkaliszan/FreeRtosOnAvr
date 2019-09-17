cmake_minimum_required(VERSION 2.8)
PROJECT("Main Controller Template")
SET(CMAKE_SYSTEM_NAME "Main Controller Template")

SET(MCU               atmega128)
SET(PROGRAMMER_DEV    /dev/avrMultiTool)

SET(IMAGENAME         firmware)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

# Output format. (can be srec, ihex, binary)
SET(FORMAT       "ihex")

# Target file name (without extension).
SET(TARGET_NAME  ${IMAGENAME})


SET(SOURCE_DIR   "../../freeRtos/Source")
SET(LIB_DIR      "../../freeRtos/Lib")
SET(LIB_NET_DIR  "../../freeRtos/Lib/net")
SET(PORT_DIR     "../../freeRtos/portable/GCC/ATMega64")
SET(PORT_MEM     "../../freeRtos/portable/MemMang")

include_directories("./" "${SOURCE_DIR}/include" "${LIB_DIR}/include" "${LIB_NET_DIR}/include" "${PORT_DIR}")

SET(CSTANDARD    "-std=gnu99")
SET(CDEBUG       "-gstabs")
SET(CWARN        "-Wall -Wstrict-prototypes")
SET(CTUNING      "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT         "-Os")
SET(CINCS        "-I. -I${SOURCE_DIR}/include -I${LIB_DIR}/include -I${LIB_NET_DIR}/include")
SET(CMCU         "-mmcu=${MCU}")
SET(CDEFS        "-DF_CPU=14745600")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT}")

SET(CMAKE_C_FLAGS ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})


add_executable(${IMAGENAME}
                        main.c configuration.c serial.c hardware.c sensors_task.c vty.c netstack_task.c cli_task.c 
                        ${LIB_DIR}/spi.c ${LIB_DIR}/memory_x.c ${LIB_DIR}/ramdysk.c ${LIB_DIR}/cmdline.c ${LIB_DIR}/xmodem.c 
                        ${LIB_DIR}/vt100.c ${LIB_DIR}/ds1305.c ${LIB_DIR}/mpc23s17.c ${LIB_DIR}/mcp3008.c ${LIB_DIR}/mcp4150.c
                        ${LIB_DIR}/enc28j60.c ${LIB_DIR}/queueStream.c 
                        ${LIB_NET_DIR}/nic.c ${LIB_NET_DIR}/net.c ${LIB_NET_DIR}/ip.c ${LIB_NET_DIR}/icmp.c ${LIB_NET_DIR}/arp.c 
                        ${LIB_NET_DIR}/tcp.c ${LIB_NET_DIR}/udp.c ${LIB_DIR}/Rs485_prot.c 
                        ${SOURCE_DIR}/tasks.c ${SOURCE_DIR}/queue.c ${SOURCE_DIR}/list.c ${SOURCE_DIR}/croutine.c 
                        ${PORT_MEM}/heap_avr.c
                        ${PORT_DIR}/port.c )



add_custom_target(mainController  DEPENDS
                        main.c                                  main.h
                        configuration.c                         configuration.h                         # Load / save configuration
                        hardware.c                              hardware.h
                        serial.c                                serial.h
                        vty.c                                   vty.h  
                        sensors_task.c                          sensors_task.h
                        netstack_task.c                         netstack_task.h
                        cli_task.c                              cli_task.h
                 )

add_custom_target(languageFiles   DEPENDS
                        vty_pl.h                                vty_en.h
                        Rs485_prot_pl.h                         Rs485_prot_en.h
                        hardware_pl.h                           hardware_en.h
                 )

add_custom_target(Config          DEPENDS
                        hardwareConfig.h                                                                # Hardware config
                        FreeRTOSConfig.h                                                                # FreeRTOSConfig.h
                        softwareConfig.h                                                                # Software configuration ie. language
                        ffconf.h                                                                        # Petits File system config
                 )

add_custom_target(Protocols       DEPENDS
                        ${LIB_DIR}/vt100.c                      ${LIB_DIR}/include/vt100.h              # Obsługa sekwencji stosowanytch w VT100
                        ${LIB_DIR}/cmdline.c                    ${LIB_DIR}/include/cmdline.h            # Obsługa interpretera poleceń
                        ${LIB_DIR}/xmodem.c                     ${LIB_DIR}/include/xmodem.h             # Obsługa systemu transmisji Xmodem
                        ${LIB_DIR}/Rs485_prot.c                 ${LIB_DIR}/include/Rs485_prot.h         # Obsługa protokołu komunikującego się przez magistralę Rs485 z modułami wykonawczymi  
                                                                ${LIB_DIR}/include/protocol1.h          # Rs485 Protocol constans
                 )

add_custom_target(FreeRtos        DEPENDS
                                                                ${SOURCE_DIR}/include/FreeRTOS.h  
                                                                ${SOURCE_DIR}/include/semphr.h
                                                                ${SOURCE_DIR}/include/portable.h 
                        ${SOURCE_DIR}/list.c                    ${SOURCE_DIR}/include/list.h  
                        ${SOURCE_DIR}/queue.c                   ${SOURCE_DIR}/include/queue.h  
                        ${SOURCE_DIR}/tasks.c                   ${SOURCE_DIR}/include/task.h
                        ${LIB_DIR}/queueStream.c                ${LIB_DIR}/include/queueStream.h        # obsługa strumienia FILE na potrzeby biblioteki libc
                 )

add_custom_target(Hardware        DEPENDS
                        ${LIB_DIR}/spi.c                        ${LIB_DIR}/include/spi.h                # Obsługa magistrali SPI
                        ${LIB_DIR}/memory_x.c                   ${LIB_DIR}/include/memory_x.h           # Obsługa malloc. Zlicza wolną i zajętą pamięć na stercie.
                        ${LIB_DIR}/mpc23s17.c                   ${LIB_DIR}/include/mpc23s17.h           # Obsługa ekspandera portu
                        ${LIB_DIR}/mcp3008.c                    ${LIB_DIR}/include/mcp3008.h            # Obsługa przetwornika A/C
                        ${LIB_DIR}/mcp4150.c                    ${LIB_DIR}/include/mcp4150.h            # Obsługa przetwornika A/C
                        ${LIB_DIR}/ds1305.c                     ${LIB_DIR}/include/ds1305.h             # Obsługa zegara RTC
                        ${LIB_DIR}/enc28j60.c                   ${LIB_DIR}/include/enc28j60.h           # Obsługa sterownika ethernetowego
                        ${LIB_DIR}/sd_diskio.c                  ${LIB_DIR}/include/sd_diskio.h          # Obsługa karty SD 
                 )

add_custom_target(NetworkStack    DEPENDS    
                        ${LIB_NET_DIR}/nic.c                    ${LIB_NET_DIR}/include/nic.h            # Hardware abstraction 
                        ${LIB_NET_DIR}/net.c                    ${LIB_NET_DIR}/include/net.h            # Network frames, packets, etc.
                        ${LIB_NET_DIR}/ip.c                     ${LIB_NET_DIR}/include/ip.h             # IPv4 support
                        ${LIB_NET_DIR}/arp.c                    ${LIB_NET_DIR}/include/arp.h            # Arp support
                        ${LIB_NET_DIR}/icmp.c                   ${LIB_NET_DIR}/include/icmp.h           # Ping
                        ${LIB_NET_DIR}/tcp.c                    ${LIB_NET_DIR}/include/tcp.h            # Tcp protocol
                        ${LIB_NET_DIR}/udp.c                    ${LIB_NET_DIR}/include/udp.h            # Udp protocol
                 )

add_custom_target(Filesystem      DEPENDS
                        ${LIB_DIR}/ramdysk.c                    ${LIB_DIR}/include/ramdysk.h            # Obsługa systemu plików Fat8
                        ${LIB_DIR}/ff.c                         ${LIB_DIR}/include/ff.h                 # Obsługa systemu plików Fat32
                        ${LIB_DIR}/include/integer.h                               
                 )

ADD_CUSTOM_COMMAND(
    OUTPUT ${IMAGENAME}.hex
    COMMAND avr-objcopy -O ${FORMAT} -R .eeprom ${IMAGENAME} ${IMAGENAME}.hex
    DEPENDS ${IMAGENAME}
)
 
ADD_CUSTOM_COMMAND(
    OUTPUT ${IMAGENAME}.eep
    COMMAND avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ${FORMAT} ${IMAGENAME} ${IMAGENAME}.eep
    DEPENDS ${IMAGENAME}
)

add_custom_target(program
    COMMAND avrdude -p ${MCU} -P ${PROGRAMMER_DEV} -c jtag1 -U flash:w:${IMAGENAME}.hex -U eeprom:w:${IMAGENAME}.eep
    DEPENDS ${IMAGENAME}.hex ${IMAGENAME}.eep
    COMMENT "Upload firmware to microcontroller"
)
