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
 *  Uses global variables IR_SAMPLE_R, IR_SAMPLE_E
 *  Uses the high priority interrupt with timer0
 * 
 *..............................................................................
 * Current Issues:
 *  not yet tested
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

#define IR_ERROR 0   //both Front and Back IRs are out of range
#define IR_TOO_CLOSE 1
#define IR_PARALLEL 2
#define IR_TOO_FAR 3

extern char Get_S_IR_state(void); //returns char 0-3 inclusive matching with above defines IR_ERROR, etc
extern float Get_Current_distance(void); //gives the average value between the two IR sensors in cm
extern void IR_Setup(void); //setups IR including setting up ADC and timer0 overflow interrupt
extern void IR_Sample(void);
extern void IR_Calculate(void);
extern int newdistance;
extern int newstate;
extern int dist_count;
extern float Get_dist_diff(void);
#endif	/* IR_SENSORS_H */

