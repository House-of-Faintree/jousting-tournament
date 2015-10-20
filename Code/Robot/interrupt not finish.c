#include <stdio.h>
#include <stdlib.h>
#include <p18f452.h>
#include <configReg.h>
#include <string.h>

#define CR 0x0D
#define LF 0x0A

#pragma config  WDT = OFF
#pragma config  LVP = OFF

int count，Signal;
#pragma code lowpriority = 0x18
void low_priority_interrupt(){
    
    if(PIR1bits.CCP1IF== 1)
        _asm goto CCP1_ISR _endasm                
}

void ISR_CCP1_Setup(){
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.CCP1IE = 1;    
    count = 0;
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
    CCPR1H = 0x04;      // value is 1250
    CCPR1L = 0xE2;  
    
    
}

/*setup an interrupt square wave with frequency 1Hz*/
# pragma interrupt CCP1_ISR
void CCP1_ISR(){
    
    count++;
    PIR1bits.CCP1IF = 0;
    TMR1H = 0;
    TMR1L = 0;
    
    if(count == 50){
     Signal = 1;
     PORTBbits.3 = !PORTBbits.3;
     count = 0;
    }
}


void main(void){
    Signal = 0;
    ISR_CCP1_Setup();
    Config_CCP_Timer1();
    while(1);
}














