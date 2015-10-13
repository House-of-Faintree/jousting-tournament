

#include <p18cxxx.h>
#include <delays.h>
#include "lcd.h"
#include "ConfigRegs.h"


void main(void)
{
  unsigned int a;
  char *ptr;
  char str[4] = "LCD";          //IMPORTANT NOTE, ALWAYS SET STRING SIZE TO
  char str2[10] = "MPLAB C18";    //NO. OF CHARACTERS + 1
  TRISD = 0x00;
  Lcd_Init();
 
  while(1)
  {
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(str);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(str2);
    Delay10KTCYx(200);
//USE THESE FUNCTIONS IF YOU WANT TO BE COOL
/*
 * moves whatever is on the LCD back and forth
    for(a=0;a<15;a++)
    {
        Delay10KTCYx(20);
        Lcd_Shift_Left();
    }

    for(a=0;a<15;a++)
    {
        Delay10KTCYx(20);
        Lcd_Shift_Right();
    }
    */
    /*
    Lcd_Clear();
    Lcd_Set_Cursor(2,1);
    Lcd_Write_Char('e');
    Lcd_Write_Char('S');
    Delay10KTCYx(300);
    */
  }

}
