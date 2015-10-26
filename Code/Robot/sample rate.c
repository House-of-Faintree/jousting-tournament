#include <stdio.h>
#include <stdlib.h>
#include <p18f452.h>
#include <timers.h>
#include "AD.h"
#include "IR_Sensors.h"
#include "ConfigRegs.h"
#include <adc.h>
#include "globals.h"

int count,sample;
int loop;
unsigned int averageDigital;

int IRbuffer[IR_BUFFSIZE], *IRptr;
float cm;

void Timer1ISR(void);
#pragma code lowInterrupt = 0x0018
void low_interrupt(void){ 
   if(PIR1bits.TMR1IF == 1)
     _asm goto Timer1ISR _endasm                    
}
#pragma code

void valueInitial(){
    IRptr = IRbuffer;
    cm = 0;
    averageDigital = 0;                //initial the average value to 0
    count = 0;                  //set the counting value to 0
    sample = 1;                //user input sample rate      
    loop = (int)15/sample;      //determine how many times of overflow require
    if(loop <= 1)
        loop = 1;
}
void LEDsetup(){
    PORTB = 0x00;               
    TRISB = 0x00;
}

void Timer1Setup(){    
    T1CONbits.RD16 = 1;         // 16 bit operation
    T1CONbits.T1CKPS1 = 0;      // 1:1 pre-scale
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
    //IR_Setup();
    ADC_setup();
    switchChannels(2);
    valueInitial();         // some value use for initialization
    Timer1Setup();
    ConfigTimer1ISR();
    LEDsetup();             // use for debug and test
    
    while(1){        

        *IRptr = doADC();        
        IRptr++;        
        
         if(IRptr == &IRbuffer[IR_BUFFSIZE]){           
             IRptr = &IRbuffer[IR_BUFFSIZE];
         }
        
        if(count == loop){
            count = 0;            
            averageDigital = getAverage(IRbuffer,IR_BUFFSIZE);
            // trigger the sending message signal
            cm = convert_IR_to_cm( averageDigital);
            
            
        }
    } 
}

# pragma interruptlow Timer1ISR
void Timer1ISR(void){                 
    PIR1bits.TMR1IF = 0;            //clear TMR1 register overflowed bit   
    count++;                        //increase the count
}
