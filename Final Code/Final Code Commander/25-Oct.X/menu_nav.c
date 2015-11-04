/*
Authored by Leo Lou
Dated 14 October 2015
********************************************************************************
This file provides external use of some functions related to the serial module.
These functions were designed to be run on the COMMANDER side, however some may
be compatible for running on the MOBILE ROBOT side.

The main function of this file is to serve as the menu navigation software. This 
software takes in user input, displays output to LCD, and uses external and internal
serial functions to interface with Robot.
*/



#include  <stdio.h>
#include  <p18cxxx.h>
#include  <delays.h>
#include  <string.h>
#include  "ConfigRegs18f4520.h"
#include  "phrases.h"
#include  "lcd.h"
#include  "AD.h"
#include  "GlobalVariblesCommander.h"
#include  "Interrupt_Defination_Commander.h"
#include  "serialcommander.h"




#define UP 240
#define DOWN 10
#define LEFT 10
#define RIGHT 240

#define MANUAL 0
#define FACTORY 1
#define ASSIST  2
#define AUTO    3

# define MAXSPEED 0
# define PIDGAINS 1        //PID Gains
# define TIMEHOR 2        //Max Yaw
# define IRSAMPE 3         //ir_samp_e;
# define IRSAMPR 4         //ir_samp_r;
# define IRRAW 5         //ir_raw
# define IRAVG 6

//ATTENTION
int MNML=1;             //CHANGE DEPENDING ON WHAT BOARD IS USED, 1 FOR MNML

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

int mode_button=0;        //flag for button to change between factory, manual mode etc
int motor_button=0;       //flag for changing between RUNTIME code and MENU code
int values[8];


/*to do */


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
int *menu=&menu_ref_1;


//Prototypes
void welcome(void);
void delay10us(int x);
void delayms(int x);
void delays(int x);
void LCD_disp(int title_item, int value_item);
void Button_Setup(void);
void on_setup(void);
void checkMode(int *menu);


