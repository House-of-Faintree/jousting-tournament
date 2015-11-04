/*******************************************************************************
 * MODULE:  IR_Sensors
 *..............................................................................
 *  Created By:         BEVAN
 *  Created Date:       21.10.2015
 *..............................................................................
 *  Last Edit Date:     28.10.2015
 *  Last Edit By:       BEVAN
 *  Last Edit Details:  cleaned up 
 *..............................................................................
 * File/Function Property/Usage:
 *  See description below.
 *
 *..............................................................................
 * Description:
 *  Controls the setup and use of IR Sensors
 *  To be used for the robot
 *  Outputs the current distance from the tilt 
 *  As well as the state of the IR_Sensor - ERROR,FRONT_IR_CLOSEST,PARALLEL,BACK_IR_CLOSEST (0-3) 
 *
 * Dependancies:
 *  ADC must first be set up
 *  Uses global variables IR_SAMPLE_R, IR_SAMPLE_E
 * 
 *..............................................................................
 * Current Issues:
 *  
*******************************************************************************/


#ifndef IR_SENSORS_H
#define	IR_SENSORS_H

#define SIDE_IR 1 //Port A 1 

#define IR_BUFFSIZE 10 //max IR_SAMPLE_E

#define UPPER_IR_LIMIT 125 //4 cm 2.45V
#define LOWER_IR_LIMIT 10 //30cm 0.56V
#define IR_OFFSET 0

#define PARALLEL_ACCURACY 1 //in cm +-

#define IR_ERROR 0   //IR out of range
#define IR_TOO_CLOSE 1
#define IR_PARALLEL 2
#define IR_TOO_FAR 3

extern char Get_S_IR_state(void); //returns char 0-3 inclusive matching with above defines IR_ERROR, etc
extern float Get_Current_distance(void); //gives the current distance from tilt
extern void IR_Setup(void); //setups IR 
extern void IR_Sample(void); //samples ADC for IR values and stores in buff. Use in interrupt.
extern void IR_Calculate(void); //calculates a new value for distance and a new IR state, use before any Get function.
extern int newdistance; //flag set when a new distance has been calculated
extern int newstate; //flag set when the system changes IR state
extern int dist_count; //counts number of distances calculated, used in assist mode
extern float Get_dist_diff(void); //returns difference between the last 2 distances calculated, used in assist mode
#endif	/* IR_SENSORS_H */

