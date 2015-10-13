/*
 * File:   PWM_motor.c
 * Author: ldra3_000
 *
 * Created on 29 September 2015, 8:08 PM
 * Location: Robot
 * Resources: Timer2, CCP1 - PORTC<2>, CCP2 - PORTC<1>
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
unsigned int max_Dvx=0;
unsigned int full = 255;
unsigned int half = 128;
unsigned int two = 2;

void PWM_motor_setup(void);
void Inputs_motor_setup(void);
void ADC_setup(void);
void direction(void);
void turn(unsigned int);


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
    PORTC |= (R_inputA<<bit_1A)|(R_inputB<<bit_1B)|(L_inputA<<bit_2A)|(L_inputB<<bit_2B); // set direction of wheels
}

void ADC_setup(void){
    T0CON = 0xC7; //  11000111  Enables TMR0, 8bit with 256 prescale (p103)
    ADCON0 = 0x41;  //0100 0001 Fosc/8, A/D enabled
    ADCON1 = 0x0E;  // 0000 1110 Make RA0 analog input, Left justify, 1 analog channel
}

void direction(void){
    //number ^= (-x ^ number) & (1 << n);
    L_inputA = L_dir|(bit(0)^L_dir) ; // toggle bit
    L_inputB = L_dir;
    R_inputA = R_dir;
    R_inputB = R_dir|(bit(0)^R_dir) ;// toggle bit

}
// update PWM values
void turn(unsigned int POT_val){
    // 128 is middle, which is no turn
    if (vel<=half){
        max_Dvx = vel;
    }else if (vel > half){
        max_Dvx = full-vel;
    }
    if (POT_val>=half){
        L_PWM = vel - (POT_val-half)*max_Dvx/half;
    }else {
        L_PWM = vel + (half-POT_val)*max_Dvx/half;
    }
    //L_PWM = (POT_val-half)*max_Dvx/half;
    R_PWM = two*vel-L_PWM;
    CCPR1L = R_PWM;
      CCPR2L = L_PWM;
}



void main(void){
    PWM_motor_setup();
    Inputs_motor_setup();
    ADC_setup();


    for(;;){
    while(INTCONbits.TMR0IF==0){}   // busy loop
    ADCON0bits.GO=1;
    while(PIR1bits.ADIF==0){}   // busy loop wait for conversion to finish

    turn(ADRESH);  // does the 8 bit to unsigned int work?

    }

}
