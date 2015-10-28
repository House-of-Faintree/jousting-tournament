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

unsigned extern char GLOBAL_RXBUFFER[BUFFSIZE];

unsigned extern char GLOBAL_MODE; 
unsigned extern char GLOBAL_PID_GAIN;
unsigned extern char GLOBAL_MAX_SPEED;  //between 0 and 100
unsigned extern char GLOBAL_MAX_YAW;
unsigned extern char GLOBAL_IRsampleE;
unsigned extern char GLOBAL_IRsampleR;
unsigned extern char GLOBAL_FAIL_COUNTER;   //
unsigned extern char GLOBAL_VELOCITY;  // y value from controller joystick
unsigned extern char GLOBAL_OMEGA;  // x value from controller joystick
unsigned extern char GLOBAL_RUN; // motor on flag from controller

unsigned extern char GLOBAL_AUTO_DISTANCE;
unsigned extern char GLOBAL_IRRAW;

#endif  /* GLOBALS_H */
