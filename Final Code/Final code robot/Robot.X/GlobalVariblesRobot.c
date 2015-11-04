/*******************************************************************************
 * MODULE:  GLOBAL DEFINITIONS
 *..............................................................................
 *  Created By:         ZIJI AN
 *  Created Date:       22.10.2015
 *..............................................................................
 *  Last Edit Date:     22.10.2015
 *  Last Edit By:       ZIJI
 *  Last Edit Details:  Created this file for global varibles on robot
 *..............................................................................
 * File/Function Property/Usage:
 *  See description below.
 *..............................................................................
 * Description:
 *  This file contains the definition of all global variables that are
 *  available to everyone and every module. It contains all globaly important
 *  things.
 *
 * Dependancies:
 *  This is the first file to be #included in the main code so it should not
 *  be allowed to have any dependancies.
 *..............................................................................
 * Current Issues:
*******************************************************************************/

#include "GlobalVariblesRobot.h"

unsigned char GLOBAL_RXBUFFER[BUFFSIZE] = "0";

unsigned char GLOBAL_MODE = 3; 
unsigned char GLOBAL_PID_GAIN = 100;
unsigned char GLOBAL_MAX_SPEED = 100;
unsigned char GLOBAL_MAX_YAW =10 ;
unsigned char GLOBAL_IRsampleE = 2;
unsigned char GLOBAL_IRsampleR = 1;
unsigned char GLOBAL_FAIL_COUNTER = 0;
unsigned char GLOBAL_VELOCITY = 128; // 'velocity' associated with 50% duty cycle
unsigned char GLOBAL_OMEGA  = 128;  // 0 turn to start with
unsigned char GLOBAL_RUN = 1;

unsigned char GLOBAL_AUTO_DISTANCE  = 15;
unsigned char GLOBAL_IRRAW = 0;

