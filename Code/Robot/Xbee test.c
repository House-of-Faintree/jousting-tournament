/*-----------------------File name---------------------*/
/*Created By: ZIJI*/
/*Last Edit by: ZIJI*/
/*Create Date: 7/10/2015*/
/*last modify date: 14/10/2015*/

/*This file is to test the XBee module, and make simple implementation of that*/
/*The final function we will be using for RF transmission called RFsend(), it */
/*takes a input of a pointer point to the stuff you would like to transmit(the*/
/*first character of a string for instance), then it will use serial interrupt to transmit the stuff out*/
/*Before the transmission, a function called Clear_Rx_Buffer() should be called*/
/*to clear everything within the receive buffer and reset the pointer*/
/*For receiving, it will be interrupt driven, anything received will be stored*/
/*into a string called GLOBAL_RXBUFFER(name to be determined), and please read the*/
/*data from this buffer asap as it will be cleared at the beginning of next transmit*/

/*-------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <adc.h>
#include <p18f4520.h>
#include <usart.h>
#include <string.h>

#define CR 0x0D
#define LF 0x0A
#define INIT_IPR1 0b00000000
#define INIT_SPBRG 25
#define BUFFSIZE    0x2f

#pragma config  WDT = OFF
#pragma config  LVP = OFF

void tx232Isr();
void rx232Isr();
#pragma code lowpriority = 0x18
void low_priority_interrupt(){

    if(PIR1bits.RCIF == 1)

        _asm goto rx232Isr _endasm

    else if (PIR1bits.TXIF== 1)

        _asm goto tx232Isr _endasm

}
#pragma code

unsigned char array[] = "JACE";
unsigned char number = 123;
unsigned char GLOBAL_RXBUFFER[BUFFSIZE]={0};
unsigned char *txptr;
unsigned char *rxptr = GLOBAL_RXBUFFER;

//serial setup
void SerialSetup(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    IPR1 = INIT_IPR1;
    RCONbits.IPEN = 1;
    SPBRG = INIT_SPBRG;
    TXSTAbits.BRGH = 1;
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 1;
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    PIE1bits.RCIE = 1;
}
//enable and disable the transmission.
void enable_tx(void){
    TXSTAbits.TXEN = 1;
    PIE1bits.TXIE = 1;
}
void disable_tx(void){
    TXSTAbits.TXEN = 0;
    PIE1bits.TXIE = 0;
}
void Clear_Rx_Buffer(void){
    unsigned int i;
    for ( i = 0;i <= BUFFSIZE ;i++ )
    {
       GLOBAL_RXBUFFER[i] = 0;
    }
    rxptr =& GLOBAL_RXBUFFER[0];
    
}
void RFSend (unsigned char *str)
// transmit function, it only transmit a string, so make sure the input is a pointer to a string
// or we can come up with a way to change a variable into a string
{
    txptr = str;
    enable_tx();
}

void main(){
    SerialSetup();
    RFSend(array);
    while(1)
    {
        if (PORTAbits.RA4 == 0)
        {
            Clear_Rx_Buffer();
        }
    }
}

//tx interrupt
#pragma interrupt tx232Isr
void tx232Isr (){

    while (BusyUSART()){}
    TXREG = *txptr;
    txptr++;
    if (TXREG == 0)
        disable_tx();

}
//rx interrupt
#pragma interrupt rx232Isr
void rx232Isr (){
    *rxptr = RCREG ;
    rxptr++;
    if (rxptr == &GLOBAL_RXBUFFER[BUFFSIZE])
    {
        rxptr =& GLOBAL_RXBUFFER[0];
    }
}
