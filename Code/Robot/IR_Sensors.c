/*
 * File:   IR_Sensors.c
 * Author: Bevan
 *
 * Created on 21 October 2015, 10:22 AM
 */


#include <p18f452.h>
#include "configReg452.h"
#include "AD.h"
#inlcude "IR_Sensors.h"

unsigned int Front_IR_Buff[IR_BUFFSIZE];
unsigned int Back_IR_Buff[IR_BUFFSIZE];
unsigned int *F_IR;
unsigned int *B_IR;
unsigned int IR_count = 0;
unsigned int avg_F_IR = 0;
unsigned int avg_B_IR = 0;

float m = 0.095;
float b = 0.021;

void IR_Setup(void);
void IR_Interrupt(void);
void IR_Readings(void);
unsigned int getAverage( unsigned int* array,unsigned int size);
char check_in_IR_range(unsigned int avg_value);
float convert_IR_to_cm( unsigned int IR_value);
char determine_IR_parallel(float F_IR, float B_IR);

void IR_Setup(void)
{
    ADC_setup();
    F_IR = &Front_IR_Buff[0];
    B_IR = &Back_IR_Buff[0];    
    
    //Interrupt setup
    INTCONbits.GIEL = 0; //disable interrupts
    INTCONbits.GIEH = 0;
    
    INTCON1bits.TMR0IE = 1;
    INTCON2bits.TMR0IP = 1;
    
    INTCON1bits.TMR0IF = 0;
    PIR1 = 0x00;
    PIR2 = 0x00;
    INTCONbits.GIEL = 1; //enable Interrupts
    INTCONbits.GIEH = 1; 
}

void IR_Interrupt(void)
{
    switchChannels(FRONT_IR);
    *F_IR = doADC();
    F_IR++;
    
    switchChannels(BACK_IR);
    *B_IR = doADC();
    B_IR++;
    
    IR_count++;
    
    if(IR_count == IR_BUFFSIZE)
    {
        F_IR = &Front_IR_Buff[0];
        B_IR = &Back_IR_Buff[0];
        IR_count = 0;
        
        IR_readings();                    
    }    
}

void IR_Readings(void)
{
    avg_F_IR = getAverage(Front_IR_Buff,IR_BUFFSIZE); 
    avg_B_IR = getAverage(Back_IR_Buff,IR_BUFFSIZE);
            
    F_IR_inrange = check_in_IR_range(avg_F_IR);
    B_IR_inrange = check_in_IR_range(avg_B_IR);
            
    if(F_IR_inrange == 1)
    {
        F_IR_cm = convert_IR_to_cm(avg_F_IR);
    }
    if(B_IR_inrange == 1)
    {
        B_IR_cm = convert_IR_to_cm(avg_B_IR);
    }                
                                  
    IR_state = determine_IR_parallel(F_IR_cm,B_IR_cm);                  
}


unsigned int getAverage( unsigned int* array,unsigned int size){
    
    int i;
    unsigned int avg = 0;
    unsigned long sum = 0;
    
    for(i = 0;i<size;++i)
        sum += array[i];
    
    avg = sum / size;
    return avg;
}

char check_in_IR_range(unsigned int avg_value)
{
    if((avg_value <= UPPER_IR_LIMIT) && (avg_value >= LOWER_IR_LIMIT))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

float convert_IR_to_cm( unsigned int IR_value)
{
    float cm_value = (1/((m*IR_value*5/255)-b))-0.42;    
    return cm_value;
}

char determine_IR_parallel(float F_IR, float B_IR)
{
    char state = 0;
    if((F_IR_inrange == 1) && ((B_IR_inrange == 0) || ((F_IR - B_IR) > PARALLEL_ACCURACY)))
    {
        state = FRONT_IR_CLOSEST;
    }
    else if((B_IR_inrange == 1) && ((F_IR_inrange == 0) || ((B_IR - F_IR) > PARALLEL_ACCURACY)))
    {
        state = BACK_IR_CLOSEST;
    }
    else if((F_IR_inrange == 0) && (B_IR_inrange == 0))
    {
        state = IR_ERROR;
    }
    else
    {
        state = IR_PARALLEL;
    }
    
    return state;
}


