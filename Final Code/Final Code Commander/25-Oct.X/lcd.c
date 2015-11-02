
#include <p18cxxx.h>
#include "lcd.h"
#define RS PORTDbits.RD2
#define EN PORTDbits.RD3
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7

/*Sends a char down 4-bit data line. Function allows for simpler interfacing in 4-bit mode*/

void Lcd_Port(char data)
{
	if(data & 1)
		D4 = 1;
	else
		D4 = 0;

	if(data & 2)
		D5 = 1;
	else
		D5 = 0;

	if(data & 4)
		D6 = 1;
	else
		D6 = 0;

	if(data & 8)
		D7 = 1;
	else
		D7 = 0;
}

/*Sends a char in command mode to be used to instruct the LCD rather than write to it*/
void Lcd_Cmd(char a)
{
	RS = 0;             // Set data as command
	Lcd_Port(a);
	EN  = 1;             // Toggle enable on
     Delay1KTCYx(4);
     EN  = 0;             // Toggle enable off
}

/*Clears the LCD screen*/
void Lcd_Clear(void)
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

/*Function sets the cursor of the LCD based on two arguments; 'row' determines the row and 'col' determines the column*/

void Lcd_Set_Cursor(char row, char col)
{
	char temp,y,z;
	//Write in line 1
	if(row == 1)
	{
	  temp = 0x80 + col - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	//Write in line 2
	else if(row == 2)
	{
		temp = 0xC0 + col - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

/*LCD Initialisation function, uses external flow-chart as basis*/
void Lcd_Init()
{
	//Configures 4 bit interface
  Lcd_Port(0x00);	//Clear
   Delay1KTCYx(20);	//Delay
  Lcd_Cmd(0x03);		//Reset	
   Delay1KTCYx(20);	//Delay
  Lcd_Cmd(0x03);		//Reset
   Delay1KTCYx(5);	//Delay
  Lcd_Cmd(0x03);		//Reset
   Delay1KTCYx(5);	//Delay

	//Configures display for 2 lines
  Lcd_Cmd(0x02);	
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

/*Writes a char to the screen in 4-bit mode*/
void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;	 // Store LSB	
   y = a&0xF0;		 // Store MSB
   RS = 1;             // Alert LCD of incoming data
   Lcd_Port(y>>4);     // Send MSB
   EN = 1;			 // Input capture	
   Delay10TCYx(4);	 // Delay	
   EN = 0;			 
   Lcd_Port(temp);	 // Send LSB
   EN = 1;			 // Input Capture
   Delay10TCYx(4);
   EN = 0;
}

/*Takes pointer to string address and iterates through string, writing each character to LCD*/
void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}
/*A more generalised version of LCD_disp, useful for out of menu displays where global variables are not used*/
void LCD_title(char *string){
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String(string);
    
}
