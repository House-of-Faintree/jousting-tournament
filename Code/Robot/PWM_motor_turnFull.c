/* 
 * File:   PWM_motor.c
 * Author: ldra3_000
 *
 * Created on 29 September 2015, 8:08 PM
 * Edit 18 Oct: Reverse capabilities
 * Edit 21 Oct: Turn at full speed (250-255) have 5% omega
 * 
 * Location: Robot
 * Resources: Timer2, CCP1 - PORTC<2>, CCP2 - PORTC<1>, 
 *            Direction enables: PORTC <0>,<3>,<4>,<5> needs pull up/down resistors?
 * NOTE: may need a way to calibrate the 'zero' resistance for the joystick
 */

#include <stdio.h>
#include <stdlib.h>
//#include <p18f452.h>
//#include "configReg452.h"
#include <p18cxxx.h>
#include "Config4520.h"

#define bit(x) (1<<(x)) 
#define test_bit(var,pos) ((var) & (1<<(pos)))

// might be done elsewhere?
unsigned int Motor_enable = 0; // 1 is enabled, both motors are either enabled or disabled
unsigned int bit_enable = 3;

// bits on portC, will make local when finalised
unsigned int bit_1A = 0; // FOR MOTORS of PORT C
unsigned int bit_1B = 3;
unsigned int bit_2A = 4;
unsigned int bit_2B = 5;

// need to be global to store between files
unsigned int vel = 0xFF; // 'velocity' associated with 50% duty cycle
unsigned int omega  = 128;  // 0 turn to start with

//unsigned int max_Dvx=0;
//unsigned int duty = 0;
//unsigned int length_wheels = 15; // length between wheels cm
//unsigned int L_PWM = 0xFE;  // value between 0 (0 duty cycle) and FF (100 duty cycle)
//unsigned int L_dir = 1; // 1 is fwd, 0 is back
//unsigned int L_inputA = 0;
//unsigned int L_inputB = 0;
//unsigned int R_PWM = 50;
//unsigned int R_dir = 1;
//unsigned int R_inputA = 0;
//unsigned int R_inputB = 0;

void PWM_motor_setup(void);
void Inputs_motor_setup(void);
void ADC_setup(void);
void direction(unsigned int, unsigned int);
void turn(void);


//void PWM_motor_setup(void){
//    PR2 = 0xFF;         // set period (FF = 610Hz with timer prescaler 16)
//    T2CON = 0x07;  // Sets bit 2 to enable timer 2(TMR2), prescaler 16
//    CCPR2L = 0;   // select duty cycle - start at zero
//    CCPR1L = 0;   // select duty cycle - start at zero
//    TRISC =0; // output for CCP1-RC<2> and CCP2-RC<1>
//    CCP2CON = 0x0C; // Configures CCP2 for PWM mode (CCP2M2 and CCP2M3)
//    CCP1CON = 0X0C;     
//}

void PWM_motor_setup(void){
    PR2 = 0xFF;         // set period (FF = 610Hz with timer prescaler 16)
    //T2CON = 0x07;  // Sets bit 2 to enable timer 2(TMR2), prescaler 16
    T2CONbits.TMR2ON = 1;
    T2CONbits.T2OUTPS = 0;
    //T2CONbits.T2CKPS = 3;
    T2CONbits.T2CKPS1 = 1;
    CCPR2L = 0;   // select duty cycle - start at zero
    CCPR1L = 0;   // select duty cycle - start at zero
    TRISC = 0; // output for CCP1-RC<2> and CCP2-RC<1>
    //CCP2CON = 0x0C; // Configures CCP2 for PWM mode (CCP2M2 and CCP2M3)
    CCP2CONbits.DC2B = 0; // LSBits duty cycle
   // CCP2CONbits.CCP2M = 15; // PWM mode
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
    CCP1CONbits.DC1B = 0; // duty cycle
    //CCP1CONbits.CCP1M = 15; // PWM mode
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
}

