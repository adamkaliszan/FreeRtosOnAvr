#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=source/cli_task.c source/configuration.c source/hardware.c source/main.c source/netstack_task.c source/rtc.c source/sensors_task.c source/serial.c source/vty.c ../../../FreeRtosCore/Source/croutine.c ../../../FreeRtosCore/Source/list.c ../../../FreeRtosCore/Source/queue.c ../../../FreeRtosCore/Source/tasks.c ../../../FreeRtosCore/portable/GCC/ATMega64/port.c ../../../FreeRtosCore/portable/MemMang/heap_avr.c ../../../Lib/Rs485_prot.c ../../../Lib/cmdline.c ../../../Lib/ramdysk.c ../../../Lib/xmodem.c ../../../Lib/memory_x.c ../../../Lib/mcp3008.c ../../../Lib/enc28j60.c ../../../Lib/mcp4150.c ../../../Lib/mpc23s17.c ../../../Lib/spi.c ../../../Lib/net/arp.c ../../../Lib/net/icmp.c ../../../Lib/net/ip.c ../../../Lib/net/net.c ../../../Lib/net/nic.c ../../../Lib/net/udp.c ../../../Lib/net/tcp.c ../../../Lib/ds1305.c ../../../Lib/queueStream.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/source/cli_task.o ${OBJECTDIR}/source/configuration.o ${OBJECTDIR}/source/hardware.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/netstack_task.o ${OBJECTDIR}/source/rtc.o ${OBJECTDIR}/source/sensors_task.o ${OBJECTDIR}/source/serial.o ${OBJECTDIR}/source/vty.o ${OBJECTDIR}/_ext/1326544842/croutine.o ${OBJECTDIR}/_ext/1326544842/list.o ${OBJECTDIR}/_ext/1326544842/queue.o ${OBJECTDIR}/_ext/1326544842/tasks.o ${OBJECTDIR}/_ext/178352208/port.o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o ${OBJECTDIR}/_ext/1386490678/cmdline.o ${OBJECTDIR}/_ext/1386490678/ramdysk.o ${OBJECTDIR}/_ext/1386490678/xmodem.o ${OBJECTDIR}/_ext/1386490678/memory_x.o ${OBJECTDIR}/_ext/1386490678/mcp3008.o ${OBJECTDIR}/_ext/1386490678/enc28j60.o ${OBJECTDIR}/_ext/1386490678/mcp4150.o ${OBJECTDIR}/_ext/1386490678/mpc23s17.o ${OBJECTDIR}/_ext/1386490678/spi.o ${OBJECTDIR}/_ext/1046042812/arp.o ${OBJECTDIR}/_ext/1046042812/icmp.o ${OBJECTDIR}/_ext/1046042812/ip.o ${OBJECTDIR}/_ext/1046042812/net.o ${OBJECTDIR}/_ext/1046042812/nic.o ${OBJECTDIR}/_ext/1046042812/udp.o ${OBJECTDIR}/_ext/1046042812/tcp.o ${OBJECTDIR}/_ext/1386490678/ds1305.o ${OBJECTDIR}/_ext/1386490678/queueStream.o
POSSIBLE_DEPFILES=${OBJECTDIR}/source/cli_task.o.d ${OBJECTDIR}/source/configuration.o.d ${OBJECTDIR}/source/hardware.o.d ${OBJECTDIR}/source/main.o.d ${OBJECTDIR}/source/netstack_task.o.d ${OBJECTDIR}/source/rtc.o.d ${OBJECTDIR}/source/sensors_task.o.d ${OBJECTDIR}/source/serial.o.d ${OBJECTDIR}/source/vty.o.d ${OBJECTDIR}/_ext/1326544842/croutine.o.d ${OBJECTDIR}/_ext/1326544842/list.o.d ${OBJECTDIR}/_ext/1326544842/queue.o.d ${OBJECTDIR}/_ext/1326544842/tasks.o.d ${OBJECTDIR}/_ext/178352208/port.o.d ${OBJECTDIR}/_ext/1817164221/heap_avr.o.d ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o.d ${OBJECTDIR}/_ext/1386490678/cmdline.o.d ${OBJECTDIR}/_ext/1386490678/ramdysk.o.d ${OBJECTDIR}/_ext/1386490678/xmodem.o.d ${OBJECTDIR}/_ext/1386490678/memory_x.o.d ${OBJECTDIR}/_ext/1386490678/mcp3008.o.d ${OBJECTDIR}/_ext/1386490678/enc28j60.o.d ${OBJECTDIR}/_ext/1386490678/mcp4150.o.d ${OBJECTDIR}/_ext/1386490678/mpc23s17.o.d ${OBJECTDIR}/_ext/1386490678/spi.o.d ${OBJECTDIR}/_ext/1046042812/arp.o.d ${OBJECTDIR}/_ext/1046042812/icmp.o.d ${OBJECTDIR}/_ext/1046042812/ip.o.d ${OBJECTDIR}/_ext/1046042812/net.o.d ${OBJECTDIR}/_ext/1046042812/nic.o.d ${OBJECTDIR}/_ext/1046042812/udp.o.d ${OBJECTDIR}/_ext/1046042812/tcp.o.d ${OBJECTDIR}/_ext/1386490678/ds1305.o.d ${OBJECTDIR}/_ext/1386490678/queueStream.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/source/cli_task.o ${OBJECTDIR}/source/configuration.o ${OBJECTDIR}/source/hardware.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/netstack_task.o ${OBJECTDIR}/source/rtc.o ${OBJECTDIR}/source/sensors_task.o ${OBJECTDIR}/source/serial.o ${OBJECTDIR}/source/vty.o ${OBJECTDIR}/_ext/1326544842/croutine.o ${OBJECTDIR}/_ext/1326544842/list.o ${OBJECTDIR}/_ext/1326544842/queue.o ${OBJECTDIR}/_ext/1326544842/tasks.o ${OBJECTDIR}/_ext/178352208/port.o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o ${OBJECTDIR}/_ext/1386490678/cmdline.o ${OBJECTDIR}/_ext/1386490678/ramdysk.o ${OBJECTDIR}/_ext/1386490678/xmodem.o ${OBJECTDIR}/_ext/1386490678/memory_x.o ${OBJECTDIR}/_ext/1386490678/mcp3008.o ${OBJECTDIR}/_ext/1386490678/enc28j60.o ${OBJECTDIR}/_ext/1386490678/mcp4150.o ${OBJECTDIR}/_ext/1386490678/mpc23s17.o ${OBJECTDIR}/_ext/1386490678/spi.o ${OBJECTDIR}/_ext/1046042812/arp.o ${OBJECTDIR}/_ext/1046042812/icmp.o ${OBJECTDIR}/_ext/1046042812/ip.o ${OBJECTDIR}/_ext/1046042812/net.o ${OBJECTDIR}/_ext/1046042812/nic.o ${OBJECTDIR}/_ext/1046042812/udp.o ${OBJECTDIR}/_ext/1046042812/tcp.o ${OBJECTDIR}/_ext/1386490678/ds1305.o ${OBJECTDIR}/_ext/1386490678/queueStream.o

