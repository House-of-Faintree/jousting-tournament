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
#include  "serialcommander.h"

void setupSerial(void);
int sendByte(char *byte);

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
int sendByte(char *byte){ 
    while(*byte != 0x00){
    while(TXSTAbits.TRMT == 0);
    TXREG = *byte;
    byte++;
    }
 /*
    TXREG = 0x0d; //carriage return
    _asm
        NOP        
        NOP        
    _endasm
    TXREG = 0x0a; //Line feed
    _asm
            nop
            nop
    _endasm
            */
    return 0;    //we are finished, null terminator found
}

/*This function sets up the serial port of the PIC for operation.
Since it changes internal register values there are no arguments or
return values associated with it.
Baud rate set to 9600 but this is arbitrary. Lower baud is possibly better for multitasking,
but higher baud makes for smaller data packets. Needs to be experimented with.
*/
void setupSerial(void){
  TRISCbits.RC6 = 0;              //set rc6 as output
  TRISCbits.RC7 = 1;              //set rc7 as input
  TXSTAbits.SYNC = 0;             //Set to asynchronous mode
  TXSTAbits.BRGH = 0;             //Set baudrate generator to high
  TXSTAbits.TXEN = 1;
  BAUDCONbits.BRG16 = 0;
  SPBRG = 16;                     //Set baudrate to 9600 for 10Mhz
  RCSTAbits.CREN = 1;             //Enables receiver in continuous mode
  RCSTAbits.SPEN = 1;             //enable serial port
  //TXSTA = 0x24;                 //All settings up TXSTA done
}
