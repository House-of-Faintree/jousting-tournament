
#include <p18cxxx.h>
#include "lcd.h"
//LCD Functions Developed by electroSome
#define _XTAL_FREQ 4000000

#define RS PORTDbits.RD2
#define EN PORTDbits.RD3
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7

/*Takes in an ideally 4 bit value, and sends it down the data line
*/
void Lcd_Port(char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}

/*Sends a char down the data line in command mode
*/
void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        Delay1KTCYx(4);
        EN  = 0;             // => E = 0
}

/*Clears the LCD screen
*/
void Lcd_Clear()
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

/*As far as I know, char a is the row and char b is the column value. Unsure, since
LCD datasheet has a bunch of commands all very obscure to follow
*/
void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

void Lcd_Init()
{
	//Configures 4 bit interface
  Lcd_Port(0x00);
   Delay1KTCYx(20);
  Lcd_Cmd(0x03);
   Delay1KTCYx(5);
  Lcd_Cmd(0x03);
   Delay1KTCYx(20);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
	//Configures display for 2 lines
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

/*Writes a char to the screen, works well.
*/
void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   Delay10TCYx(4);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   Delay10TCYx(4);
   EN = 0;
}

/*Iterates lcd_write_char over a string. As such, takes pointer to a string address.
*/
void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

//Further investigation needed.
void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}

void LCD_title(char *string){
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String(string);
    
}