# Source Files
SOURCEFILES=source/cli_task.c source/configuration.c source/hardware.c source/main.c source/netstack_task.c source/rtc.c source/sensors_task.c source/serial.c source/vty.c ../../../FreeRtosCore/Source/croutine.c ../../../FreeRtosCore/Source/list.c ../../../FreeRtosCore/Source/queue.c ../../../FreeRtosCore/Source/tasks.c ../../../FreeRtosCore/portable/GCC/ATMega64/port.c ../../../FreeRtosCore/portable/MemMang/heap_avr.c ../../../Lib/Rs485_prot.c ../../../Lib/cmdline.c ../../../Lib/ramdysk.c ../../../Lib/xmodem.c ../../../Lib/memory_x.c ../../../Lib/mcp3008.c ../../../Lib/enc28j60.c ../../../Lib/mcp4150.c ../../../Lib/mpc23s17.c ../../../Lib/spi.c ../../../Lib/net/arp.c ../../../Lib/net/icmp.c ../../../Lib/net/ip.c ../../../Lib/net/net.c ../../../Lib/net/nic.c ../../../Lib/net/udp.c ../../../Lib/net/tcp.c ../../../Lib/ds1305.c ../../../Lib/queueStream.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATmega128
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/source/cli_task.o: source/cli_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/cli_task.o.d 
	@${RM} ${OBJECTDIR}/source/cli_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/cli_task.o.d" -MT "${OBJECTDIR}/source/cli_task.o.d" -MT ${OBJECTDIR}/source/cli_task.o -o ${OBJECTDIR}/source/cli_task.o source/cli_task.c 
	
