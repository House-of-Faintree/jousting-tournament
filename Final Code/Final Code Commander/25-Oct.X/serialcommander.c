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


#include "serialCommander.h"
#include "GlobalVariblesCommander.h"
#include  <delays.h>

//char ByteTest = 0x78;
//char StrTest[] = "week12!";
unsigned char *rxptr = GLOBAL_RXBUFFER;


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
    //Delay10TCYx(1);
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

void sendMaxSpeed(void)
{
    sendNum(MaxSpeed);
    delay10us(5);
    sendNum(GLOBAL_MAX_SPEED);
    delay10us(5);
    sendNum(MaxSpeedEnd);
}
void sendMaxYaw(void)
{
    sendNum(MaxYaw);
    delay10us(5);
    sendNum(GLOBAL_MAX_YAW);
    delay10us(5);
    sendNum(MaxYawEnd);
}
void sendIRsampleR(void)
{
    sendNum(IRsampleR);
    delay10us(5);
    sendNum(GLOBAL_IRsampleR);
    delay10us(5);
    sendNum(IRsampleREnd);
}
void sendMode(void)
{
    sendNum(Mode);
    delay10us(5);
    sendNum(GLOBAL_MODE);
    delay10us(5);
    sendNum(ModeEnd);
}
void sendHello(void)
{
    sendNum(Hello);
    delay10us(5);
    sendNum(HelloEnd);
    delay10us(5);
}

void sendVelocity(void)
{
    if (GLOBAL_VELOCITY > 230)
    {
        GLOBAL_VELOCITY = 230;
    }
    sendNum(Velocity);
    delay10us(5);

    sendNum(GLOBAL_VELOCITY);
    delay10us(5);

    sendNum(VelocityEnd);
}

void sendOmega(void)
{
    if (GLOBAL_OMEGA > 230)
    {
        GLOBAL_OMEGA = 230;
    }
    sendNum(Turn);
    delay10us(5);

    sendNum(GLOBAL_OMEGA);
    delay10us(5);

    sendNum(TurnEnd);
}

void sendRun (void)
{
    sendNum(RunStart);
    delay10us(5);
    sendNum(GLOBAL_RUN);
    delay10us(5);
    sendNum(RunEnd);
}

void HandShake(void)
{
    sendMaxSpeed();
    sendMaxYaw();
    sendIRsampleR();
    sendMode();
    sendVelocity();
    sendOmega();
    sendRun();
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
  
  TRISBbits.RB4 = 0;
  
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

//void ADCInit(void)
//{
//    T0CON = 0xC7;
//    ADCON0 = 0x41;
//    ADCON1 = 0x0E;
//}

void TMR0Init(void)
{
    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 0;
    T0CONbits.PSA = 1;
    T0CONbits.T0CS = 0;
    INTCON2bits.TMR0IP = 0;
    INTCONbits.TMR0IE = 0;
    INTCONbits.TMR0IF = 0;
}


void rx232Isr (void)
{    
    if (RCREG == Mode)
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
    else if (RCREG == Hello)
    {
        sendNum(Hello);
        PORTBbits.RB4 = 1;
        sendNum(HelloEnd);
    }
    else if (RCREG == HelloEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == Velocity)
    {
        rxptr = &GLOBAL_VELOCITY;
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
    else if (RCREG == IRraw)
    {
        rxptr = &GLOBAL_IRRAW;
    }
    else if (RCREG == IRrawEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];    
    }
    else 
    {
        *rxptr = RCREG;
        rxptr++;
    }
    *rxptr = RCREG ;
    
    
    if (RCREG == '\r'||rxptr == &GLOBAL_RXBUFFER[BUFFSIZE])
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
}

void TMR0Isr (){
    
    INTCONbits.TMR0IE = 0;
    
    
    INTCONbits.TMR0IF = 0;    
    //sendString(StrTest);
    INTCONbits.TMR0IE = 1;

    
}
