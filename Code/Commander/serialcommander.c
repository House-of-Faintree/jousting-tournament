
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

#define INIT_IPR1   0B00100000
#define BUFFSIZE    50

void setupSerial(void);
int sendByte(char *byte);
void sendNewLine(void);
void sendString(char *byte);
void sendNum(int result);

char GLOBAL_RXBUFFER[BUFFSIZE] = "0";
char *rxptr;
void rx232Isr();
#pragma code lowpriority = 0x18
void low_priority_interrupt(){

    if(PIR1bits.RCIF == 1)

        _asm goto rx232Isr 
        _endasm
    /*
    else if (PIR1bits.TXIF== 1)

        _asm goto tx232Isr _endasm
     */
}

#pragma code

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
    if(*byte != 0x00){
    while(TXSTAbits.TRMT == 0);
    TXREG = *byte;
    return 1;
    }
    return 0;    //we are finished, null terminator found
}

void sendNum(int result){
    while(TXSTAbits.TRMT == 0);
    TXREG = result;
    return;
}

void sendNewLine(void){
    while(TXSTAbits.TRMT == 0);
    TXREG = 0x0a; //Line feed
    
    while(TXSTAbits.TRMT == 0);
    TXREG = 0x0d; //carriage return'
    
}

void sendString(char *byte){
    
    while(sendByte(byte)){
        byte++;
    }
    sendNewLine();
    return;
    
}

/*This function sets up the serial port of the PIC for operation.
Since it changes internal register values there are no arguments or
return values associated with it.
Baud rate set to 9600 but this is arbitrary. Lower baud is possibly better for multitasking,
but higher baud makes for smaller data packets. Needs to be experimented with.
*/
void setupSerial(void){
  INTCONbits.GIE = 1;
  INTCONbits.PEIE = 1;
  IPR1 = INIT_IPR1;
  RCONbits.IPEN = 1;
  TRISCbits.RC6 = 0;              //set rc6 as output
  TRISCbits.RC7 = 1;              //set rc7 as input
  TXSTAbits.SYNC = 0;             //Set to asynchronous mode
  TXSTAbits.BRGH = 0;             //Set baudrate generator to high
  TXSTAbits.TXEN = 1;
  BAUDCONbits.BRG16 = 0;
  SPBRG = 16;                     //Set baudrate to 9600 for 10Mhz
  RCSTAbits.CREN = 1;             //Enables receiver in continuous mode
  RCSTAbits.SPEN = 1;             //enable serial port
  PIE1bits.RCIE = 1;
  //TXSTA = 0x24;                 //All settings up TXSTA done
  
}

void sendHello(void)
{
    sendNum(Hello);
    sendNum(HelloEnd);
}
void sendMaxSpeed(void)
{
    sendNum(MaxSpeed);
    sendNum(GLOBAL_MAX_SPEED);
    sendNum(MaxSpeedEnd);
}
void sendMaxYaw(void)
{
    sendNum(MaxYaw);
    sendNum(GLOBAL_MAX_YAW);
    sendNum(MaxYawEnd);
}
void sendIRsampleE(void)
{
    sendNum(IRsampleE);
    sendNum(GLOBAL_IR_SAMPLE_E);
    sendNum(IRsampleEEnd);
}
void sendIRsampleR(void)
{
    sendNum(IRsampleR);
    sendNum(GLOBAL_IR_SAMPLE_R);
    sendNum(IRsampleREnd);
}
void sendMode(void)
{
    sendNum(Mode);
    sendNum(GLOBAL_MODE);
    sendNum(ModeEnd);
}

//rx interrupt
#pragma interrupt rx232Isr
void rx232Isr (){
    *rxptr = RCREG ;
    rxptr++;
    if (rxptr == &GLOBAL_RXBUFFER[BUFFSIZE])
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
}