${OBJECTDIR}/source/configuration.o: source/configuration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/configuration.o.d 
	@${RM} ${OBJECTDIR}/source/configuration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/configuration.o.d" -MT "${OBJECTDIR}/source/configuration.o.d" -MT ${OBJECTDIR}/source/configuration.o -o ${OBJECTDIR}/source/configuration.o source/configuration.c 
	
${OBJECTDIR}/source/hardware.o: source/hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/hardware.o.d 
	@${RM} ${OBJECTDIR}/source/hardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/hardware.o.d" -MT "${OBJECTDIR}/source/hardware.o.d" -MT ${OBJECTDIR}/source/hardware.o -o ${OBJECTDIR}/source/hardware.o source/hardware.c 
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/main.o.d" -MT "${OBJECTDIR}/source/main.o.d" -MT ${OBJECTDIR}/source/main.o -o ${OBJECTDIR}/source/main.o source/main.c 
	
${OBJECTDIR}/source/netstack_task.o: source/netstack_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/netstack_task.o.d 
	@${RM} ${OBJECTDIR}/source/netstack_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/netstack_task.o.d" -MT "${OBJECTDIR}/source/netstack_task.o.d" -MT ${OBJECTDIR}/source/netstack_task.o -o ${OBJECTDIR}/source/netstack_task.o source/netstack_task.c 
	
${OBJECTDIR}/source/rtc.o: source/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/rtc.o.d 
	@${RM} ${OBJECTDIR}/source/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/rtc.o.d" -MT "${OBJECTDIR}/source/rtc.o.d" -MT ${OBJECTDIR}/source/rtc.o -o ${OBJECTDIR}/source/rtc.o source/rtc.c 
	
${OBJECTDIR}/source/sensors_task.o: source/sensors_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/sensors_task.o.d 
	@${RM} ${OBJECTDIR}/source/sensors_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/sensors_task.o.d" -MT "${OBJECTDIR}/source/sensors_task.o.d" -MT ${OBJECTDIR}/source/sensors_task.o -o ${OBJECTDIR}/source/sensors_task.o source/sensors_task.c 
	
${OBJECTDIR}/source/serial.o: source/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/serial.o.d 
	@${RM} ${OBJECTDIR}/source/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/serial.o.d" -MT "${OBJECTDIR}/source/serial.o.d" -MT ${OBJECTDIR}/source/serial.o -o ${OBJECTDIR}/source/serial.o source/serial.c 
	
${OBJECTDIR}/source/vty.o: source/vty.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/vty.o.d 
	@${RM} ${OBJECTDIR}/source/vty.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/vty.o.d" -MT "${OBJECTDIR}/source/vty.o.d" -MT ${OBJECTDIR}/source/vty.o -o ${OBJECTDIR}/source/vty.o source/vty.c 
	
${OBJECTDIR}/_ext/1326544842/croutine.o: ../../../FreeRtosCore/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT ${OBJECTDIR}/_ext/1326544842/croutine.o -o ${OBJECTDIR}/_ext/1326544842/croutine.o ../../../FreeRtosCore/Source/croutine.c 
	
${OBJECTDIR}/_ext/1326544842/list.o: ../../../FreeRtosCore/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT ${OBJECTDIR}/_ext/1326544842/list.o -o ${OBJECTDIR}/_ext/1326544842/list.o ../../../FreeRtosCore/Source/list.c 
	
${OBJECTDIR}/_ext/1326544842/queue.o: ../../../FreeRtosCore/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT ${OBJECTDIR}/_ext/1326544842/queue.o -o ${OBJECTDIR}/_ext/1326544842/queue.o ../../../FreeRtosCore/Source/queue.c 
	
${OBJECTDIR}/_ext/1326544842/tasks.o: ../../../FreeRtosCore/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT ${OBJECTDIR}/_ext/1326544842/tasks.o -o ${OBJECTDIR}/_ext/1326544842/tasks.o ../../../FreeRtosCore/Source/tasks.c 
	
${OBJECTDIR}/_ext/178352208/port.o: ../../../FreeRtosCore/portable/GCC/ATMega64/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/178352208" 
	@${RM} ${OBJECTDIR}/_ext/178352208/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/178352208/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/178352208/port.o.d" -MT "${OBJECTDIR}/_ext/178352208/port.o.d" -MT ${OBJECTDIR}/_ext/178352208/port.o -o ${OBJECTDIR}/_ext/178352208/port.o ../../../FreeRtosCore/portable/GCC/ATMega64/port.c 
	
