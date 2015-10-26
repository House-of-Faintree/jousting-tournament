/*******************************************************************************
 * MODULE:  Interrupt Definitions Header File
 *............................................................................
 *  Created By:         ZIJI AN
 *  Created date:       22.10.2015
 *..............................................................................
 *  Last Edit Date:     22.10.2015
 *  Last Edit By:       ZIJI
 *  Last Edit Details:  create this file to write interrupt defination
 *..............................................................................
 * File/Function Property/Usage:
 *  Handles interrupt calls. This file is responsible for directing an interrupt
 *  occurance to the correct function to handle it bassed on the interrupt
 *  flags.
 * .............................................................................
 * Description:
 *
 * Dependancies:
 *
******************************************************************************/

#include "Interrupt_Defination_Commander.h"
#include <p18cxxx.h>
#include "GlobalVariblesCommander.h"
#include "serialCommander.h"

/*******************************************************************************
 * INTERRUPT VECTOR DEFINITIONS
 * places goto statementin the interrupt vector table for the high and low
 * priority interrupt functions
*******************************************************************************/
#pragma code High_priority_interrupt = 0x0008
void HIGH_PRIORITY_ISR(void)
{
    _asm GOTO HIGH_PRIORITY_INTERRUPT _endasm
}
#pragma code Low_priority_interrupt = 0x0018
void LOW_PRIORITY_ISR(void)
{
    _asm GOTO LOW_PRIORITY_INTERRUPT _endasm
}

#pragma interrupt HIGH_PRIORITY_INTERRUPT
void HIGH_PRIORITY_INTERRUPT(void)
{
    if(PIR1bits.RCIF == 1)
    {
        rx232Isr();
    }
}

#pragma interrupt LOW_PRIORITY_INTERRUPT
void LOW_PRIORITY_INTERRUPT(void)
{
    if (INTCONbits.TMR0IF == 1)
    {
        TMR0Isr();
    }
}