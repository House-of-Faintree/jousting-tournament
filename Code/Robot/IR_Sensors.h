/*******************************************************************************
 * MODULE:  IR_Sensors
 *..............................................................................
 *  Created By:         BEVAN
 *  Created Date:       21.10.2015
 *..............................................................................
 *  Last Edit Date:     23.10.2015
 *  Last Edit By:       BEVAN
 *  Last Edit Details:  cleaned up not tested yet
 *..............................................................................
 * File/Function Property/Usage:
 *  See description below.
 *
 *..............................................................................
 * Description:
 *  Controls the setup and use of IR Sensors
 *  To be used for the robot
 *  Outputs the current distance from the tilt (robot has to be parallel to be accurate)
 *  As well as the state of the 2 IR_Sensors - ERROR,FRONT_IR_CLOSEST,PARALLEL,BACK_IR_CLOSEST (0-3) 
 *
 * Dependancies:
 *  ADC must first be set up
 *  Uses global variables:
 *  IR_SAMPLE_R = sample rate as percent 0 - 100% in increments of 10 
 *  IR_SAMPLE_E = number of samples to use to get an average distance int 0 - 10
 *  Wont function if either IR_SAMPLE_R or IR_SAMPLE_E = 0 
 *  Uses the high priority interrupt with timer0
 * 
 *..............................................................................
 * Current Issues:
 *  not yet tested
 *  
*******************************************************************************/


#ifndef IR_SENSORS_H
#define	IR_SENSORS_H

#define FRONT_IR 0 //Port A 0 
#define BACK_IR 1 // Port A 1

#define IR_BUFFSIZE 10 //max IR_SAMPLE_E

#define UPPER_IR_LIMIT 125 //4 cm 2.45V
#define LOWER_IR_LIMIT 28 //30cm 0.56V

#define PARALLEL_ACCURACY 1 //in cm +-

#define IR_ERROR 0   //both Front and Back IRs are out of range
#define BACK_IR_CLOSEST 1
#define IR_PARALLEL 2
#define FRONT_IR_CLOSEST 3

char Get_IR_state(); //returns char 0-3 inclusive matching with above defines IR_ERROR, etc
float Get_Current_distance(); //gives the average value between the two IR sensors in cm
void Infrared__Interrupt_Setup(void); //setups IR including setting up ADC and timer0 overflow interrupt
void IR_Interrupt(void); // add into high priority interrupt in another file, need to check and clear interrupt bits elsewhere

#endif	/* IR_SENSORS_H */