void main(void){

  //Default
  TRISD = 0x00;         //set port D signals to output
  // By default, code starts in manual mode
  menu_ref_1=MANUAL;
  menu_ref_2=MAXSPEED;


  /*Values array stores important variables for factory mode*/

  values[MAXSPEED]=100;        //Max speed
  values[PIDGAINS]=100;        //PID Gains
  values[TIMEHOR]=100;        //Time horizon
  values[IRSAMPE]=10;         //ir_samp_e;
  values[IRSAMPR]=20;         //ir_samp_r;
  values[IRRAW]=60;         //ir_raw
  values[IRAVG]=10;         //ir_avg;

  /*****************/
  TMR0Init();
  setupSerial();
  Lcd_Init();
  ADC_setup();
  Button_Setup();
  welcome();
  LCD_disp(menu_ref_1, menu_ref_2);

  RUN=0;
  while(1){


//NORMAL OPERATION
    while(RUN==0){

        //Determine which button has been pressed
        if(PORTDbits.RD0 == 0)
        {
            //Mode button indicates a request for a change. Change is initiated
            //when safe to do so
            mode_button = 1;
        }

        if(PORTDbits.RD1 == 0)
        {
            motor_button = 1;
        }




        if (mode_button==1){                  //Mode change sequence
                //Checkmode syncs the modes between commander and robot
                checkMode(menu);
                menu_ref_2=0;         //Default menu ref, should only be SPEED
                delayms(200);
                mode_button=0;
                LCD_disp(menu_ref_1, menu_ref_2);
        }

        delayms(200);       //Used to prevent double tapping
        //possibly new function
        //As of now: Channel 2 = x, Channel 1 = y
        switchChannels(2);
        joy_x = doADC();
        switchChannels(1);
        joy_y = doADC();


        if(menu_ref_1==MANUAL)   //IF IN MANUAL
        {


            if(joy_x<=LEFT){      //User pushes right joystick left

                if(values[menu_ref_2]>0){
                    values[menu_ref_2]=values[menu_ref_2]-5;          //decrement value by 5%
                    GLOBAL_MAX_SPEED = values[menu_ref_2];
                    sendMaxSpeed();
                }

                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(200);     //Delay to prevent flickering, and also to prevent 100-0 increments

            }
            else if (joy_x>=RIGHT){     //User pushes right joystick right
                if(values[menu_ref_2]<100){

                    values[menu_ref_2]=values[menu_ref_2]+5;          //increment value by 5%
                    GLOBAL_MAX_SPEED = values[menu_ref_2];
                    sendMaxSpeed();
                }

                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(200);
            }
        }

        else if(menu_ref_1==FACTORY){            //IF IN FACTORY MODE
            if (joy_y>=UP){            //User pushes left joystick UP
                //Circular selection
                if(menu_ref_2==0){
                  menu_ref_2=6;
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
                //Some values should not be adjusted from menu
                if (menu_ref_2<IRRAW){

                    if (menu_ref_2==3 && values[menu_ref_2]>0){
                        values[menu_ref_2]=values[menu_ref_2]-1;

                    }
                    else if(values[menu_ref_2]>0){
                        values[menu_ref_2]=values[menu_ref_2]-5;          //decrement value by 5%
                    }
                }

                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(255);
                delayms(200);

            }
            else if (joy_x>=RIGHT){     //User pushes right joystick right

                if (menu_ref_2<IRRAW){

                    if (menu_ref_2==3 && values[menu_ref_2]<10){
                        values[menu_ref_2]=values[menu_ref_2]+1;

                    }

                    else if(values[menu_ref_2]<100){
                        values[menu_ref_2]=values[menu_ref_2]+5;          //decrement value by 5%
                    }
                }
                LCD_disp(menu_ref_1, menu_ref_2);
                delayms(255);
                delayms(200);

            }
        }

        //All other menu_ref_1's do not have values displayed

        if (motor_button==1){
            motor_button=0;
            RUN=1;

        }
        //send here
    }

    /*Menu on setup*/
    if (RUN==1){
        on_setup();
        GLOBAL_RUN = RUN;
        sendRun();
        //MOTOR ON GOES AFTER
        motor_button=0;
    }

      /*MOTOR ON BEHAVIOUR*/
    while(RUN==1 && menu_ref_1==MANUAL){
        if(PORTDbits.RD1 == 0)
        {
            motor_button = 1;
        }
        if (motor_button==1){      //At interrupt, stop motor and return to menu
            motor_button=0;
            RUN=0;
            GLOBAL_RUN = RUN;
            sendRun();

        }
        switchChannels(1);
        //GLOBAL_VELOCITY = doADC();    GLOBAL_VELOCITY = doADC() //Get joystick values
        joy_y = doADC();
        GLOBAL_VELOCITY = joy_y;
        sendVelocity();

        switchChannels(2);
        joy_x = doADC();
        GLOBAL_OMEGA = joy_x;


        sendOmega();

        //send command here
    }
    //Auto mode
        while(RUN==1 && menu_ref_1>MANUAL){
            char  cm_info[LCDL];
            if(PORTDbits.RD1 == 0)
            {
                motor_button = 1;
            }
            if (motor_button==1){      //At interrupt, stop motor and return to menu
                motor_button=0;
                RUN=0;
                GLOBAL_RUN = RUN;
                sendRun();


            }

            //Display additional info
            Lcd_Set_Cursor(2,1);
            sprintf(cm_info, "IR: %d",GLOBAL_IRRAW);
            Lcd_Write_String(cm_info);


    }


      /*MOTOR OFF MESSAGE*/
    RUN=0;
      //MOTOR OFF FUNCTION GOES BEFORE MESSAGE
    LCD_title(t5);         //"Whoa!"

    delays(2);    //safety delay

    LCD_disp(menu_ref_1, menu_ref_2); //Return to normal display



  }




}

void Button_Setup(void)
{

    TRISDbits.RD0 = 1;
    TRISDbits.RD1 = 1;

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
    if (menu_ref_1<=1){ 

        if (menu_ref_2<=2){
            Lcd_Set_Cursor(2,1);
            sprintf(string, "%s%d%", stringtab[value_item],values[value_item]);
            Lcd_Write_String(string);
        }
        //Statistics
        //Average: takes the mean of values stored in the IR array
        //Not implemented into final code but kept as placeholder
        else if(menu_ref_2==7){
//            unsigned char i;
//            int total;
//            int average;
//            for (i=values[IRSAMPE];i>0;i--){
//                total=+IR_samps[i];         //IR_samps defines the array not yet present
//            }
//            values[IRAVG]=total/values[IRSAMPE];
//            sprintf(string,"%s%d", stringtab[IRAVG], values[IRAVG]);
//            Lcd_Write_String(string);

        }

        else {
            Lcd_Set_Cursor(2,1);
            sprintf(string, "%s%d", stringtab[value_item],values[value_item]);//no % symbol
            Lcd_Write_String(string);
        }
    }
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
    delays(2);
}

/*DELAY FUNCTIONS, SUITABLE FOR BOTH BOARDS*/
//Delay for seconds indicated
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
//Delay for ms indicated
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
/*Function creates an initiation sequence when motor turns on*/
void on_setup(void){
      /*MOTOR ON MESSAGE*/
      LCD_title(t4);     //Giddy up
      delays(1);
      //Display run-time message
      LCD_title(t6);     //"Running"

      //If motor is set on while on factory, defaults to manual mode
      if (menu_ref_1==1){
          menu_ref_1=0;
          checkMode(menu);
      }
}


void checkMode(int *menu){
    if(*menu == MANUAL)
                {
                    *menu = FACTORY;
                    GLOBAL_MODE = FACTORY;
                    sendMode();
                }
                else if(*menu == FACTORY)
                {
                    *menu = ASSIST;
                    GLOBAL_MODE = ASSIST;
                    sendMode();
                }
                else if(*menu == ASSIST)
                {
                    *menu = AUTO;
                    GLOBAL_MODE = AUTO;
                    sendMode();
                }
                else if(*menu == AUTO) {
                    *menu = MANUAL;
                    GLOBAL_MODE = MANUAL;
                    sendMode();

                }
}
