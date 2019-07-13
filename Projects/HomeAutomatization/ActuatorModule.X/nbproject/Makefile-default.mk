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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=source/hardware.c source/main.c source/serial.c source/stateMachine.c ../../../FreeRtosCore/portable/GCC/ATMega168/port.c ../../../FreeRtosCore/portable/MemMang/heap_avr.c ../../../FreeRtosCore/Source/tasks.c ../../../FreeRtosCore/Source/queue.c ../../../FreeRtosCore/Source/list.c ../../../FreeRtosCore/Source/croutine.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/source/hardware.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/serial.o ${OBJECTDIR}/source/stateMachine.o ${OBJECTDIR}/_ext/1233955839/port.o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ${OBJECTDIR}/_ext/1326544842/tasks.o ${OBJECTDIR}/_ext/1326544842/queue.o ${OBJECTDIR}/_ext/1326544842/list.o ${OBJECTDIR}/_ext/1326544842/croutine.o
POSSIBLE_DEPFILES=${OBJECTDIR}/source/hardware.o.d ${OBJECTDIR}/source/main.o.d ${OBJECTDIR}/source/serial.o.d ${OBJECTDIR}/source/stateMachine.o.d ${OBJECTDIR}/_ext/1233955839/port.o.d ${OBJECTDIR}/_ext/1817164221/heap_avr.o.d ${OBJECTDIR}/_ext/1326544842/tasks.o.d ${OBJECTDIR}/_ext/1326544842/queue.o.d ${OBJECTDIR}/_ext/1326544842/list.o.d ${OBJECTDIR}/_ext/1326544842/croutine.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/source/hardware.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/serial.o ${OBJECTDIR}/source/stateMachine.o ${OBJECTDIR}/_ext/1233955839/port.o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ${OBJECTDIR}/_ext/1326544842/tasks.o ${OBJECTDIR}/_ext/1326544842/queue.o ${OBJECTDIR}/_ext/1326544842/list.o ${OBJECTDIR}/_ext/1326544842/croutine.o

# Source Files
SOURCEFILES=source/hardware.c source/main.c source/serial.c source/stateMachine.c ../../../FreeRtosCore/portable/GCC/ATMega168/port.c ../../../FreeRtosCore/portable/MemMang/heap_avr.c ../../../FreeRtosCore/Source/tasks.c ../../../FreeRtosCore/Source/queue.c ../../../FreeRtosCore/Source/list.c ../../../FreeRtosCore/Source/croutine.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATmega168
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/source/hardware.o: source/hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/hardware.o.d 
	@${RM} ${OBJECTDIR}/source/hardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/hardware.o.d" -MT "${OBJECTDIR}/source/hardware.o.d" -MT ${OBJECTDIR}/source/hardware.o -o ${OBJECTDIR}/source/hardware.o source/hardware.c 
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/main.o.d" -MT "${OBJECTDIR}/source/main.o.d" -MT ${OBJECTDIR}/source/main.o -o ${OBJECTDIR}/source/main.o source/main.c 
	
${OBJECTDIR}/source/serial.o: source/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/serial.o.d 
	@${RM} ${OBJECTDIR}/source/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/serial.o.d" -MT "${OBJECTDIR}/source/serial.o.d" -MT ${OBJECTDIR}/source/serial.o -o ${OBJECTDIR}/source/serial.o source/serial.c 
	
${OBJECTDIR}/source/stateMachine.o: source/stateMachine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/stateMachine.o.d 
	@${RM} ${OBJECTDIR}/source/stateMachine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/stateMachine.o.d" -MT "${OBJECTDIR}/source/stateMachine.o.d" -MT ${OBJECTDIR}/source/stateMachine.o -o ${OBJECTDIR}/source/stateMachine.o source/stateMachine.c 
	
${OBJECTDIR}/_ext/1233955839/port.o: ../../../FreeRtosCore/portable/GCC/ATMega168/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1233955839" 
	@${RM} ${OBJECTDIR}/_ext/1233955839/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1233955839/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1233955839/port.o.d" -MT "${OBJECTDIR}/_ext/1233955839/port.o.d" -MT ${OBJECTDIR}/_ext/1233955839/port.o -o ${OBJECTDIR}/_ext/1233955839/port.o ../../../FreeRtosCore/portable/GCC/ATMega168/port.c 
	
${OBJECTDIR}/_ext/1817164221/heap_avr.o: ../../../FreeRtosCore/portable/MemMang/heap_avr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1817164221" 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT ${OBJECTDIR}/_ext/1817164221/heap_avr.o -o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ../../../FreeRtosCore/portable/MemMang/heap_avr.c 
	
${OBJECTDIR}/_ext/1326544842/tasks.o: ../../../FreeRtosCore/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT ${OBJECTDIR}/_ext/1326544842/tasks.o -o ${OBJECTDIR}/_ext/1326544842/tasks.o ../../../FreeRtosCore/Source/tasks.c 
	
