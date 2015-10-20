/*
Authored by Leo Lou
Dated 14 October 2015
********************************************************************************
This file proves external use of some functions related to the serial module.
These functions were designed to be run on the COMMANDER side, however some may
be compatible for running on the MOBILE ROBOT side.
*/

//Code for menu navigation by Leo
//TODO: 
//    - try and do navigation through function calls
//    - move interrupt code to interrupts.c
//    - different run time message for each runtime mode

#include  <stdio.h>
#include  <p18cxxx.h>
#include  <delays.h>
#include  <string.h>
#include  "ConfigRegs.h"
#include  "phrases.h"
#include  "lcd.h"
#include  "AD.h"

#define UP 240
#define DOWN 10
#define LEFT 10
#define RIGHT 240


# define MAXSPEED 0
# define PIDGAINS 1        //PID Gains
# define MAXYAW 2        //Max Yaw
# define IRSAMPE 3         //ir_samp_e;
# define IRSAMPR 4         //ir_samp_r;
# define IRRAW 5         //ir_raw
# define IRAVG 6

//ATTENTION
int MNML=0;             //CHANGE DEPENDING ON WHAT BOARD IS USED, 1 FOR MNML

int  RUN;
int joy_x;
int joy_y;
int max_speed;
int pid_gain;
int max_yaw;
int ir_samp_e;
int ir_samp_r;
int ir_raw;
int ir_avg;
//int values[] = { max_speed, pid_gain, max_yaw, ir_samp_e, ir_samp_r, ir_raw, ir_avg}; 


int values[8];





/*
 *Pointer Arrays
 ************************************
 * These arrays contain pointers to string constants that need to be written
 * often and do not need to be modified. The strings can be found in
 * phrases.h
 */
char *menutitle[4]={t0,t1,t2,t3}; 
char *valueStrings[4]={s0,s1,s2,s3};
char *stringtab[7]={s0,s1,s2,s3,s4,s5,s6};

/****************************************************************************
 * menu_ref_1 is a reference variable which keeps track of where in the menu
 * hierarchy the user currently is. It is used as an array index in the array
 * menutitle[].
 * **************************************************************************
 * menu_ref_2 is a reference variable that keeps track of menu subitems, and is
 * an array index in the array valueStrings[].
 */
int menu_ref_1;           //Change through ISR
int menu_ref_2;  

//Prototypes
void welcome(void);
void delay10us(int x);
void delayms(int x);
void delays(int x);
void LCD_disp(int title_item, int value_item);


void main(void){
  


  //Default
  TRISD = 0x00;         //set port D signals to output
  menu_ref_1=1;
  menu_ref_2=0;
  
  /*zone of disgust*/
  
  values[MAXSPEED]=100;        //Max speed
  values[PIDGAINS]=100;        //PID Gains
  values[MAXYAW]=100;        //Max Yaw
  values[IRSAMPE]=50;         //ir_samp_e;
  values[IRSAMPR]=20;         //ir_samp_r;
  values[IRRAW]=60;         //ir_raw
  values[IRAVG]=10;         //ir_avg;
  
  /*****************/
  

  Lcd_Init();
  welcome();
  ADC_setup();
  LCD_disp(menu_ref_1, menu_ref_2);
  RUN=0;
  while(1){

  
//NORMAL OPERATION  
    while(RUN==0){
        switchChannels(1);
        joy_x = doADC();
        switchChannels(2);
        joy_y = doADC();    //Get joystick values CHANGE THIS
      
        if(menu_ref_1==0)   //IF IN MANUAL
        {
            menu_ref_2=0;         //Default menu ref, should only be SPEED
            if(joy_x<=LEFT){      //User pushes right joystick left
                if(values[menu_ref_2]>0){
                    values[menu_ref_2]=values[menu_ref_2]-5;          //decrement value by 5%
                }
                
                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(255);     //Delay to prevent flickering, and also to prevent 100-0 increments

            }
            else if (joy_x>=RIGHT){     //User pushes right joystick right
                if(values[menu_ref_2]<100){
                
                values[menu_ref_2]=values[menu_ref_2]+5;          //increment value by 5%
                }
                
                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(255);

            }        


        }  

        else if(menu_ref_1==1){            //IF IN FACTORY MODE

            if (joy_y>=UP){            //User pushes left joystick UP
                
                //Circular selection

                if(menu_ref_2==0){
                  menu_ref_2=4;
                }
                else{
                  menu_ref_2--;             //switched the order of this to prevent overflow
                }
                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(250);               //Arbitrary delay of 250 milliseconds


            }
            else if (joy_y<=DOWN){      //User pushes left joystick DOWN
                menu_ref_2++;
                //Circular selection
                if(menu_ref_2>=7){
                  menu_ref_2=1;
                }
                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(250);
                

            }
            else if (joy_x<=LEFT){      //User pushes right joystick left
                //decval(menu_ref_2);
                
                if(values[menu_ref_2]>0){
                    values[menu_ref_2]=values[menu_ref_2]-5;          //decrement value by 5%
                }
                
                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(255);
                delayms(200);

            }
            else if (joy_x>=RIGHT){     //User pushes right joystick right
                
                if(values[menu_ref_2]<100){
                    values[menu_ref_2]=values[menu_ref_2]+5;          //decrement value by 5%
                }
                
                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(255);
                delayms(200);

            }            
        }  
        
        else{       //All other menu_ref_1's do not values displayed
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(menutitle[menu_ref_1]);
        delayms(200);
        }
      }
      /*MOTOR ON MESSAGE*/
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String(t4);
      
      delays(2);
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String(t6);     //Display run-time message
      
      if (menu_ref_1=1){
          menu_ref_1=0;       //If motor is set on while on factory, defaults to manual mode
      }
      
      /*MOTOR ON BEHAVIOUR*/
      while(RUN==1){
        
        switchChannels(0);
        joy_x = doADC();
        switchChannels(1);
        joy_y = doADC();    //Get joystick values
        
      }
      
      /*MOTOR OFF MESSAGE*/  
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String(t5);
      delays(1);
      LCD_disp(menu_ref_1, menu_ref_2); //Return to normal display
      
  }
  
  
}

