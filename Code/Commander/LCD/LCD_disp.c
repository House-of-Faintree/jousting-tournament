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
#define LCDL 17

int pid_gain=50;
int max_speed=20;
int max_yaw=30;
int ir_samp_e=60;

//NTS exercise care with string lengths
const char s0[LCDL]="PID gain:";
const char s1[LCDL]="Max speed:";
const char s2[LCDL]="Max yaw:";
const char s3[LCDL]="IR samp/est:";
    
const char *stringtab[4]={s0,s1,s2,s3};

void main(void)
{   
    void LCD_disp(int x);
    TRISD = 0x00;
    Lcd_Init();
    
    while(1){
    LCD_disp(2);
    }
    
}

void LCD_disp(int x)
{
    
    int values[4]={pid_gain,max_speed,max_yaw,ir_samp_e};            
    char  string[LCDL];
    Lcd_Clear();
    
    //Display as _string:_value'%'_
    sprintf(string, "%s%d%", stringtab[x],values[x]);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(string);
    Delay10KTCYx(200);
}
