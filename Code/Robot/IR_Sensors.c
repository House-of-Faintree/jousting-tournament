/*
 * File:   IR_Sensors.c
 * Author: Bevan
 *
 * Created on 20 October 2015, 10:22 AM
 */


#include <p18f452.h>
#include "configReg452.h"
#include "AD.h"

#define FRONT_IR 0
#define BACK_IR 1

#define IR_BUFFSIZE 5

#define UPPER_IR_LIMIT 125 //4 cm 2.45V
#define LOWER_IR_LIMIT 28 //30cm 0.56V

#define PARALLEL_ACCURACY 1 //in cm

#define IR_ERROR 0
#define BACK_IR_CLOSEST 1
#define IR_PARALLEL 2
#define FRONT_IR_CLOSEST 3

#define NEUTRAL 128
#define TURN_RIGHT 255
#define TURN_LEFT 0

unsigned int Front_IR_Buff[IR_BUFFSIZE];
unsigned int Back_IR_Buff[IR_BUFFSIZE];
unsigned int *F_IR;
unsigned int *B_IR;
unsigned int avg_F_IR = 0;
unsigned int avg_B_IR = 0;
float F_IR_cm = 0;
float B_IR_cm = 0;
char F_IR_inrange = 0; //0 not in range, 1 in range
char B_IR_inrange = 0;
char IR_state = 0; //parallel or not
unsigned int IR_count = 0;

float m = 0.095;
float b = 0.021;

int turn = NEUTRAL;
int auto_turn = NEUTRAL;

void high_interrupt(void);
void highPriorityIsr(void);

#pragma code highPriorityInterruptAddress=0x0008
void high_interrupt(void)
{
    _asm GOTO highPriorityIsr _endasm

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
        PORTB = 0x00;
        PORTBbits.RB1 = 1;
    }
    else if((B_IR_inrange == 1) && ((F_IR_inrange == 0) || ((B_IR - F_IR) > PARALLEL_ACCURACY)))
    {
        state = BACK_IR_CLOSEST;
        PORTB = 0x00;
        PORTBbits.RB2 = 1;
    }
    else if((F_IR_inrange == 0) && (B_IR_inrange == 0))
    {
        state = IR_ERROR;
        PORTB = 0x00;
        PORTBbits.RB3 = 1;
    }
    else
    {
        state = IR_PARALLEL;
        PORTB = 0x00;
        PORTBbits.RB0 = 1; //test 
    }
    
    return state;
}

void IR_readings(void)
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

void highPriorityIsr(void)
{  
    if(INTCON1bits.TMR0IF == 1)
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
        
        INTCON1bits.TMR0IF = 0;
        INTCONbits.GIEH = 1;
    }    
}

void IR_Setup(void)
{
    ADC_setup();
    F_IR = &Front_IR_Buff[0];
    B_IR = &Back_IR_Buff[0];
    TRISB = 0x00;
    PORTB = 0x00; //test
    
    
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

void main(void) {
    IR_Setup();
    while(1);
}

/*
void IR_Control (void)
{
   while( !(NEUTRAL -5 <= turn <= NEUTRAL + 5) && !(IR_state == IR_PARALLEL) )
   {
       if(IR_state == BACK_IR_CLOSEST)
       {
           auto_turn = TURN_LEFT;
       }
       else if(IR_state = FRONT_IR_CLOSEST)
       {
           auto_turn = TURN_RIGHT;
       }
       else
       {
           auto_turn = NEUTRAL;
       }
       
       //turn(auto_turn); 
   }
}
*/
