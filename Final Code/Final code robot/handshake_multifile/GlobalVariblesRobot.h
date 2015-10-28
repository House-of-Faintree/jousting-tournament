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
 
#ifndef GLOBALS_H
#define GLOBALS_H
 
/* Chip Configurations */
#include <p18cxxx.h>
#define BUFFSIZE 50

extern char GLOBAL_RXBUFFER[BUFFSIZE];

extern char GLOBAL_MODE; 
extern char GLOBAL_PID_GAIN;
extern char GLOBAL_MAX_SPEED;
extern char GLOBAL_MAX_YAW;
extern char GLOBAL_IRsampleE;
extern char GLOBAL_IRsampleR;
extern char GLOBAL_FAIL_COUNTER;


#endif  /* GLOBALS_H */
