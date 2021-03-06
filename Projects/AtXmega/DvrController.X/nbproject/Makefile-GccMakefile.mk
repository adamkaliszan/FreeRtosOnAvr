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
ifeq "$(wildcard nbproject/Makefile-local-GccMakefile.mk)" "nbproject/Makefile-local-GccMakefile.mk"
include nbproject/Makefile-local-GccMakefile.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=GccMakefile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../../../FreeRtosCore/Source/croutine.c ../../../FreeRtosCore/Source/list.c ../../../FreeRtosCore/Source/queue.c ../../../FreeRtosCore/Source/tasks.c ../../../Lib/tlvProt.c ../../../LibRtos/sim900.c ../../../FreeRtosCore/portable/MemMang/heap_avr.c ../../../FreeRtosCore/portable/GCC/ATXmega/port.c cli_task.c configuration.c hardware.c main.c main_task.c serial.c tlv.c tlv_task.c vty.c ../../../Lib/cmdline.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1326544842/croutine.o ${OBJECTDIR}/_ext/1326544842/list.o ${OBJECTDIR}/_ext/1326544842/queue.o ${OBJECTDIR}/_ext/1326544842/tasks.o ${OBJECTDIR}/_ext/1386490678/tlvProt.o ${OBJECTDIR}/_ext/1044994052/sim900.o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ${OBJECTDIR}/_ext/1380831254/port.o ${OBJECTDIR}/cli_task.o ${OBJECTDIR}/configuration.o ${OBJECTDIR}/hardware.o ${OBJECTDIR}/main.o ${OBJECTDIR}/main_task.o ${OBJECTDIR}/serial.o ${OBJECTDIR}/tlv.o ${OBJECTDIR}/tlv_task.o ${OBJECTDIR}/vty.o ${OBJECTDIR}/_ext/1386490678/cmdline.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1326544842/croutine.o.d ${OBJECTDIR}/_ext/1326544842/list.o.d ${OBJECTDIR}/_ext/1326544842/queue.o.d ${OBJECTDIR}/_ext/1326544842/tasks.o.d ${OBJECTDIR}/_ext/1386490678/tlvProt.o.d ${OBJECTDIR}/_ext/1044994052/sim900.o.d ${OBJECTDIR}/_ext/1817164221/heap_avr.o.d ${OBJECTDIR}/_ext/1380831254/port.o.d ${OBJECTDIR}/cli_task.o.d ${OBJECTDIR}/configuration.o.d ${OBJECTDIR}/hardware.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/main_task.o.d ${OBJECTDIR}/serial.o.d ${OBJECTDIR}/tlv.o.d ${OBJECTDIR}/tlv_task.o.d ${OBJECTDIR}/vty.o.d ${OBJECTDIR}/_ext/1386490678/cmdline.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1326544842/croutine.o ${OBJECTDIR}/_ext/1326544842/list.o ${OBJECTDIR}/_ext/1326544842/queue.o ${OBJECTDIR}/_ext/1326544842/tasks.o ${OBJECTDIR}/_ext/1386490678/tlvProt.o ${OBJECTDIR}/_ext/1044994052/sim900.o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ${OBJECTDIR}/_ext/1380831254/port.o ${OBJECTDIR}/cli_task.o ${OBJECTDIR}/configuration.o ${OBJECTDIR}/hardware.o ${OBJECTDIR}/main.o ${OBJECTDIR}/main_task.o ${OBJECTDIR}/serial.o ${OBJECTDIR}/tlv.o ${OBJECTDIR}/tlv_task.o ${OBJECTDIR}/vty.o ${OBJECTDIR}/_ext/1386490678/cmdline.o

# Source Files
SOURCEFILES=../../../FreeRtosCore/Source/croutine.c ../../../FreeRtosCore/Source/list.c ../../../FreeRtosCore/Source/queue.c ../../../FreeRtosCore/Source/tasks.c ../../../Lib/tlvProt.c ../../../LibRtos/sim900.c ../../../FreeRtosCore/portable/MemMang/heap_avr.c ../../../FreeRtosCore/portable/GCC/ATXmega/port.c cli_task.c configuration.c hardware.c main.c main_task.c serial.c tlv.c tlv_task.c vty.c ../../../Lib/cmdline.c


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

# The following macros may be used in the pre and post step lines
Device=ATxmega128A4U
ProjectDir=/home/adam/githab/FreeRtosOnAvr/Projects/AtXmega/DvrController.X
ConfName=GccMakefile
ImagePath=dist/GccMakefile/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
ImageDir=dist/GccMakefile/${IMAGE_TYPE}
ImageName=DvrController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif

