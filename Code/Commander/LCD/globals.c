/*******************************************************************************
 * MODULE:  GLOBAL DEFINITIONS
 *..............................................................................
 *..............................................................................
 *  Created By:         ZIJI
 *  Created Date:       22.10.2015
 *..............................................................................
 *  Last Edit Date:     22.10.2015
 *  Last Edit By:       ZIJI
 *  Last Edit Details:  created file
 *..............................................................................
 * File/Function Property/Usage:
 *  See description below.
 *
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
 *  Currently working and utilises servo and LCD modules. Other modules to
 *  be added later.
*******************************************************************************/

    /* Chip Configurations */
    //#include <pic18f452.h>
    //#include "ConfigRegs18f452.h"
#include <p18cxxx.h>
//#include  <usart.h>
#include  "GlobalVaribles.h"

char GLOBAL_RXBUFFER[BUFFSIZE] = "0";
char  GLOBAL_MAX_SPEED = 0;
char  GLOBAL_MAX_YAW = 0;
char  GLOBAL_IR_SAMPLE_E = 0;
char  GLOBAL_IR_SAMPLE_R = 0;
char  GLOBAL_MODE = 0;