${OBJECTDIR}/_ext/1817164221/heap_avr.o: ../../../FreeRtosCore/portable/MemMang/heap_avr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1817164221" 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT ${OBJECTDIR}/_ext/1817164221/heap_avr.o -o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ../../../FreeRtosCore/portable/MemMang/heap_avr.c 
	
${OBJECTDIR}/_ext/1386490678/Rs485_prot.o: ../../../Lib/Rs485_prot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/Rs485_prot.o.d" -MT "${OBJECTDIR}/_ext/1386490678/Rs485_prot.o.d" -MT ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o -o ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o ../../../Lib/Rs485_prot.c 
	
${OBJECTDIR}/_ext/1386490678/cmdline.o: ../../../Lib/cmdline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/cmdline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/cmdline.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/cmdline.o.d" -MT "${OBJECTDIR}/_ext/1386490678/cmdline.o.d" -MT ${OBJECTDIR}/_ext/1386490678/cmdline.o -o ${OBJECTDIR}/_ext/1386490678/cmdline.o ../../../Lib/cmdline.c 
	
${OBJECTDIR}/_ext/1386490678/ramdysk.o: ../../../Lib/ramdysk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/ramdysk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/ramdysk.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/ramdysk.o.d" -MT "${OBJECTDIR}/_ext/1386490678/ramdysk.o.d" -MT ${OBJECTDIR}/_ext/1386490678/ramdysk.o -o ${OBJECTDIR}/_ext/1386490678/ramdysk.o ../../../Lib/ramdysk.c 
	
${OBJECTDIR}/_ext/1386490678/xmodem.o: ../../../Lib/xmodem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/xmodem.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/xmodem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/xmodem.o.d" -MT "${OBJECTDIR}/_ext/1386490678/xmodem.o.d" -MT ${OBJECTDIR}/_ext/1386490678/xmodem.o -o ${OBJECTDIR}/_ext/1386490678/xmodem.o ../../../Lib/xmodem.c 
	
${OBJECTDIR}/_ext/1386490678/memory_x.o: ../../../Lib/memory_x.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/memory_x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/memory_x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/memory_x.o.d" -MT "${OBJECTDIR}/_ext/1386490678/memory_x.o.d" -MT ${OBJECTDIR}/_ext/1386490678/memory_x.o -o ${OBJECTDIR}/_ext/1386490678/memory_x.o ../../../Lib/memory_x.c 
	
${OBJECTDIR}/_ext/1386490678/mcp3008.o: ../../../Lib/mcp3008.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mcp3008.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mcp3008.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/mcp3008.o.d" -MT "${OBJECTDIR}/_ext/1386490678/mcp3008.o.d" -MT ${OBJECTDIR}/_ext/1386490678/mcp3008.o -o ${OBJECTDIR}/_ext/1386490678/mcp3008.o ../../../Lib/mcp3008.c 
	
${OBJECTDIR}/_ext/1386490678/enc28j60.o: ../../../Lib/enc28j60.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/enc28j60.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/enc28j60.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/enc28j60.o.d" -MT "${OBJECTDIR}/_ext/1386490678/enc28j60.o.d" -MT ${OBJECTDIR}/_ext/1386490678/enc28j60.o -o ${OBJECTDIR}/_ext/1386490678/enc28j60.o ../../../Lib/enc28j60.c 
	
${OBJECTDIR}/_ext/1386490678/mcp4150.o: ../../../Lib/mcp4150.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mcp4150.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mcp4150.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/mcp4150.o.d" -MT "${OBJECTDIR}/_ext/1386490678/mcp4150.o.d" -MT ${OBJECTDIR}/_ext/1386490678/mcp4150.o -o ${OBJECTDIR}/_ext/1386490678/mcp4150.o ../../../Lib/mcp4150.c 
	
${OBJECTDIR}/_ext/1386490678/mpc23s17.o: ../../../Lib/mpc23s17.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mpc23s17.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mpc23s17.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/mpc23s17.o.d" -MT "${OBJECTDIR}/_ext/1386490678/mpc23s17.o.d" -MT ${OBJECTDIR}/_ext/1386490678/mpc23s17.o -o ${OBJECTDIR}/_ext/1386490678/mpc23s17.o ../../../Lib/mpc23s17.c 
	
