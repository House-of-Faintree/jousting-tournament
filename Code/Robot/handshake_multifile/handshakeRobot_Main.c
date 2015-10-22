
/*
Authored by Ayush Sharma
Dated 24th September 2015
********************************************************************************
This file proves external use of some functions related to the serial module.
These functions were designed to be run on the COMMANDER side, however some may
be compatible for running on the MOBILE ROBOT side.
*/



#include  <p18f4520.h>
#include  <usart.h>
#include  "GlobalVariblesRobot.h"
#include  "ConfigRegs18f4520.h"
#include  "serialRobot.h"
#include  "Interrupt_Defination_Robot.h"



/*This function sends individual bytes and as such needs to have a iterative
loop such as a while implemented in the calling function. With a minor modification,
namely of a while inside this function, it can be transformed to sendString rather
than sendByte. However, implementing while loops inside nested functions
seemed like it might give us some problems to debug.
Takes in a dereferenced pointer to an address, and returns one of a set of debug integers

Usage:
if someString = 'command to send to the robot'

while(sendByte(&someString)){
  do other things between sending chars;
}
*/


void main(void)
{
    TMR0Init();
    setupSerial();
    while(1)
    {
    }
}


