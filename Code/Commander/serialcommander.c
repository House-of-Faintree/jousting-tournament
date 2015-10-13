/*
Authored by Ayush Sharma
Dated 24th September 2015
********************************************************************************
This file proves external use of some functions related to the serial module.
These functions were designed to be run on the COMMANDER side, however some may
be compatible for running on the MOBILE ROBOT side.
*/



#include  <p18f452.h>
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
    TXREG = *byte;
    if (*byte == 0x00) {
    //we are finished, null terminator found
    return 0;
    }
    *byte++;
    return 1;
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
  TXSTAbits.BRGH = 1;             //Set baudrate generator to high
  TXSTAbits.TXEN = 1;
  SPBRG = 25;                     //Set baudrate to 9600 for 4Mhz
  RCSTAbits.CREN = 1;             //Enables receiver in continuous mode
  RCSTAbits.SPEN = 1;             //enable serial port
  //TXSTA = 0x24;                 //All settings up TXSTA done
}
