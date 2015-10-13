#ifndef __LCD_H
#define __LCD_H

#include <p18cxxx.h>




void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear();
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init();
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right();
void Lcd_Shift_Left();
void delays(int x);
void delayms(int x);
void delay10us(int x);

#endif
