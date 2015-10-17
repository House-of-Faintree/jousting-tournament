
#include <stdio.h>
#include <stdlib.h>
#include <p18f452.h>
#include <configReg.h>
#include <string.h>

#define CR 0x0D
#define LF 0x0A

#pragma config  WDT = OFF
#pragma config  LVP = OFF

#pragma code lowpriority = 0x18
void low_priority_interrupt(){
    
    if(PIR1bits.CCP1IF== 1)
        _asm goto CCP1_ISR _endasm                
}

void ISR_CCP1_Setup(){
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.CCP1IE = 1;    
    
}

void Config_CCP_Timer1(){
    T1CONbits.RD16 = 1;
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.TICKPS0 = 1;
    T1CONbits.TMR1ON = 1;
    TMR1L = 0;
    TMR1H = 0;
    CCP1CON = 0b00001010
    
    PORTB = 0;
    TRISC = 0;
    CCPR1H = 
    
    
    
}


void main(void){
    
    while(1);
}














