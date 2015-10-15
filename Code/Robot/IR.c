#include <stdio.h>
#include <stdlib.h>
#include <p18f452.h>
#include <adc.h>
#include <usart.h>
#include "serialcommander.h"
#include "AD.h"

#pragma config  WDT = OFF
#pragma config  LVP = OFF

char IRsensor;
char IRbuffer[25];
char *IRptr;
char rxbuffer[25];         //IRsensor value store to rxbuffer via serial.
int  result;
     
char *txptr;

void main(void) {
    PORTB = 0;
    result = 0;
    ADC_setup();
    switchChannels(2);
    setupSerial();
    rxptr = &GLOBAL_RXBUFFER[0];
    
    IRptr = &IRbuffer[0];

    while(1){

        *IRptr = doADC();
        result = result + *IRptr;
        IRptr++;
        if(IRptr == &IRbuffer[25]){
            result = result/25; 
            IRptr = &IRbuffer[0];
        }
        sendByte(*IRptr);
        sendNewLine();

    }

}


double getAverage( float* array,int size){
    
    int i;
    double avg,sum;
    
    for(i = 0;i<size;++i)
        sum += array[i];
    
    avg = sum / size;
    return avg;
}