void Inputs_motor_setup(void){
    TRISB &=~bit(bit_enable);
    PORTB |=  (Motor_enable<<bit_enable) ;   // shift motor_enable left by 3 bits
    
}

//void ADC_setup(void){
//    T0CON = 0xC7; //  11000111  Enables TMR0, 8bit with 256 prescale (p103)
//    ADCON0 = 0x41;  //0100 0001 Fosc/8, A/D enabled
//    ADCON1 = 0x0E;  // 0000 1110 Make RA0 analog input, Left justify, 1 analog channel
//}

void ADC_setup(void){
    T0CONbits.TMR0ON = 1; //  11000111  Enables TMR0, 8bit with 256 prescale (p103)
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 7;
    
    ADCON0bits.CHS = 0;  //0100 0001 Fosc/8, A/D enabled
    ADCON0bits.ADON = 1;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.PCFG = 0;
    ADCON2bits.ADFM = 0;
    ADCON2bits.ACQT = 0;
    ADCON2bits.ADCS = 0;
    TRISA = 0xFF;  // inputs
}

void direction(unsigned int L_dir, unsigned int R_dir){
    // Left wheel anticlockwise for fwds 01, Right wheel clockwise for fwds 10
    unsigned int L_inputB = (bit(0)^L_dir) ; // toggle bit
    unsigned int L_inputA = L_dir;
    unsigned int R_inputA = R_dir;
    unsigned int R_inputB = (bit(0)^R_dir) ;// toggle bit
    PORTC = (R_inputA<<bit_1A)|(R_inputB<<bit_1B)|(L_inputA<<bit_2A)|(L_inputB<<bit_2B); // set direction of wheels
}
// update PWM values
void turn(void){
    // 128 is middle, which is no turn
    unsigned int L_dir;     // 1 is fwd, 0 is back
    unsigned int R_dir;     // 1 is fwd, 0 is back
    unsigned int max_Dvx;   // maximum turn achievable at the current velocity
    unsigned int duty;      // mean duty cycle 0-FF
    unsigned int L_PWM;     // duty cycle of left wheel 0-FF
    unsigned int R_PWM;     // duty cycle of right wheel 0-FF
    if (vel<128){      // reverse
        max_Dvx = vel;   
        duty = (127-vel)*2;
        L_dir = 0;
        R_dir = 0;
    }else if (vel >= 128){  // forwards
        max_Dvx = 255-vel;
        duty = (vel-128)*2;
        L_dir = 1;
        R_dir = 1;
    }
    
    if (vel>250||vel<5){  // turn at full speed both directions
        max_Dvx = 5;            // note: will not actually be at the correct velocity  
    }
    
    if (omega>=128 && max_Dvx == 5){   // in reverse/forward turning right near full speed
        L_PWM = 255; // run at max
        R_PWM = 255-(omega-128)*max_Dvx/128;
    } else if(omega<128 && max_Dvx == 5){ // in reverse/forward turning left near full speed
        R_PWM = 255; // run at max
        L_PWM = 255-(128-omega)*max_Dvx/128;
    }else if (omega >=128) {            // right turn
        L_PWM = duty - (omega-128)*max_Dvx/128;
        R_PWM = 2*duty-L_PWM;
    }else if (omega < 128){                               // left turn
        L_PWM = duty + (128-omega)*max_Dvx/128;
        R_PWM = 2*duty-L_PWM;
    }
    
    
    CCPR1L = R_PWM;
    CCPR2L = L_PWM;    //  some sort of error check?
    direction(L_dir,R_dir);            // update direction of wheels
}



void main(void){
    PWM_motor_setup();
    Inputs_motor_setup();
    ADC_setup();

    
    for(;;){
//    while(INTCONbits.TMR0IF==0){}   // busy loop
//    ADCON0bits.GO=1; 
//    while(PIR1bits.ADIF==0){}   // busy loop wait for conversion to finish
//    vel = ADRESH;
    turn();  // does the 8 bit to unsigned int work?
    
    }
    
}