${OBJECTDIR}/_ext/1386490678/spi.o: ../../../Lib/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/spi.o.d" -MT "${OBJECTDIR}/_ext/1386490678/spi.o.d" -MT ${OBJECTDIR}/_ext/1386490678/spi.o -o ${OBJECTDIR}/_ext/1386490678/spi.o ../../../Lib/spi.c 
	
${OBJECTDIR}/_ext/1046042812/arp.o: ../../../Lib/net/arp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/arp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/arp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/arp.o.d" -MT "${OBJECTDIR}/_ext/1046042812/arp.o.d" -MT ${OBJECTDIR}/_ext/1046042812/arp.o -o ${OBJECTDIR}/_ext/1046042812/arp.o ../../../Lib/net/arp.c 
	
${OBJECTDIR}/_ext/1046042812/icmp.o: ../../../Lib/net/icmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/icmp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/icmp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/icmp.o.d" -MT "${OBJECTDIR}/_ext/1046042812/icmp.o.d" -MT ${OBJECTDIR}/_ext/1046042812/icmp.o -o ${OBJECTDIR}/_ext/1046042812/icmp.o ../../../Lib/net/icmp.c 
	
${OBJECTDIR}/_ext/1046042812/ip.o: ../../../Lib/net/ip.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/ip.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/ip.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/ip.o.d" -MT "${OBJECTDIR}/_ext/1046042812/ip.o.d" -MT ${OBJECTDIR}/_ext/1046042812/ip.o -o ${OBJECTDIR}/_ext/1046042812/ip.o ../../../Lib/net/ip.c 
	
${OBJECTDIR}/_ext/1046042812/net.o: ../../../Lib/net/net.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/net.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/net.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/net.o.d" -MT "${OBJECTDIR}/_ext/1046042812/net.o.d" -MT ${OBJECTDIR}/_ext/1046042812/net.o -o ${OBJECTDIR}/_ext/1046042812/net.o ../../../Lib/net/net.c 
	
${OBJECTDIR}/_ext/1046042812/nic.o: ../../../Lib/net/nic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/nic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/nic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/nic.o.d" -MT "${OBJECTDIR}/_ext/1046042812/nic.o.d" -MT ${OBJECTDIR}/_ext/1046042812/nic.o -o ${OBJECTDIR}/_ext/1046042812/nic.o ../../../Lib/net/nic.c 
	
${OBJECTDIR}/_ext/1046042812/udp.o: ../../../Lib/net/udp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/udp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/udp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/udp.o.d" -MT "${OBJECTDIR}/_ext/1046042812/udp.o.d" -MT ${OBJECTDIR}/_ext/1046042812/udp.o -o ${OBJECTDIR}/_ext/1046042812/udp.o ../../../Lib/net/udp.c 
	
${OBJECTDIR}/_ext/1046042812/tcp.o: ../../../Lib/net/tcp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/tcp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/tcp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/tcp.o.d" -MT "${OBJECTDIR}/_ext/1046042812/tcp.o.d" -MT ${OBJECTDIR}/_ext/1046042812/tcp.o -o ${OBJECTDIR}/_ext/1046042812/tcp.o ../../../Lib/net/tcp.c 
	
${OBJECTDIR}/_ext/1386490678/ds1305.o: ../../../Lib/ds1305.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/ds1305.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/ds1305.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/ds1305.o.d" -MT "${OBJECTDIR}/_ext/1386490678/ds1305.o.d" -MT ${OBJECTDIR}/_ext/1386490678/ds1305.o -o ${OBJECTDIR}/_ext/1386490678/ds1305.o ../../../Lib/ds1305.c 
	
${OBJECTDIR}/_ext/1386490678/queueStream.o: ../../../Lib/queueStream.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/queueStream.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/queueStream.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/queueStream.o.d" -MT "${OBJECTDIR}/_ext/1386490678/queueStream.o.d" -MT ${OBJECTDIR}/_ext/1386490678/queueStream.o -o ${OBJECTDIR}/_ext/1386490678/queueStream.o ../../../Lib/queueStream.c 
	
else
${OBJECTDIR}/source/cli_task.o: source/cli_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/cli_task.o.d 
	@${RM} ${OBJECTDIR}/source/cli_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/cli_task.o.d" -MT "${OBJECTDIR}/source/cli_task.o.d" -MT ${OBJECTDIR}/source/cli_task.o -o ${OBJECTDIR}/source/cli_task.o source/cli_task.c 
	
