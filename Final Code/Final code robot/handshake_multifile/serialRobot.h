/*******************************************************************************
 * MODULE:  GLOBAL DEFINITIONS
 *..............................................................................
 *  Created By:         ZIJI AN
 *  Created Date:       22.10.2015
 *..............................................................................
 *  Last Edit Date:     22.10.2015
 *  Last Edit By:       ZIJI
 *  Last Edit Details:  Created this file for serial communication on robot
 *..............................................................................
 * File/Function Property/Usage:
 *  See description below.
 *..............................................................................
 * Description:
 *  This file contains the definition of all functions used in serial communication
 *
 * Dependancies:
 *..............................................................................
 * Current Issues:
*******************************************************************************/
#ifndef SERIALROBOT_H
#define	SERIALROBOT_H

#include <p18cxxx.h>

#define Hello           254
#define HelloEnd        253
#define MaxSpeed        252
#define MaxSpeedEnd     251
#define MaxYaw          250
#define MaxYawEnd       249
#define IRsampleE       248
#define IRsampleEEnd    247
#define IRsampleR       246
#define IRsampleREnd    245
#define Mode            244
#define ModeEnd         243

void setupSerial(void);
int  sendByte(char *byte);
void sendNewLine(void);
void sendString(char *byte);
void sendNum(int result);
void rx232Isr (void);
void TMR0Isr (void);
void TMR0Init (void);

#endif	/* SERIALROBOT_H */

