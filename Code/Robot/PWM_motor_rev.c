/* 
 * File:   PWM_motor.c
 * Author: ldra3_000
 *
 * Created on 29 September 2015, 8:08 PM
 * Edit 18 Oct: Reverse capabilities
 * 
 * Location: Robot
 * Resources: Timer2, CCP1 - PORTC<2>, CCP2 - PORTC<1>, 
 *            Direction enables: PORTC <0>,<3>,<4>,<5> needs pull up/down resistors?
 * NOTE: may need a way to calibrate the 'zero' resistance for the joystick
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f452.h>
#include "configReg452.h"
//#include <p18f4520.h>
//#include "configReg4520.h"

#define bit(x) (1<<(x)) 
#define test_bit(var,pos) ((var) & (1<<(pos)))
//#define change_bit(var,pos,val) ()
//number ^= (-x ^ number) & (1 << n);

unsigned int L_PWM = 0xFE;  // value between 0 (0 duty cycle) and FF (100 duty cycle)
unsigned int L_dir = 1; // 1 is fwd, 0 is back
unsigned int L_inputA = 0;
unsigned int L_inputB = 0;
unsigned int R_PWM = 50;
unsigned int R_dir = 1;
unsigned int R_inputA = 0;
unsigned int R_inputB = 0;
unsigned int Motor_enable = 0; // 1 is enabled, both motors are either enabled or disabled
unsigned int bit_1A = 0; // FOR MOTORS of PORT C
unsigned int bit_1B = 3;
unsigned int bit_2A = 4;
unsigned int bit_2B = 5;
unsigned int bit_enable = 3;
unsigned int length_wheels = 15; // length between wheels cm
unsigned int vel = 0x10; // 'velocity' associated with 50% duty cycle
unsigned int omega  = 128;  // 0 turn to start with
unsigned int max_Dvx=0;
unsigned int duty = 0;

void PWM_motor_setup(void);
void Inputs_motor_setup(void);
void ADC_setup(void);
void direction(void);
void turn(void);


void PWM_motor_setup(void){
    PR2 = 0xFF;         // set period (FF = 610Hz with timer prescaler 16)
    T2CON = 0x07;  // Sets bit 2 to enable timer 2(TMR2), prescaler 16
    CCPR2L = L_PWM;   // select duty cycle
    CCPR1L = R_PWM;   // select duty cycle
    TRISC =0; // output for CCP1-RC<2> and CCP2-RC<1>
    CCP2CON = 0x0C; // Configures CCP2 for PWM mode (CCP2M2 and CCP2M3)
    CCP1CON = 0X0C;     
}

void Inputs_motor_setup(void){
    TRISB &=~bit(bit_enable);
    PORTB |=  (Motor_enable<<bit_enable) ;   // shift motor_enable left by 3 bits
    direction();
    
}

void ADC_setup(void){
    T0CON = 0xC7; //  11000111  Enables TMR0, 8bit with 256 prescale (p103)
    ADCON0 = 0x41;  //0100 0001 Fosc/8, A/D enabled
    ADCON1 = 0x0E;  // 0000 1110 Make RA0 analog input, Left justify, 1 analog channel
}

void direction(void){
    // Left wheel anticlockwise for fwds 01, Right wheel clockwise for fwds 10
    L_inputA = (bit(0)^L_dir) ; // toggle bit
    L_inputB = L_dir;
    R_inputA = R_dir;
    R_inputB = (bit(0)^R_dir) ;// toggle bit
    PORTC |= (R_inputA<<bit_1A)|(R_inputB<<bit_1B)|(L_inputA<<bit_2A)|(L_inputB<<bit_2B); // set direction of wheels
}
// update PWM values
void turn(void){
    // 128 is middle, which is no turn
    if (vel<128){
        max_Dvx = vel;
        duty = (128-vel)*2;
        L_dir = 0;
        R_dir = 0;
    }else if (vel >= 128){
        max_Dvx = 255-vel;
        duty = (vel-128)*2;
        L_dir = 1;
        R_dir = 1;
    }
    if (omega>=128){
        L_PWM = duty - (omega-128)*max_Dvx/128;
    }else {
        L_PWM = duty + (128-omega)*max_Dvx/128;
    }
    //L_PWM = (omega-half)*max_Dvx/half;
    R_PWM = 2*duty-L_PWM;
    CCPR1L = R_PWM;
    CCPR2L = L_PWM;
    direction();            // update direction of wheels
}



void main(void){
    PWM_motor_setup();
    Inputs_motor_setup();
    ADC_setup();

    
    for(;;){
    while(INTCONbits.TMR0IF==0){}   // busy loop
    ADCON0bits.GO=1; 
    while(PIR1bits.ADIF==0){}   // busy loop wait for conversion to finish
    
    turn();  // does the 8 bit to unsigned int work?
    
    }
    
}