${OBJECTDIR}/source/configuration.o: source/configuration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/configuration.o.d 
	@${RM} ${OBJECTDIR}/source/configuration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/configuration.o.d" -MT "${OBJECTDIR}/source/configuration.o.d" -MT ${OBJECTDIR}/source/configuration.o -o ${OBJECTDIR}/source/configuration.o source/configuration.c 
	
${OBJECTDIR}/source/hardware.o: source/hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/hardware.o.d 
	@${RM} ${OBJECTDIR}/source/hardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/hardware.o.d" -MT "${OBJECTDIR}/source/hardware.o.d" -MT ${OBJECTDIR}/source/hardware.o -o ${OBJECTDIR}/source/hardware.o source/hardware.c 
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/main.o.d" -MT "${OBJECTDIR}/source/main.o.d" -MT ${OBJECTDIR}/source/main.o -o ${OBJECTDIR}/source/main.o source/main.c 
	
${OBJECTDIR}/source/netstack_task.o: source/netstack_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/netstack_task.o.d 
	@${RM} ${OBJECTDIR}/source/netstack_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/netstack_task.o.d" -MT "${OBJECTDIR}/source/netstack_task.o.d" -MT ${OBJECTDIR}/source/netstack_task.o -o ${OBJECTDIR}/source/netstack_task.o source/netstack_task.c 
	
${OBJECTDIR}/source/rtc.o: source/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/rtc.o.d 
	@${RM} ${OBJECTDIR}/source/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/rtc.o.d" -MT "${OBJECTDIR}/source/rtc.o.d" -MT ${OBJECTDIR}/source/rtc.o -o ${OBJECTDIR}/source/rtc.o source/rtc.c 
	
${OBJECTDIR}/source/sensors_task.o: source/sensors_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/sensors_task.o.d 
	@${RM} ${OBJECTDIR}/source/sensors_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/sensors_task.o.d" -MT "${OBJECTDIR}/source/sensors_task.o.d" -MT ${OBJECTDIR}/source/sensors_task.o -o ${OBJECTDIR}/source/sensors_task.o source/sensors_task.c 
	
${OBJECTDIR}/source/serial.o: source/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/serial.o.d 
	@${RM} ${OBJECTDIR}/source/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/serial.o.d" -MT "${OBJECTDIR}/source/serial.o.d" -MT ${OBJECTDIR}/source/serial.o -o ${OBJECTDIR}/source/serial.o source/serial.c 
	
${OBJECTDIR}/source/vty.o: source/vty.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/vty.o.d 
	@${RM} ${OBJECTDIR}/source/vty.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/vty.o.d" -MT "${OBJECTDIR}/source/vty.o.d" -MT ${OBJECTDIR}/source/vty.o -o ${OBJECTDIR}/source/vty.o source/vty.c 
	
${OBJECTDIR}/_ext/1326544842/croutine.o: ../../../FreeRtosCore/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT ${OBJECTDIR}/_ext/1326544842/croutine.o -o ${OBJECTDIR}/_ext/1326544842/croutine.o ../../../FreeRtosCore/Source/croutine.c 
	
${OBJECTDIR}/_ext/1326544842/list.o: ../../../FreeRtosCore/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT ${OBJECTDIR}/_ext/1326544842/list.o -o ${OBJECTDIR}/_ext/1326544842/list.o ../../../FreeRtosCore/Source/list.c 
	
${OBJECTDIR}/_ext/1326544842/queue.o: ../../../FreeRtosCore/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT ${OBJECTDIR}/_ext/1326544842/queue.o -o ${OBJECTDIR}/_ext/1326544842/queue.o ../../../FreeRtosCore/Source/queue.c 
	
${OBJECTDIR}/_ext/1326544842/tasks.o: ../../../FreeRtosCore/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT ${OBJECTDIR}/_ext/1326544842/tasks.o -o ${OBJECTDIR}/_ext/1326544842/tasks.o ../../../FreeRtosCore/Source/tasks.c 
	
${OBJECTDIR}/_ext/178352208/port.o: ../../../FreeRtosCore/portable/GCC/ATMega64/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/178352208" 
	@${RM} ${OBJECTDIR}/_ext/178352208/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/178352208/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/178352208/port.o.d" -MT "${OBJECTDIR}/_ext/178352208/port.o.d" -MT ${OBJECTDIR}/_ext/178352208/port.o -o ${OBJECTDIR}/_ext/178352208/port.o ../../../FreeRtosCore/portable/GCC/ATMega64/port.c 
	
