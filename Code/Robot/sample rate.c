#include <stdio.h>
#include <stdlib.h>
#include <p18f452.h>
#include "ConfigRegs.h"

int count,sample;
int loop;

void valueInitial(){
    count = 0;                  //set the counting value to 0
    sample = 2;
    
    PORTB = 0x00;
    TRISB = 0x00;
    loop = (int)15/sample;
}
void Timer1Setup(){    
    T1CONbits.RD16 = 1;         // 16 bit operation
    T1CONbits.T1CKPS1 = 0;      //1:1 prescale
    T1CONbits.T1CKPS0 = 0;
    T1CONbits.TMR1ON = 1;       //enable timer1;
    
}
void ConfigTimer1ISR(){
    INTCONbits.GIE = 1;     //enable all unmasked interrupts
    INTCONbits.PEIE = 1;    // enable all low priority interrupts
    PIE1bits.TMR1IE = 1;    //enable the TMR1 overflow interrupt
    IPR1bits.TMR1IP = 0;    // TMR1 overflow Interrupt Priority bit, 0 = Low priority
    
}

void Timer1isr(void);
#pragma code lowpriority = 0x0018
void low_interrupt(void){ 
    if(PIR1bits.TMR1IF == 1)
     _asm GOTO Timer1isr _endasm                    
}
#pragma code

void main(void){
    valueInitial();
    Timer1Setup();
    ConfigTimer1ISR();
    while(1){
        
        if(count == loop){
            
            count = 0;
           // PORTBbits.RB0=!PORTBbits.RB0;     // test purpose only
        }
    } 
}

# pragma interrupt Timer1isr
void Timer1isr(void){   
    
               //if timer1 is overflow
        PIR1bits.TMR1IF = 0;            //clear TMR1 register overflowed bit
        count++;    
       // PORTB = 0xFF;
   
  
}

