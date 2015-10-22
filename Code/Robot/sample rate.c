#include <stdio.h>
#include <stdlib.h>
#include <p18f452.h>
#include "ConfigRegs.h"
#include <timers.h>

int count,sample;
int loop;

void lowPriorityIsr(void);
#pragma code lowPriorityInterruptAddress = 0x0018
void low_interrupt(void){ 
   if(PIR1bits.TMR1IF == 1)
     _asm goto lowPriorityIsr _endasm                    
}
#pragma code

void valueInitial(){
    count = 0;                  //set the counting value to 0
    sample = 8;                 //user input sample rate  
    PORTB = 0x00;               
    TRISB = 0x00;
    loop = (int)15/sample;
    if(loop <= 1)
        loop = 1;
}

void Timer1Setup(){    
    T1CONbits.RD16 = 1;         // 16 bit operation
    T1CONbits.T1CKPS1 = 0;      //1:1 pre-scale
    T1CONbits.T1CKPS0 = 0;
    T1CONbits.TMR1ON = 1;       //enable timer1
    
}
void ConfigTimer1ISR(){
    INTCONbits.GIE = 1;     //enable all unmasked interrupts
    INTCONbits.PEIE = 1;    //enable all low priority interrupts
    PIR1bits.TMR1IF = 0;    //initialize TMR1 register overflowed bit
    PIE1bits.TMR1IE = 1;    //enable the TMR1 overflow interrupt
    IPR1bits.TMR1IP = 0;    //TMR1 overflow Interrupt Priority bit, 0 = Low priority
    RCONbits.IPEN = 1;      //enable priority levels on interrupts
    
}

void main(void){
    valueInitial();
    Timer1Setup();
    ConfigTimer1ISR();
   
    while(1){   
        if(count == loop){
            count = 0;
            PORTB = !PORTB;
        }
    } 
}

# pragma interruptlow lowPriorityIsr
void lowPriorityIsr(void){       
           
    PIR1bits.TMR1IF = 0;            //clear TMR1 register overflowed bit   
    count++;    
}
