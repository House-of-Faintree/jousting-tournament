/*******************************************************************************
 * MODULE:  Robot main 
 *..............................................................................
 *  Created By:         BEVAN
 *  Created Date:       26.10.2015
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
 * This is the main file for the robot and controls the robots operation during 3 modes:
 * 0 - Manual mode: Takes input from joysticks through global variables and calls turn function in PWM_Motor.c  
 * 2 - Assist mode: Uses readings from IR sensor to make robot parallel with tilt, uses function new_turn in PWM_Motor.c  
 * 3 - Auto mode: Jousts autonomously using IR sensors turns using turn function in  PWM_Motor.c with set velocity and omega 
 *
 * Dependancies:
 *  This is the main file for the robot and is therefore dependant on all other robot modules.
 *  GlobalVariblesRobot
 * ConfigRegs18f4520
 * serialRobot
 * Interrupt_Defination_Robot
 * PWM_Motor
 * IR_Sensors
 *..............................................................................
 * Current Issues:
 *  Auto mode still doesn't work perfectly may need to alter set values for turning
 *  Cannot change mode through controller. Have to manually change GlobalVariablesRobot.c
 *  PWM_Motor turn function not working. So still using simple turn method for manual and hard-coding values for auto mode
 *  Assist mode uses new control to mke robot parallel with tilt, however this method does not work.
 *  
*******************************************************************************/
#include  <p18f4520.h>
#include  <usart.h>
#include  "GlobalVariblesRobot.h"
#include  "ConfigRegs18f4520.h"
#include  "serialRobot.h"
#include  "Interrupt_Defination_Robot.h"
#include  "PWM_Motor.h"
#include  "AD.h"
#include  "IR_Sensors.h"

#define MANUAL 0
#define FACTORY 1
#define ASSIST  2
#define AUTO    3

#define AUTO_VEL 140
#define AUTO_OMEGA 170

void main(void)
{
    PWM_motor_setup();
    ADC_setup();
    setupSerial();
    IR_Setup();
    TMR2_INT_setup();
    while(1)
    {
        while(GLOBAL_MODE == MANUAL)
        {
          //turn(GLOBAL_VELOCITY,GLOBAL_OMEGA);
          noob_turn(GLOBAL_VELOCITY,GLOBAL_OMEGA);
            
        }
        
        while(GLOBAL_MODE == ASSIST)
        {
            IR_Calculate();                  

            if(dist_count == 2)
            {
                 new_turn(5, Get_dist_diff());
                newdistance = 0;
            }                       
                
        }
        
        while(GLOBAL_MODE == AUTO)
        {
            IR_Calculate();          
          
            if(newstate == 1)
            {
                    if(Get_S_IR_state() == IR_TOO_CLOSE)
                    {
//                        turn(AUTO_VEL, AUTO_OMEGA);
                        CCPR1L =  40;
                        CCPR2L =  50;
              
                    }
                    else if(Get_S_IR_state() == IR_TOO_FAR)
                    {
//                        turn(AUTO_VEL,255 -AUTO_OMEGA);
                        CCPR1L =  50;
                        CCPR2L =  40;
              
                    }
                    else if(Get_S_IR_state() == IR_ERROR)
                    {
                        turn(128,128);
              
                    }
                    newstate = 0;
            }
          
       }                   
        
        
    }
}