.build-conf:  .pre ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-GccMakefile.mk dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: [make -f MakefileGcc]"
	@make -f MakefileGcc
	@echo "--------------------------------------"

MP_PROCESSOR_OPTION=ATxmega128A4U
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1326544842/croutine.o: ../../../FreeRtosCore/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT ${OBJECTDIR}/_ext/1326544842/croutine.o -o ${OBJECTDIR}/_ext/1326544842/croutine.o ../../../FreeRtosCore/Source/croutine.c 
	
${OBJECTDIR}/_ext/1326544842/list.o: ../../../FreeRtosCore/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT ${OBJECTDIR}/_ext/1326544842/list.o -o ${OBJECTDIR}/_ext/1326544842/list.o ../../../FreeRtosCore/Source/list.c 
	
${OBJECTDIR}/_ext/1326544842/queue.o: ../../../FreeRtosCore/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT ${OBJECTDIR}/_ext/1326544842/queue.o -o ${OBJECTDIR}/_ext/1326544842/queue.o ../../../FreeRtosCore/Source/queue.c 
	
${OBJECTDIR}/_ext/1326544842/tasks.o: ../../../FreeRtosCore/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT ${OBJECTDIR}/_ext/1326544842/tasks.o -o ${OBJECTDIR}/_ext/1326544842/tasks.o ../../../FreeRtosCore/Source/tasks.c 
	
${OBJECTDIR}/_ext/1386490678/tlvProt.o: ../../../Lib/tlvProt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/tlvProt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/tlvProt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/tlvProt.o.d" -MT "${OBJECTDIR}/_ext/1386490678/tlvProt.o.d" -MT ${OBJECTDIR}/_ext/1386490678/tlvProt.o -o ${OBJECTDIR}/_ext/1386490678/tlvProt.o ../../../Lib/tlvProt.c 
	
${OBJECTDIR}/_ext/1044994052/sim900.o: ../../../LibRtos/sim900.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1044994052" 
	@${RM} ${OBJECTDIR}/_ext/1044994052/sim900.o.d 
	@${RM} ${OBJECTDIR}/_ext/1044994052/sim900.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1044994052/sim900.o.d" -MT "${OBJECTDIR}/_ext/1044994052/sim900.o.d" -MT ${OBJECTDIR}/_ext/1044994052/sim900.o -o ${OBJECTDIR}/_ext/1044994052/sim900.o ../../../LibRtos/sim900.c 
	
${OBJECTDIR}/_ext/1817164221/heap_avr.o: ../../../FreeRtosCore/portable/MemMang/heap_avr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1817164221" 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT ${OBJECTDIR}/_ext/1817164221/heap_avr.o -o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ../../../FreeRtosCore/portable/MemMang/heap_avr.c 
	
${OBJECTDIR}/_ext/1380831254/port.o: ../../../FreeRtosCore/portable/GCC/ATXmega/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1380831254" 
	@${RM} ${OBJECTDIR}/_ext/1380831254/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1380831254/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1380831254/port.o.d" -MT "${OBJECTDIR}/_ext/1380831254/port.o.d" -MT ${OBJECTDIR}/_ext/1380831254/port.o -o ${OBJECTDIR}/_ext/1380831254/port.o ../../../FreeRtosCore/portable/GCC/ATXmega/port.c 
	
${OBJECTDIR}/cli_task.o: cli_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cli_task.o.d 
	@${RM} ${OBJECTDIR}/cli_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/cli_task.o.d" -MT "${OBJECTDIR}/cli_task.o.d" -MT ${OBJECTDIR}/cli_task.o -o ${OBJECTDIR}/cli_task.o cli_task.c 
	
${OBJECTDIR}/configuration.o: configuration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/configuration.o.d 
	@${RM} ${OBJECTDIR}/configuration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/configuration.o.d" -MT "${OBJECTDIR}/configuration.o.d" -MT ${OBJECTDIR}/configuration.o -o ${OBJECTDIR}/configuration.o configuration.c 
	
${OBJECTDIR}/hardware.o: hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hardware.o.d 
	@${RM} ${OBJECTDIR}/hardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/hardware.o.d" -MT "${OBJECTDIR}/hardware.o.d" -MT ${OBJECTDIR}/hardware.o -o ${OBJECTDIR}/hardware.o hardware.c 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
${OBJECTDIR}/main_task.o: main_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main_task.o.d 
	@${RM} ${OBJECTDIR}/main_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/main_task.o.d" -MT "${OBJECTDIR}/main_task.o.d" -MT ${OBJECTDIR}/main_task.o -o ${OBJECTDIR}/main_task.o main_task.c 
	
