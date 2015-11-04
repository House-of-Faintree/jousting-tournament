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
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/IR_minimal_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/IR_minimal_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=IR_test_main.c Interrupt_defination_Robot.c PWM_Motor.c serialRobot.c GlobalVariblesRobot.c AD.c IR_Sample.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/IR_test_main.o ${OBJECTDIR}/Interrupt_defination_Robot.o ${OBJECTDIR}/PWM_Motor.o ${OBJECTDIR}/serialRobot.o ${OBJECTDIR}/GlobalVariblesRobot.o ${OBJECTDIR}/AD.o ${OBJECTDIR}/IR_Sample.o
POSSIBLE_DEPFILES=${OBJECTDIR}/IR_test_main.o.d ${OBJECTDIR}/Interrupt_defination_Robot.o.d ${OBJECTDIR}/PWM_Motor.o.d ${OBJECTDIR}/serialRobot.o.d ${OBJECTDIR}/GlobalVariblesRobot.o.d ${OBJECTDIR}/AD.o.d ${OBJECTDIR}/IR_Sample.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/IR_test_main.o ${OBJECTDIR}/Interrupt_defination_Robot.o ${OBJECTDIR}/PWM_Motor.o ${OBJECTDIR}/serialRobot.o ${OBJECTDIR}/GlobalVariblesRobot.o ${OBJECTDIR}/AD.o ${OBJECTDIR}/IR_Sample.o

# Source Files
SOURCEFILES=IR_test_main.c Interrupt_defination_Robot.c PWM_Motor.c serialRobot.c GlobalVariblesRobot.c AD.c IR_Sample.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/IR_minimal_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F4520
MP_PROCESSOR_OPTION_LD=18f4520
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0x7dc0 -u_DEBUGCODELEN=0x240 -u_DEBUGDATASTART=0x5f4 -u_DEBUGDATALEN=0xb
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/IR_test_main.o: IR_test_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IR_test_main.o.d 
	@${RM} ${OBJECTDIR}/IR_test_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/IR_test_main.o   IR_test_main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/IR_test_main.o 
	@${FIXDEPS} "${OBJECTDIR}/IR_test_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/Interrupt_defination_Robot.o: Interrupt_defination_Robot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Interrupt_defination_Robot.o.d 
	@${RM} ${OBJECTDIR}/Interrupt_defination_Robot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/Interrupt_defination_Robot.o   Interrupt_defination_Robot.c 
	@${DEP_GEN} -d ${OBJECTDIR}/Interrupt_defination_Robot.o 
	@${FIXDEPS} "${OBJECTDIR}/Interrupt_defination_Robot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/PWM_Motor.o: PWM_Motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PWM_Motor.o.d 
	@${RM} ${OBJECTDIR}/PWM_Motor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/PWM_Motor.o   PWM_Motor.c 
	@${DEP_GEN} -d ${OBJECTDIR}/PWM_Motor.o 
	@${FIXDEPS} "${OBJECTDIR}/PWM_Motor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/serialRobot.o: serialRobot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/serialRobot.o.d 
	@${RM} ${OBJECTDIR}/serialRobot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/serialRobot.o   serialRobot.c 
	@${DEP_GEN} -d ${OBJECTDIR}/serialRobot.o 
	@${FIXDEPS} "${OBJECTDIR}/serialRobot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/GlobalVariblesRobot.o: GlobalVariblesRobot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GlobalVariblesRobot.o.d 
	@${RM} ${OBJECTDIR}/GlobalVariblesRobot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/GlobalVariblesRobot.o   GlobalVariblesRobot.c 
	@${DEP_GEN} -d ${OBJECTDIR}/GlobalVariblesRobot.o 
	@${FIXDEPS} "${OBJECTDIR}/GlobalVariblesRobot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/AD.o: AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AD.o.d 
	@${RM} ${OBJECTDIR}/AD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/AD.o   AD.c 
	@${DEP_GEN} -d ${OBJECTDIR}/AD.o 
	@${FIXDEPS} "${OBJECTDIR}/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/IR_Sample.o: IR_Sample.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IR_Sample.o.d 
	@${RM} ${OBJECTDIR}/IR_Sample.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/IR_Sample.o   IR_Sample.c 
	@${DEP_GEN} -d ${OBJECTDIR}/IR_Sample.o 
	@${FIXDEPS} "${OBJECTDIR}/IR_Sample.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/IR_test_main.o: IR_test_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IR_test_main.o.d 
	@${RM} ${OBJECTDIR}/IR_test_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/IR_test_main.o   IR_test_main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/IR_test_main.o 
	@${FIXDEPS} "${OBJECTDIR}/IR_test_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/Interrupt_defination_Robot.o: Interrupt_defination_Robot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Interrupt_defination_Robot.o.d 
	@${RM} ${OBJECTDIR}/Interrupt_defination_Robot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/Interrupt_defination_Robot.o   Interrupt_defination_Robot.c 
	@${DEP_GEN} -d ${OBJECTDIR}/Interrupt_defination_Robot.o 
	@${FIXDEPS} "${OBJECTDIR}/Interrupt_defination_Robot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/PWM_Motor.o: PWM_Motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PWM_Motor.o.d 
	@${RM} ${OBJECTDIR}/PWM_Motor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/PWM_Motor.o   PWM_Motor.c 
	@${DEP_GEN} -d ${OBJECTDIR}/PWM_Motor.o 
	@${FIXDEPS} "${OBJECTDIR}/PWM_Motor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/serialRobot.o: serialRobot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/serialRobot.o.d 
	@${RM} ${OBJECTDIR}/serialRobot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/serialRobot.o   serialRobot.c 
	@${DEP_GEN} -d ${OBJECTDIR}/serialRobot.o 
	@${FIXDEPS} "${OBJECTDIR}/serialRobot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/GlobalVariblesRobot.o: GlobalVariblesRobot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GlobalVariblesRobot.o.d 
	@${RM} ${OBJECTDIR}/GlobalVariblesRobot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/GlobalVariblesRobot.o   GlobalVariblesRobot.c 
	@${DEP_GEN} -d ${OBJECTDIR}/GlobalVariblesRobot.o 
	@${FIXDEPS} "${OBJECTDIR}/GlobalVariblesRobot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/AD.o: AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AD.o.d 
	@${RM} ${OBJECTDIR}/AD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/AD.o   AD.c 
	@${DEP_GEN} -d ${OBJECTDIR}/AD.o 
	@${FIXDEPS} "${OBJECTDIR}/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/IR_Sample.o: IR_Sample.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IR_Sample.o.d 
	@${RM} ${OBJECTDIR}/IR_Sample.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/IR_Sample.o   IR_Sample.c 
	@${DEP_GEN} -d ${OBJECTDIR}/IR_Sample.o 
	@${FIXDEPS} "${OBJECTDIR}/IR_Sample.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/IR_minimal_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_ICD3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/IR_minimal_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/IR_minimal_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/IR_minimal_test.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
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

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
