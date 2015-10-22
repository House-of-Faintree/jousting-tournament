/*******************************************************************************
 * MODULE:  GLOBAL DEFINITIONS
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
 *  
 *  
*******************************************************************************/



#ifndef GLOBALS_H
#define	GLOBALS_H

    /* Chip Configurations */
    #include    <p18cxxx.h>
    //#include    "ConfigRegs.h"

#define BUFFSIZE        50

extern char  GLOBAL_RXBUFFER[BUFFSIZE];
extern char  GLOBAL_MAX_SPEED;
extern char  GLOBAL_MAX_YAW;
extern char  GLOBAL_IR_SAMPLE_E;
extern char  GLOBAL_IR_SAMPLE_R;
extern char  GLOBAL_MODE;



#endif  /* GLOBALS_H */