${OBJECTDIR}/serial.o: serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/serial.o.d 
	@${RM} ${OBJECTDIR}/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/serial.o.d" -MT "${OBJECTDIR}/serial.o.d" -MT ${OBJECTDIR}/serial.o -o ${OBJECTDIR}/serial.o serial.c 
	
${OBJECTDIR}/tlv.o: tlv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tlv.o.d 
	@${RM} ${OBJECTDIR}/tlv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/tlv.o.d" -MT "${OBJECTDIR}/tlv.o.d" -MT ${OBJECTDIR}/tlv.o -o ${OBJECTDIR}/tlv.o tlv.c 
	
${OBJECTDIR}/tlv_task.o: tlv_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tlv_task.o.d 
	@${RM} ${OBJECTDIR}/tlv_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/tlv_task.o.d" -MT "${OBJECTDIR}/tlv_task.o.d" -MT ${OBJECTDIR}/tlv_task.o -o ${OBJECTDIR}/tlv_task.o tlv_task.c 
	
${OBJECTDIR}/vty.o: vty.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/vty.o.d 
	@${RM} ${OBJECTDIR}/vty.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/vty.o.d" -MT "${OBJECTDIR}/vty.o.d" -MT ${OBJECTDIR}/vty.o -o ${OBJECTDIR}/vty.o vty.c 
	
${OBJECTDIR}/_ext/1386490678/cmdline.o: ../../../Lib/cmdline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/cmdline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/cmdline.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/cmdline.o.d" -MT "${OBJECTDIR}/_ext/1386490678/cmdline.o.d" -MT ${OBJECTDIR}/_ext/1386490678/cmdline.o -o ${OBJECTDIR}/_ext/1386490678/cmdline.o ../../../Lib/cmdline.c 
	
else
${OBJECTDIR}/_ext/1326544842/croutine.o: ../../../FreeRtosCore/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT "${OBJECTDIR}/_ext/1326544842/croutine.o.d" -MT ${OBJECTDIR}/_ext/1326544842/croutine.o -o ${OBJECTDIR}/_ext/1326544842/croutine.o ../../../FreeRtosCore/Source/croutine.c 
	
${OBJECTDIR}/_ext/1326544842/list.o: ../../../FreeRtosCore/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT "${OBJECTDIR}/_ext/1326544842/list.o.d" -MT ${OBJECTDIR}/_ext/1326544842/list.o -o ${OBJECTDIR}/_ext/1326544842/list.o ../../../FreeRtosCore/Source/list.c 
	
${OBJECTDIR}/_ext/1326544842/queue.o: ../../../FreeRtosCore/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT "${OBJECTDIR}/_ext/1326544842/queue.o.d" -MT ${OBJECTDIR}/_ext/1326544842/queue.o -o ${OBJECTDIR}/_ext/1326544842/queue.o ../../../FreeRtosCore/Source/queue.c 
	
${OBJECTDIR}/_ext/1326544842/tasks.o: ../../../FreeRtosCore/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1326544842" 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1326544842/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT "${OBJECTDIR}/_ext/1326544842/tasks.o.d" -MT ${OBJECTDIR}/_ext/1326544842/tasks.o -o ${OBJECTDIR}/_ext/1326544842/tasks.o ../../../FreeRtosCore/Source/tasks.c 
	
${OBJECTDIR}/_ext/1386490678/tlvProt.o: ../../../Lib/tlvProt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/tlvProt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/tlvProt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/tlvProt.o.d" -MT "${OBJECTDIR}/_ext/1386490678/tlvProt.o.d" -MT ${OBJECTDIR}/_ext/1386490678/tlvProt.o -o ${OBJECTDIR}/_ext/1386490678/tlvProt.o ../../../Lib/tlvProt.c 
	
${OBJECTDIR}/_ext/1044994052/sim900.o: ../../../LibRtos/sim900.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1044994052" 
	@${RM} ${OBJECTDIR}/_ext/1044994052/sim900.o.d 
	@${RM} ${OBJECTDIR}/_ext/1044994052/sim900.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1044994052/sim900.o.d" -MT "${OBJECTDIR}/_ext/1044994052/sim900.o.d" -MT ${OBJECTDIR}/_ext/1044994052/sim900.o -o ${OBJECTDIR}/_ext/1044994052/sim900.o ../../../LibRtos/sim900.c 
	
