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

char GLOBAL_RXBUFFER[BUFFSIZE] = "0";

char GLOBAL_MODE = 0; 
char GLOBAL_PID_GAIN = 100;
char GLOBAL_MAX_SPEED = 100;
char GLOBAL_MAX_YAW =10 ;
char GLOBAL_IRsampleE = 20;
char GLOBAL_IRsampleR = 10;
char GLOBAL_FAIL_COUNTER = 0;

