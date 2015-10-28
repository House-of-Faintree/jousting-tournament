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

#ifndef INTERRUPT_DFINITIONS_H
#define	INTERRUPT_DFINITIONS_H

/* Prototypes for the interrupt calls and distribution functions */
/******************************************************************************/
void HIGH_PRIORITY_INTERRUPT(void);
void LOW_PRIORITY_INTERRUPT(void);
void HIGH_PRIORITY_ISR(void);
void LOW_PRIORITY_ISR(void);
//void Initiate_Interrupts(void);
void TMR2_INT_setup(void);

#endif  /* INTERRUPT_DFINITIONS_H */