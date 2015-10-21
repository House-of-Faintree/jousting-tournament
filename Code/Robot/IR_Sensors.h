/* 
 * File:   IR_Sensors.h
 * Author: Bevan
 *
 * Created on 21 October 2015, 11:27 AM
 */

#ifndef IR_SENSORS_H
#define	IR_SENSORS_H

#define FRONT_IR 0 //Port A 0 
#define BACK_IR 1 // Port A 1

#define IR_BUFFSIZE 5 //takes average of values when buff full

#define UPPER_IR_LIMIT 125 //4 cm 2.45V
#define LOWER_IR_LIMIT 28 //30cm 0.56V

#define PARALLEL_ACCURACY 1 //in cm +-

#define IR_ERROR 0   //both Front and Back IRs are out of range
#define BACK_IR_CLOSEST 1
#define IR_PARALLEL 2
#define FRONT_IR_CLOSEST 3

float F_IR_cm = 0;
float B_IR_cm = 0;
char F_IR_inrange = 0; //0 not in range, 1 in range
char B_IR_inrange = 0;
char IR_state = 0; //uses above defines IR_ERROR, BACK_IR_CLOSEST, IR_PARALLEl, FRONT_IR_CLOSEST


void IR_Setup(void); //setups IR including setting up ADC and timer0 overflow interrupt
void IR_Interrupt(void); // gets converts values from IR sensors and determines what state they are in (parallel or not) 

#endif	/* IR_SENSORS_H */