${OBJECTDIR}/_ext/1326544842/queue.o: ../../../FreeRtosCore/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT ${OBJECTDIR}/_ext/1326544842/queue.o -o ${OBJECTDIR}/_ext/1326544842/queue.o ../../../FreeRtosCore/Source/queue.c 
	
${OBJECTDIR}/_ext/1326544842/list.o: ../../../FreeRtosCore/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT ${OBJECTDIR}/_ext/1326544842/list.o -o ${OBJECTDIR}/_ext/1326544842/list.o ../../../FreeRtosCore/Source/list.c 
	
${OBJECTDIR}/_ext/1326544842/croutine.o: ../../../FreeRtosCore/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT ${OBJECTDIR}/_ext/1326544842/croutine.o -o ${OBJECTDIR}/_ext/1326544842/croutine.o ../../../FreeRtosCore/Source/croutine.c 
	
else
${OBJECTDIR}/source/hardware.o: source/hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/hardware.o.d 
	@${RM} ${OBJECTDIR}/source/hardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/hardware.o.d" -MT "${OBJECTDIR}/source/hardware.o.d" -MT ${OBJECTDIR}/source/hardware.o -o ${OBJECTDIR}/source/hardware.o source/hardware.c 
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/main.o.d" -MT "${OBJECTDIR}/source/main.o.d" -MT ${OBJECTDIR}/source/main.o -o ${OBJECTDIR}/source/main.o source/main.c 
	
${OBJECTDIR}/source/serial.o: source/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/serial.o.d 
	@${RM} ${OBJECTDIR}/source/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/serial.o.d" -MT "${OBJECTDIR}/source/serial.o.d" -MT ${OBJECTDIR}/source/serial.o -o ${OBJECTDIR}/source/serial.o source/serial.c 
	
${OBJECTDIR}/source/stateMachine.o: source/stateMachine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/source" 
	@${RM} ${OBJECTDIR}/source/stateMachine.o.d 
	@${RM} ${OBJECTDIR}/source/stateMachine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/source/stateMachine.o.d" -MT "${OBJECTDIR}/source/stateMachine.o.d" -MT ${OBJECTDIR}/source/stateMachine.o -o ${OBJECTDIR}/source/stateMachine.o source/stateMachine.c 
	
${OBJECTDIR}/_ext/1233955839/port.o: ../../../FreeRtosCore/portable/GCC/ATMega168/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1233955839" 
	@${RM} ${OBJECTDIR}/_ext/1233955839/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1233955839/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1233955839/port.o.d" -MT "${OBJECTDIR}/_ext/1233955839/port.o.d" -MT ${OBJECTDIR}/_ext/1233955839/port.o -o ${OBJECTDIR}/_ext/1233955839/port.o ../../../FreeRtosCore/portable/GCC/ATMega168/port.c 
	
${OBJECTDIR}/_ext/1817164221/heap_avr.o: ../../../FreeRtosCore/portable/MemMang/heap_avr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1817164221" 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT ${OBJECTDIR}/_ext/1817164221/heap_avr.o -o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ../../../FreeRtosCore/portable/MemMang/heap_avr.c 
	
${OBJECTDIR}/_ext/1326544842/tasks.o: ../../../FreeRtosCore/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT ${OBJECTDIR}/_ext/1326544842/tasks.o -o ${OBJECTDIR}/_ext/1326544842/tasks.o ../../../FreeRtosCore/Source/tasks.c 
	
${OBJECTDIR}/_ext/1326544842/queue.o: ../../../FreeRtosCore/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT ${OBJECTDIR}/_ext/1326544842/queue.o -o ${OBJECTDIR}/_ext/1326544842/queue.o ../../../FreeRtosCore/Source/queue.c 
	
${OBJECTDIR}/_ext/1326544842/list.o: ../../../FreeRtosCore/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT ${OBJECTDIR}/_ext/1326544842/list.o -o ${OBJECTDIR}/_ext/1326544842/list.o ../../../FreeRtosCore/Source/list.c 
	
${OBJECTDIR}/_ext/1326544842/croutine.o: ../../../FreeRtosCore/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT ${OBJECTDIR}/_ext/1326544842/croutine.o -o ${OBJECTDIR}/_ext/1326544842/croutine.o ../../../FreeRtosCore/Source/croutine.c 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1     -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1    -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -I"source" -I"../Common" -I"../../../FreeRtosCore/Source/include" -I"../../../FreeRtosCore/portable/MemMang" -I"../../../FreeRtosCore/portable/GCC/ATMega168" -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}/avr-objcopy -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/ActuatorModule.X.${IMAGE_TYPE}.hex"
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
