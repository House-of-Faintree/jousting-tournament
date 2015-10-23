/*******************************************************************************
 * MODULE:  ROBOT_CONTROL
 *..............................................................................
 *  Created By:         BEVAN
 *  Created Date:       23.10.2015
 *..............................................................................
 *  Last Edit Date:     23.10.2015
 *  Last Edit By:       BEVAN
 *  Last Edit Details:  made file
 *..............................................................................
 * File/Function Property/Usage:
 *  See description below.
 *
 *..............................................................................
 * Description:
 *  Main file of Robot 
 *  Controls during the different modes MANUAL,ASSIST,AUTO
 *  Uses all robot modules ADC,IR_SENSORS,PWM_MOTOR,SERIAL_ROBOT
 *   
 *
 * Dependancies:
 *  Uses Global variable MODE
 *  Uses all other robot modules
 * 
 *..............................................................................
 * Current Issues:
 *  not yet tested, 
 *  clarify PWM_motors turn() function
 *  Get values from controller
 *  Send values to controller (distance from tilt)
 *  Add more global variables - RUN, joysticks, Full auto distance
 *  Get error saying not enough space for lowpriority interrupt when using serial
 *  
*******************************************************************************/



#include <p18cxxx.h>
#include "configReg452.h"
//#include "serialRobot.h"
#include "globals.h"
#include "AD.h"
#include "IR_Sensors.h"


#define MANUAL 0
#define FACTORY 1
#define ASSIST  2
#define AUTO    3


#define NEUTRAL 128
#define TURN_RIGHT 255
#define TURN_LEFT 0

char RUN = 1; //change to global 

int joy_turn = NEUTRAL; //change to global 
int auto_turn = NEUTRAL; 
float Full_auto_distance = 10; //change to global



void turn(int turn_value);
void Manual_Mode(void);
void User_Assist_Mode(void);
void Full_Auto_Mode(void);

    

void main(void) {
    ADC_setup;
    IR_Setup();
    //setupSerial();
    //PWM_motor_setup();
    //Inputs_motor_setup();
    while(1)
    {
        
        while((GLOBAL_MODE == MANUAL) && (RUN == 1))
        {
            Manual_Mode();
        }
        
        while((GLOBAL_MODE == ASSIST) && (RUN == 1))
        {
            User_Assist_Mode();
        }
        
        while((GLOBAL_MODE == AUTO) && (RUN == 1))
        {
            Full_Auto_Mode();            
        }
        
    }
}


void Manual_Mode(void)
{
    //turn(joy_turn);
}

void User_Assist_Mode(void)
{
     while((joy_turn == NEUTRAL) && (RUN == 1) && !(Get_IR_state() == IR_PARALLEL) && !(Get_IR_state() == IR_ERROR))
    {
        if(Get_IR_state() == BACK_IR_CLOSEST)
        {
            auto_turn = TURN_LEFT;
        }
        else if(Get_IR_state() == FRONT_IR_CLOSEST)
        {
            auto_turn = TURN_RIGHT;
        }   
        else
        {
            auto_turn = NEUTRAL;
        }
       
        //turn(auto_turn);                
    }
    
     //send current distance to controller
            
    //turn(joy_turn);
}

void Full_Auto_Mode(void)
{
    while((RUN == 1) && !(Get_Current_distance() == Full_auto_distance) && (Get_IR_state() == IR_PARALLEL))
    {
        //send current distance to controller
        
        if(Get_Current_distance() >= Full_auto_distance)
        {
            auto_turn = TURN_LEFT;
        }
        else if(Get_Current_distance() <= Full_auto_distance)
        {
            auto_turn = TURN_RIGHT;
        }
                
       // turn(auto_turn);
    }
            
    while((RUN == 1) && !(Get_IR_state() == IR_PARALLEL) && !(Get_IR_state() == IR_ERROR))
    {
        if(Get_IR_state() == BACK_IR_CLOSEST)
        {
            auto_turn = TURN_LEFT;
        }
        else if(Get_IR_state() == FRONT_IR_CLOSEST)
        {
            auto_turn = TURN_RIGHT;
        }   
        else
        {
            auto_turn = NEUTRAL;
        }
       
       // turn(auto_turn);                
    }
}
 