/*
Authored by Leo Lou
Dated 14 October 2015
********************************************************************************
This file proves external use of some functions related to the serial module.
These functions were designed to be run on the COMMANDER side, however some may
be compatible for running on the MOBILE ROBOT side.
*/

//Pseudo code for menu navigation by Leo


#include  <stdio.h>
#include  <p18cxxx.h>
#define UP 200
#define DOWN 40
#define LEFT 200
#define RIGHT 40

int  RUN;
int joy_x;
int joy_y;
int max_speed=20;
int pid_gain=50;
int max_yaw=30;
int ir_samp_e=60;
int ir_samp_r=30;
int ir_raw=30;
int ir_avg=50;
int values[7]={max_speed, pid_gain, max_yaw, ir_samp_e, ir_samp_r, ir_raw, ir_avg};
//Setup
int menu_ref_1=1;           //Change through ISR
int menu_ref_2=1;    


void main(void){
  
  RUN==0;
  
  while(1){
      
  
//NORMAL OPERATION  
    while(RUN==0){
      
        if(menu_ref_1==1)
        {
            if(joy_x<=LEFT){      //User pushes right joystick left
                if (menu_ref_2<6 && values[menu_ref_2]>0){
                  values[menu_ref_2]-=5;
                }
                disp_LCD(menu_ref_1, menu_ref_2);
                delayms(200);

            }
            else if (joy_x>=RIGHT){     //User pushes right joystick right
                if (menu_ref_2<6 && values[menu_ref_2]<100){
                    values[menu_ref_2]+=5;
                }
                disp_LCD(menu_ref_1, menu_ref_2);
                delayms(200);

            }        


        }  

        if(menu_ref_1==2){            //WHEN IT IS IN FACTORY MODE

            if (joy_y>=UP){            //User pushes left joystick UP
              menu_ref_2--
              //Circular selection

                if(menu_ref_2==0){
                  menu_ref_2=4;
                }
                disp_LCD(menu_ref_1, menu_ref_2);
                delayms(250);               //Arbitrary delay of 250 milliseconds

            }
            else if (joy_y<=DOWN){      //User pushes left joystick DOWN
                menu_ref_2++;
                //Circular selection
                if(menu_ref_1==5){
                  menu_ref_2=1;
                }
                disp_LCD(menu_ref_1, menu_ref_2);
                delayms(250);

            }
            else if (joy_x<=LEFT){      //User pushes right joystick left
                decrement(menu_ref_2);
                disp_LCD(menu_ref_1, menu_ref_2);
                delayms(100);

            }
            else if (joy_x>=RIGHT){     //User pushes right joystick right
                increment(menu_ref_2);
                disp_LCD(menu_ref_1, menu_ref_2);
                delayms(100);

            }            
        }   
      }
  }
  
  
}

  void highPriorityIsr(void)
{    
    //PORT B bit 0 external interrupt for motors on/off button
    if(INTCONbits.INT0IF == 1)
    {
          if (RUN==0)
                {
                RUN==1;
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String(t4);     //Motor on message
                delays(2);
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String(t5);
                }
    }

    //Port B bit 1 external interrupt for menu button
    if(INTCON3bits.INT1IF == 1)  
    {
        if (RUN==0){
           menu_ref_1++;
           if (menu_ref_1==5){
               menu_ref_1==0;
            }

        }
        INTCON3bits.INT1IF = 0; //clear flag
        INTCONbits.GIEH = 1; //re-enable interrupts
        PORTBbits.RB1 = 0; //clear port B bit
    }        
