/*******************************************************************************
 * MODULE:  GLOBAL DEFINITIONS
 *..............................................................................
 *  Created By:         ZIJI AN
 *  Created Date:       22.10.2015
 *..............................................................................
 *  Last Edit Date:     22.10.2015
 *  Last Edit By:       ZIJI
 *  Last Edit Details:  Created this file for serial communication on robot
 *..............................................................................
 * File/Function Property/Usage:
 *  See description below.
 *..............................................................................
 * Description:
 *  This file contains the definition of all functions used in serial communication
 *
 * Dependancies:
 *..............................................................................
 * Current Issues:
*******************************************************************************/


#include "serialRobot.h"
#include "GlobalVariblesRobot.h"
#include <p18f4520.h>

//char ByteTest = 0x78;
//char StrTest[] = "week12!";
unsigned char *rxptr = GLOBAL_RXBUFFER;
unsigned char data;
unsigned int Serial_count = 0;
unsigned int Timer_count = 0;


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
  IPR1bits.RCIP = 1;
  RCONbits.IPEN = 1;
  
  TRISDbits.RD1 = 0;
  TRISDbits.RD2 = 0;
  
  PORTDbits.RD1 = 0;
  PORTDbits.RD2 = 0;
  
  TRISCbits.RC6 = 0;              //set rc6 as output
  TRISCbits.RC7 = 1;              //set rc7 as input
  TXSTAbits.SYNC = 0;             //Set to asynchronous mode
  TXSTAbits.BRGH = 1;             //Set baudrate generator to high
  TXSTAbits.TXEN = 1;
  //BAUDCONbits.BRG16 = 0;
  SPBRG = 64;                     //Set baudrate to 9600 for 10Mhz
  RCSTAbits.SPEN = 1;             //enable serial port
  RCSTAbits.CREN = 1;             //Enables receiver in continuous mode
  PIE1bits.RCIE = 1;
  //TXSTA = 0x24;                 //All settings up TXSTA done
  INTCONbits.GIE = 1;
  INTCONbits.PEIE = 1;
}

//void sendVelocity(void) 
//{ 
//    sendNum(Velocity); 
//    sendNum(GLOBAL_VELOCITY); 
//    sendNum(VelocityEnd); 
//} 
//void sendOmega(void)
//{
//    sendNum(Turn); 
//    sendNum(GLOBAL_VELOCITY); 
//    sendNum(TurnEnd); 
//}
//
void sendIRraw(void)
{
    sendNum(IRraw); 
    sendNum(GLOBAL_IRRAW); 
    sendNum(IRrawend); 
}


//void TMR0Init(void)
//{
//    T0CONbits.TMR0ON = 1;
//    T0CONbits.T08BIT = 0;
//    T0CONbits.PSA = 1;
//    
//    INTCONbits.GIE = 0;
//    T0CONbits.T0CS = 0;
//    INTCON2bits.TMR0IP = 1;
//    INTCONbits.TMR0IE = 1;
//    INTCONbits.TMR0IF = 0;
//    INTCONbits.GIE = 1;
//}
//
//void TMR1Init(void)
//{
//    T1CONbits.RD16 = 1;
//    T1CONbits.TMR1CS = 0;
//    T1CONbits.T1SYNC = 0;
//    T1CONbits.T1OSCEN = 0;
//    T1CONbits.T1CKPS0 = 0;
//    T1CONbits.T1CKPS1 = 0;
//    T1CONbits.TMR1ON = 1;
//    PIR1bits.TMR1IF = 0;
//    PIE1bits.TMR1IE = 1;
//    IPR1bits.TMR1IP = 0;
//    
//}
// look at pointer rxptr
void rx232Isr (void)
{    
    Serial_count++;
    
    if (RCREG == Hello)
    {
        GLOBAL_FAIL_COUNTER = 0;
    }
    else if (RCREG == HelloEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == Mode)
    {
        rxptr = &GLOBAL_MODE;
    }
    else if (RCREG == ModeEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == MaxSpeed)
    {
        rxptr = &GLOBAL_MAX_SPEED;
    }
    else if (RCREG == MaxSpeedEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == MaxYaw)
    {
        rxptr = &GLOBAL_MAX_YAW;
    }
    else if (RCREG == MaxYawEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    
    else if (RCREG == Velocity)
    {
        rxptr = &GLOBAL_VELOCITY;
        //GLOBAL_VELOCITY = RCREG;
    }
    else if (RCREG == VelocityEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == IRsampleR)
    {
        rxptr = &GLOBAL_IRsampleR;
    }
    else if (RCREG == IRsampleREnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == Turn)
    {
        rxptr = &GLOBAL_OMEGA;
    }
    else if (RCREG == TurnEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == RunStart)
    {
        rxptr = &GLOBAL_RUN;
    }
    else if (RCREG == RunEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    
    else 
    {
        *rxptr = RCREG;
        rxptr++;
    }
    
    //*rxptr = RCREG ;


    
    
    if (RCREG == '\r'||rxptr == &GLOBAL_RXBUFFER[BUFFSIZE])
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    //receiveAck(RCREG);
    
}

//void TMR0Isr (){
//    
////    sendNum(Hello);
////    sendNum(HelloEnd);
////    GLOBAL_FAIL_COUNTER++;
////    if (GLOBAL_FAIL_COUNTER > 200 )
////    {
////        PORTBbits.RB4 ^= PORTBbits.RB4;
////    }
////    //sendString(StrTest);
//    
//    
//    
//    if(Serial_count == Timer_count)
//    {
//        GLOBAL_RUN = 0;
//    }
//    
//    Timer_count = Serial_count;
//    
//    INTCONbits.TMR0IF = 0;
//    
//}



//void TMR1Isr(void)
//{
//    PIR1bits.TMR1IF = 0;
//    sendNum(Hello);
//    sendNum(HelloEnd);
//    GLOBAL_FAIL_COUNTER++;
//    if (GLOBAL_FAIL_COUNTER > 200 )
//    {
//        PORTBbits.RB4 ^= PORTBbits.RB4;
//    }
//}

////void receiveAck(char data){
////    
////    sendByte(data);
////   //recieve data
////   //send back data
////   //return 0
////}
////
////int transmitAck(char recData, char senData){
////    //if receive == transmit
////    //return 1
////    if (recData == senData){
////        return 1;
////    }
////    else
////        return 0;
////
