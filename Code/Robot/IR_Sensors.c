/*******************************************************************************
 * MODULE:  IR_Sensors
 *..............................................................................
 *  Created By:         BEVAN
 *  Created Date:       21.10.2015
 *..............................................................................
 *  Last Edit Date:     23.10.2015
 *  Last Edit By:       BEVAN
 *  Last Edit Details:  cleaned up not tested yet
 *..............................................................................
 * File/Function Property/Usage:
 *  See description below.
 *
 *..............................................................................
 * Description:
 *  Controls the setup and use of IR Sensors
 *  To be used for the robot
 *  Outputs the current distance from the tilt (robot has to be parallel to be accurate)
 *  As well as the state of the 2 IR_Sensors - ERROR,FRONT_IR_CLOSEST,PARALLEL,BACK_IR_CLOSEST (0-3) 
 *
 * Dependancies:
 *  ADC must first be set up
 *  Uses global variables:
 *  IR_SAMPLE_R = sample rate as percent 0 - 100% in increments of 10 
 *  IR_SAMPLE_E = number of samples to use to get an average distance int 0 - 10
 *  Wont function if either IR_SAMPLE_R or IR_SAMPLE_E = 0 
 *  Uses the high priority interrupt with timer0
 * 
 *..............................................................................
 * Current Issues:
 *  not yet tested
 *  
*******************************************************************************/


#include <p18cxxx.h>
#include "AD.h"
#include "IR_Sensors.h"
#include "globals.h"

float F_IR_cm = 0;
float B_IR_cm = 0;
char F_IR_inrange = 0; //0 not in range, 1 in range
char B_IR_inrange = 0;
char IR_state = 0; //uses above defines IR_ERROR, BACK_IR_CLOSEST, IR_PARALLEl, FRONT_IR_CLOSEST

int IR_Sample_count = 0;
unsigned int Front_IR_Buff[IR_BUFFSIZE];
unsigned int Back_IR_Buff[IR_BUFFSIZE];
unsigned int *F_IR;
unsigned int *B_IR;
unsigned int IR_count = 0;
unsigned int avg_F_IR = 0;
unsigned int avg_B_IR = 0;

float m = 0.095;
float b = 0.021;



char Get_IR_state();
float Get_Current_distance();
void IR_Setup(void);
void IR_Interrupt(void);
void IR_Readings(void);
unsigned int getAverage( unsigned int* array,unsigned int size);
char check_in_IR_range(unsigned int avg_value);
float convert_IR_to_cm( unsigned int IR_value);
char determine_IR_parallel(float F_IR, float B_IR);

void high_interrupt(void);
void highPriorityIsr(void);

#pragma code highPriorityInterruptAddress=0x0008
void high_interrupt(void)
{
    _asm GOTO highPriorityIsr _endasm

}

void highPriorityIsr(void)
{  
    if(INTCON1bits.TMR0IF == 1)
    {
        IR_Interrupt();    
        INTCON1bits.TMR0IF = 0;
        INTCONbits.GIEH = 1;
    }    
}

char Get_IR_state()
{
    return IR_state;
}

float Get_Current_distance()
{
    return (F_IR_cm + B_IR_cm)/2;
}

void IR_Setup(void)
{
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
    IR_Sample_count++;
    if(IR_Sample_count == (1+ (100 -GLOBAL_IR_SAMPLE_R)/10)) //100% sample rate = go on sample count 1, 10% sample rate = go after 10 interrupts, 0% sample rate = wont go
    {
        IR_Sample_count = 0;
        switchChannels(FRONT_IR);
        *F_IR = doADC();
        F_IR++;

        switchChannels(BACK_IR);
        *B_IR = doADC();
        B_IR++;

        IR_count++;

        if(IR_count ==  (GLOBAL_IR_SAMPLE_E))
        {
            F_IR = &Front_IR_Buff[0];
            B_IR = &Back_IR_Buff[0];
            IR_count = 0;

            IR_Readings();                    
        }
        
    }
    else if(IR_Sample_count == IR_BUFFSIZE)
    {
        IR_Sample_count = 0;
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


