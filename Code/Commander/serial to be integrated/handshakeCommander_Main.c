
#include  <p18f4520.h>
#include  <usart.h>
#include  "GlobalVariblesCommander.h"
#include  "ConfigRegs18f4520.h"
#include  "serialCommander.h"
#include  "Interrupt_Defination_Commander.h"
#include  "AD.h"



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
    
    ADC_setup();
    setupSerial();
    while(1)
    {
        switchChannels(1);
        GLOBAL_VELOCITY = doADC();
        sendVelocity();
        switchChannels(0);
        GLOBAL_OMEGA = doADC();
        sendOmega();
        
    }
}


