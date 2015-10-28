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
//#include <p18cxxx.h>
#define BUFFSIZE 50

extern unsigned char GLOBAL_RXBUFFER[BUFFSIZE];

extern unsigned char GLOBAL_MODE; 
extern unsigned char GLOBAL_PID_GAIN;
extern unsigned char GLOBAL_MAX_SPEED;
extern unsigned char GLOBAL_MAX_YAW;
extern unsigned char GLOBAL_IRsampleE;
extern unsigned char GLOBAL_IRsampleR;
extern unsigned char GLOBAL_FAIL_COUNTER;
extern unsigned char GLOBAL_VELOCITY;
extern unsigned char GLOBAL_OMEGA;
extern unsigned char GLOBAL_RUN;
extern unsigned char GLOBAL_IRRAW;


#endif  /* GLOBALS_H */