${OBJECTDIR}/_ext/1817164221/heap_avr.o: ../../../FreeRtosCore/portable/MemMang/heap_avr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1817164221" 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT ${OBJECTDIR}/_ext/1817164221/heap_avr.o -o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ../../../FreeRtosCore/portable/MemMang/heap_avr.c 
	
${OBJECTDIR}/_ext/1386490678/Rs485_prot.o: ../../../Lib/Rs485_prot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/Rs485_prot.o.d" -MT "${OBJECTDIR}/_ext/1386490678/Rs485_prot.o.d" -MT ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o -o ${OBJECTDIR}/_ext/1386490678/Rs485_prot.o ../../../Lib/Rs485_prot.c 
	
${OBJECTDIR}/_ext/1386490678/cmdline.o: ../../../Lib/cmdline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/cmdline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/cmdline.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/cmdline.o.d" -MT "${OBJECTDIR}/_ext/1386490678/cmdline.o.d" -MT ${OBJECTDIR}/_ext/1386490678/cmdline.o -o ${OBJECTDIR}/_ext/1386490678/cmdline.o ../../../Lib/cmdline.c 
	
${OBJECTDIR}/_ext/1386490678/ramdysk.o: ../../../Lib/ramdysk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/ramdysk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/ramdysk.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/ramdysk.o.d" -MT "${OBJECTDIR}/_ext/1386490678/ramdysk.o.d" -MT ${OBJECTDIR}/_ext/1386490678/ramdysk.o -o ${OBJECTDIR}/_ext/1386490678/ramdysk.o ../../../Lib/ramdysk.c 
	
${OBJECTDIR}/_ext/1386490678/xmodem.o: ../../../Lib/xmodem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/xmodem.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/xmodem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/xmodem.o.d" -MT "${OBJECTDIR}/_ext/1386490678/xmodem.o.d" -MT ${OBJECTDIR}/_ext/1386490678/xmodem.o -o ${OBJECTDIR}/_ext/1386490678/xmodem.o ../../../Lib/xmodem.c 
	
${OBJECTDIR}/_ext/1386490678/memory_x.o: ../../../Lib/memory_x.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/memory_x.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/memory_x.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/memory_x.o.d" -MT "${OBJECTDIR}/_ext/1386490678/memory_x.o.d" -MT ${OBJECTDIR}/_ext/1386490678/memory_x.o -o ${OBJECTDIR}/_ext/1386490678/memory_x.o ../../../Lib/memory_x.c 
	
${OBJECTDIR}/_ext/1386490678/mcp3008.o: ../../../Lib/mcp3008.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mcp3008.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mcp3008.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/mcp3008.o.d" -MT "${OBJECTDIR}/_ext/1386490678/mcp3008.o.d" -MT ${OBJECTDIR}/_ext/1386490678/mcp3008.o -o ${OBJECTDIR}/_ext/1386490678/mcp3008.o ../../../Lib/mcp3008.c 
	
${OBJECTDIR}/_ext/1386490678/enc28j60.o: ../../../Lib/enc28j60.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/enc28j60.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/enc28j60.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/enc28j60.o.d" -MT "${OBJECTDIR}/_ext/1386490678/enc28j60.o.d" -MT ${OBJECTDIR}/_ext/1386490678/enc28j60.o -o ${OBJECTDIR}/_ext/1386490678/enc28j60.o ../../../Lib/enc28j60.c 
	
${OBJECTDIR}/_ext/1386490678/mcp4150.o: ../../../Lib/mcp4150.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mcp4150.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mcp4150.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/mcp4150.o.d" -MT "${OBJECTDIR}/_ext/1386490678/mcp4150.o.d" -MT ${OBJECTDIR}/_ext/1386490678/mcp4150.o -o ${OBJECTDIR}/_ext/1386490678/mcp4150.o ../../../Lib/mcp4150.c 
	
${OBJECTDIR}/_ext/1386490678/mpc23s17.o: ../../../Lib/mpc23s17.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mpc23s17.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/mpc23s17.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/mpc23s17.o.d" -MT "${OBJECTDIR}/_ext/1386490678/mpc23s17.o.d" -MT ${OBJECTDIR}/_ext/1386490678/mpc23s17.o -o ${OBJECTDIR}/_ext/1386490678/mpc23s17.o ../../../Lib/mpc23s17.c 
	