${OBJECTDIR}/_ext/1817164221/heap_avr.o: ../../../FreeRtosCore/portable/MemMang/heap_avr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1817164221" 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1817164221/heap_avr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT "${OBJECTDIR}/_ext/1817164221/heap_avr.o.d" -MT ${OBJECTDIR}/_ext/1817164221/heap_avr.o -o ${OBJECTDIR}/_ext/1817164221/heap_avr.o ../../../FreeRtosCore/portable/MemMang/heap_avr.c 
	
${OBJECTDIR}/_ext/1380831254/port.o: ../../../FreeRtosCore/portable/GCC/ATXmega/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1380831254" 
	@${RM} ${OBJECTDIR}/_ext/1380831254/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1380831254/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1380831254/port.o.d" -MT "${OBJECTDIR}/_ext/1380831254/port.o.d" -MT ${OBJECTDIR}/_ext/1380831254/port.o -o ${OBJECTDIR}/_ext/1380831254/port.o ../../../FreeRtosCore/portable/GCC/ATXmega/port.c 
	
${OBJECTDIR}/cli_task.o: cli_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cli_task.o.d 
	@${RM} ${OBJECTDIR}/cli_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/cli_task.o.d" -MT "${OBJECTDIR}/cli_task.o.d" -MT ${OBJECTDIR}/cli_task.o -o ${OBJECTDIR}/cli_task.o cli_task.c 
	
${OBJECTDIR}/configuration.o: configuration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/configuration.o.d 
	@${RM} ${OBJECTDIR}/configuration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/configuration.o.d" -MT "${OBJECTDIR}/configuration.o.d" -MT ${OBJECTDIR}/configuration.o -o ${OBJECTDIR}/configuration.o configuration.c 
	
${OBJECTDIR}/hardware.o: hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hardware.o.d 
	@${RM} ${OBJECTDIR}/hardware.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/hardware.o.d" -MT "${OBJECTDIR}/hardware.o.d" -MT ${OBJECTDIR}/hardware.o -o ${OBJECTDIR}/hardware.o hardware.c 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
${OBJECTDIR}/main_task.o: main_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main_task.o.d 
	@${RM} ${OBJECTDIR}/main_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/main_task.o.d" -MT "${OBJECTDIR}/main_task.o.d" -MT ${OBJECTDIR}/main_task.o -o ${OBJECTDIR}/main_task.o main_task.c 
	
${OBJECTDIR}/serial.o: serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/serial.o.d 
	@${RM} ${OBJECTDIR}/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/serial.o.d" -MT "${OBJECTDIR}/serial.o.d" -MT ${OBJECTDIR}/serial.o -o ${OBJECTDIR}/serial.o serial.c 
	
${OBJECTDIR}/tlv.o: tlv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tlv.o.d 
	@${RM} ${OBJECTDIR}/tlv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/tlv.o.d" -MT "${OBJECTDIR}/tlv.o.d" -MT ${OBJECTDIR}/tlv.o -o ${OBJECTDIR}/tlv.o tlv.c 
	
${OBJECTDIR}/tlv_task.o: tlv_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tlv_task.o.d 
	@${RM} ${OBJECTDIR}/tlv_task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/tlv_task.o.d" -MT "${OBJECTDIR}/tlv_task.o.d" -MT ${OBJECTDIR}/tlv_task.o -o ${OBJECTDIR}/tlv_task.o tlv_task.c 
	
${OBJECTDIR}/vty.o: vty.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/vty.o.d 
	@${RM} ${OBJECTDIR}/vty.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/vty.o.d" -MT "${OBJECTDIR}/vty.o.d" -MT ${OBJECTDIR}/vty.o -o ${OBJECTDIR}/vty.o vty.c 
	
${OBJECTDIR}/_ext/1386490678/cmdline.o: ../../../Lib/cmdline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1386490678" 
	@${RM} ${OBJECTDIR}/_ext/1386490678/cmdline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386490678/cmdline.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__  -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_GccMakefile=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/_ext/1386490678/cmdline.o.d" -MT "${OBJECTDIR}/_ext/1386490678/cmdline.o.d" -MT ${OBJECTDIR}/_ext/1386490678/cmdline.o -o ${OBJECTDIR}/_ext/1386490678/cmdline.o ../../../Lib/cmdline.c 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_GccMakefile=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"   -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.map  -DXPRJ_GccMakefile=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="/opt/microchip/mplabx/v5.20/packs/Microchip/XMEGAA_DFP/2.0.6"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}/avr-objcopy -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/DvrController.X.${IMAGE_TYPE}.hex"
endif

.pre:
	@echo "--------------------------------------"
	@echo "User defined pre-build step: [make -f MakefileGcc clean]"
	@make -f MakefileGcc clean
	@echo "--------------------------------------"

# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/GccMakefile
	${RM} -r dist/GccMakefile

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
