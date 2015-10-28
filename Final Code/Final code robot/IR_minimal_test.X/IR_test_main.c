
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
    //TMR0Init();
    PWM_motor_setup();
    //Inputs_motor_setup();
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
            //turn(128,128);

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