${OBJECTDIR}/_ext/1386490678/spi.o: ../../../Lib/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/spi.o.d" -MT "${OBJECTDIR}/_ext/1386490678/spi.o.d" -MT ${OBJECTDIR}/_ext/1386490678/spi.o -o ${OBJECTDIR}/_ext/1386490678/spi.o ../../../Lib/spi.c 
	
${OBJECTDIR}/_ext/1046042812/arp.o: ../../../Lib/net/arp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/arp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/arp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/arp.o.d" -MT "${OBJECTDIR}/_ext/1046042812/arp.o.d" -MT ${OBJECTDIR}/_ext/1046042812/arp.o -o ${OBJECTDIR}/_ext/1046042812/arp.o ../../../Lib/net/arp.c 
	
${OBJECTDIR}/_ext/1046042812/icmp.o: ../../../Lib/net/icmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/icmp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/icmp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/icmp.o.d" -MT "${OBJECTDIR}/_ext/1046042812/icmp.o.d" -MT ${OBJECTDIR}/_ext/1046042812/icmp.o -o ${OBJECTDIR}/_ext/1046042812/icmp.o ../../../Lib/net/icmp.c 
	
${OBJECTDIR}/_ext/1046042812/ip.o: ../../../Lib/net/ip.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/ip.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/ip.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/ip.o.d" -MT "${OBJECTDIR}/_ext/1046042812/ip.o.d" -MT ${OBJECTDIR}/_ext/1046042812/ip.o -o ${OBJECTDIR}/_ext/1046042812/ip.o ../../../Lib/net/ip.c 
	
${OBJECTDIR}/_ext/1046042812/net.o: ../../../Lib/net/net.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/net.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/net.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/net.o.d" -MT "${OBJECTDIR}/_ext/1046042812/net.o.d" -MT ${OBJECTDIR}/_ext/1046042812/net.o -o ${OBJECTDIR}/_ext/1046042812/net.o ../../../Lib/net/net.c 
	
${OBJECTDIR}/_ext/1046042812/nic.o: ../../../Lib/net/nic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/nic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/nic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/nic.o.d" -MT "${OBJECTDIR}/_ext/1046042812/nic.o.d" -MT ${OBJECTDIR}/_ext/1046042812/nic.o -o ${OBJECTDIR}/_ext/1046042812/nic.o ../../../Lib/net/nic.c 
	
${OBJECTDIR}/_ext/1046042812/udp.o: ../../../Lib/net/udp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/udp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/udp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/udp.o.d" -MT "${OBJECTDIR}/_ext/1046042812/udp.o.d" -MT ${OBJECTDIR}/_ext/1046042812/udp.o -o ${OBJECTDIR}/_ext/1046042812/udp.o ../../../Lib/net/udp.c 
	
${OBJECTDIR}/_ext/1046042812/tcp.o: ../../../Lib/net/tcp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1046042812" 
	@${RM} ${OBJECTDIR}/_ext/1046042812/tcp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1046042812/tcp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1046042812/tcp.o.d" -MT "${OBJECTDIR}/_ext/1046042812/tcp.o.d" -MT ${OBJECTDIR}/_ext/1046042812/tcp.o -o ${OBJECTDIR}/_ext/1046042812/tcp.o ../../../Lib/net/tcp.c 
	
${OBJECTDIR}/_ext/1386490678/ds1305.o: ../../../Lib/ds1305.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/ds1305.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/ds1305.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/ds1305.o.d" -MT "${OBJECTDIR}/_ext/1386490678/ds1305.o.d" -MT ${OBJECTDIR}/_ext/1386490678/ds1305.o -o ${OBJECTDIR}/_ext/1386490678/ds1305.o ../../../Lib/ds1305.c 
	
${OBJECTDIR}/_ext/1386490678/queueStream.o: ../../../Lib/queueStream.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/queueStream.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/queueStream.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/queueStream.o.d" -MT "${OBJECTDIR}/_ext/1386490678/queueStream.o.d" -MT ${OBJECTDIR}/_ext/1386490678/queueStream.o -o ${OBJECTDIR}/_ext/1386490678/queueStream.o ../../../Lib/queueStream.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1     -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1    -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=14745600UL -I"source" -I"../Common" -I"../../../Lib" -I"../../../Lib/include" -I"../../../Lib/net" -I"../../../Lib/net/include" -I"../../../FreeRtosCore/Source" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/GCC/ATMega64" -I"../../../FreeRtosCore/portable/MemMang" -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}/avr-objcopy -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/MainController.X.${IMAGE_TYPE}.hex"
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