/* TODO : 
 * Move code to interrupts.c
 * 
 */
  void highPriorityIsr(void)
{    
    INTCONbits.GIE = 0;
    //PORT B bit 0 external interrupt for motors on/off button
    if(INTCONbits.INT0IF == 1)
    {
     
        if (RUN==0)
        {
            RUN==1;

            //MOTOR ON FUNCTION GOES HERE

        }
        else
        {
            RUN==0;

            //MOTOR OFF FUNCTION GOES HERE
            delays(2);
        }
        

    }

    //Port B bit 1 external interrupt for menu button
    if(INTCON3bits.INT1IF == 1)         
    {
        if (RUN==0){                //menu_ref_1 does not change while running
           menu_ref_1++;
           if (menu_ref_1==5){
               menu_ref_1==0;
            }

        }

    }
    
    //Re-enable interrupts
    INTCON3bits.INT1IF = 0; //clear flag
    INTCONbits.GIE = 1; //re-enable interrupts
    PORTBbits.RB1 = 0; //clear port B bit
}        
  
//FUNCTIONS  
  
/*This function takes in 2 arguments, corresponding to what should be displayed on
 * the 1st and 2nd lines of the LCD screen. 
 * title_item selects which string to display as the title to the menu position.
 * value_item selects which string to display while changing the submenu item value.
 */  
void LCD_disp(int title_item, int value_item)
{
    
    char  string[LCDL];
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(menutitle[title_item]);
    
    //Checks if in Manual or Factory mode
    if (menu_ref_1<=1){ //possibly need to change order of phrases to
                        //include a check for user assist mode, in which case
                        //we check if menu_ref_1 <= 2
                        // Leo: we don't need to, it does it automatically

        if (menu_ref_2<=2){
            Lcd_Set_Cursor(2,1);
            sprintf(string, "%s%d%", stringtab[value_item],values[value_item]);
            Lcd_Write_String(string);
        }
        else {
            Lcd_Set_Cursor(2,1);    
            sprintf(string, "%s%d", stringtab[value_item],values[value_item]);//no % symbol
            Lcd_Write_String(string);
        }
        
    }
    


    //#############################

}

/*
 * Simply a function that prints out a standard welcome message on startup.
 */   
void welcome(void){
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(w0);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(w1);
    delays(3);
}


void delays(int x){
    if (MNML){
        for(x;x>0;x--){
        Delay10KTCYx(100);
        Delay10KTCYx(100);
        Delay10KTCYx(50);
        }
    }
    else{
        for(x;x>0;x--){
        Delay10KTCYx(100);
        }
    }
}

void delayms(int x){
    if (MNML){
        Delay1KTCYx(x);
        Delay1KTCYx(x);
        Delay1KTCYx(x);

    }
    else{
        Delay1KTCYx(x);
        
    }
}

void delay10us(int x){
    if (MNML){
        Delay10TCYx(x);
        Delay10TCYx(x);
        Delay10TCYx(x);

    }
    else{
        Delay10TCYx(x);      
    }    
}

/*void decval(int x){
//    values[x]=-5;
    
//}

//void incval(int x){
//    values[x]=+5;
 * }
    
*/
