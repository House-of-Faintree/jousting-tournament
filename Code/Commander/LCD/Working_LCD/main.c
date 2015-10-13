/*
Authored by Leo Lou
Dated 1 October 2015
********************************************************************************
This function takes in an number (as outlined in serialcommand.c) that refers to both a corresponding
string output and value output to the LCD
main function has been kept for testing purposes. The variable 'x' corresponds to menu_ref_1 from my previous
file. Further improvements could involve a look up table.
The function of this program depends on an agreed upon indexing system of variables and strings (to be discussed
later)
*/


#include  <stdio.h>
#include  <p18cxxx.h>
#include <delays.h>
#include "ConfigRegs.h"
#include "lcd.h"
#include "phrases.h"
#define internfreq  10   //In Mhz * 10, i.e. 1MHz = 10; 2.5Mhz = 25

//ATTENTION
int MNML=0;             //CHANGE DEPENDING ON WHAT BOARD IS USED, 1 FOR MNML

//Global Variables (in (%))
int pid_gain=50;
int max_speed=20;
int max_yaw=30;
int ir_samp_e=60;


//Pointers to Strings
const char *menutitle[4]={t0,t1,t2,t3};
const char *stringtab[4]={s0,s1,s2,s3};



void main(void)
{   
    void LCD_disp(int x,int y);
    void welcome(void);
    TRISD = 0x00;
    Lcd_Init();
    
    welcome();                      //welcome screen

    
    while(1){
    LCD_disp(1,2);
    delays(2);
    LCD_disp(0,3);
    delays(2);
    }
    
}

void LCD_disp(int x, int y)
{
    
    int values[4]={pid_gain,max_speed,max_yaw,ir_samp_e};            
    char  string[LCDL];
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(menutitle[x]);
    //Display as _string:_value'%'_
    sprintf(string, "%s%d%", stringtab[y],values[y]);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(string);

    //#############################

}

void welcome(void){
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(w0);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(w1);
    delays(5);
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
