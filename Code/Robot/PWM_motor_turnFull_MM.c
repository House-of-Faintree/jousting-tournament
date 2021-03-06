/* 
 * File:   PWM_motor.c
 * Author: ldra3_000
 *
 * Created on 29 September 2015, 8:08 PM
 * Edit 18 Oct: Reverse capabilities
 * Edit 21 Oct: Turn at full speed (250-255) have 5% localOmega
 * 
 * Location: Robot
 * Resources: Timer2, CCP1 - PORTC<2>, CCP2 - PORTC<1>, 
 *            Direction enables: PORTC <0>,<3>,<4>,<5> needs pull up/down resistors?
 * NOTE: may need a way to calibrate the 'zero' resistance for the joystick
 */

#include  "PWM_Motor.h"
#include  "GlobalVariblesRobot.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <p18f452.h>
//#include "configReg452.h"
//#include <p18f4520.h>
//#include "ConfigRegs18f4520.h"

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
unsigned int Motor_enable = 0; // 1 is enabled, both motors are either enabled or disabled
unsigned int bit_enable = 3;

// bits on portC, will make local when finalised
unsigned int bit_1A = 0; // FOR MOTORS of PORT C
unsigned int bit_1B = 3;
unsigned int bit_2A = 4;
unsigned int bit_2B = 5;


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

void direction(unsigned int L_dir, unsigned int R_dir){
    // Left wheel anticlockwise for fwds 01, Right wheel clockwise for fwds 10
//    unsigned int L_inputB = (bit(0)^L_dir) ; // toggle bit
//    unsigned int L_inputA = L_dir;
//    unsigned int R_inputA = R_dir;
//    unsigned int R_inputB = (bit(0)^R_dir) ;// toggle bit
    unsigned int L_inputB = (bit(0)^L_dir) ; // toggle bit
    unsigned int L_inputA = L_dir;
    unsigned int R_inputA = (bit(0)^R_dir);// toggle bit;
    unsigned int R_inputB = R_dir;
    PORTC = (R_inputA<<bit_1A)|(R_inputB<<bit_1B)|(L_inputA<<bit_2A)|(L_inputB<<bit_2B); // set direction of wheels
}
// update PWM values
void turn(void){
    // 128 is middle, which is no turn
        
    unsigned int L_dir;     // 1 is fwd, 0 is back
    unsigned int R_dir;     // 1 is fwd, 0 is back
    unsigned int max_Dvx;   // maximum turn achievable at the current localVelocity
    unsigned int duty;      // mean duty cycle 0-FF
    unsigned float L_PWM;     // duty cycle of left wheel 0-FF
    unsigned float R_PWM;     // duty cycle of right wheel 0-FF    unsigned int localVel = (unsigned int) GLOBAL_VELOCITY;
    unsigned int localVel;
    unsigned int localOmega;
    //unsigned int localOmega = (unsigned int) GLOBAL_OMEGA;
//    unsigned int localOmega = 0;
//    localVel =(unsigned int) GLOBAL_VELOCITY;
//    localOmega = (unsigned int) GLOBAL_OMEGA;
    
    if (GLOBAL_RUN == 0)
    {
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 0;
    }
    else if (GLOBAL_RUN == 1)
    {
        PORTDbits.RD1 = 1;
        PORTDbits.RD2 = 1;
    }
    
    localVel = GLOBAL_VELOCITY;
    localOmega = GLOBAL_OMEGA;
    
    if(localVel == 238){
        localVel = 255;
    }
    else if (localVel < 17){
        localVel = 0;
    }
    if(localOmega >= 238){
        localOmega = 255;
    }
    else if(localOmega < 17){
        localOmega = 0;
    }
    
    
//    if (localVel<120){      // reverse
//        max_Dvx = localVel;   
//        duty = (128-localVel)*2;
//        L_dir = 0;
//        R_dir = 0;
//    }else if (localVel >= 135){  // forwards
//        max_Dvx = 255-localVel;
//        duty = (localVel-128)*2;
//        L_dir = 1;
//        R_dir = 1;
//    }
//    
//    else{
//        duty = 0;
//        L_dir = 1;
//        R_dir = 1;
//        max_Dvx = 0;
//    }
//   
//    
//    if ((localVel>250)||(localVel<5)){  // turn at full speed both directions
//        max_Dvx = 30;            // note: will not actually be at the correct localVelocity  
//    } 
//    if (localOmega>=135 && max_Dvx == 30){   // in reverse/forward turning right near full speed
//        L_PWM = 255; // run at max
//        R_PWM = 255-(localOmega-128)*max_Dvx/128;
//    } else if(localOmega<120 && max_Dvx == 30){ // in reverse/forward turning left near full speed
//        R_PWM = 255; // run at max
//        L_PWM = 255-(localOmega-128)*max_Dvx/128;
//    }
//    
//    else if (( localOmega >=135)&&(duty > 0)) {            // right turn
//        L_PWM = duty + (( localOmega-128)*max_Dvx)/128; //Use bitshift instead
//        R_PWM = 2*duty-L_PWM;
//    }else if(( localOmega <= 120) &&(duty > 0)){                               // left turn
//        L_PWM = duty - ((128- localOmega)*max_Dvx)/128; // Use Bitshift instead
//        R_PWM = 2*duty-L_PWM;
//    }
//    else{
//        L_PWM = duty;
//        R_PWM = duty;
//    }
    
    if ((localVel > 135) && (localOmega < 254)){
        R_PWM = 255;
        L_PWM = 255;
        
        L_dir = 1;
        R_dir = 1;   
    }
    
    else if (localVel < 121){
        R_PWM = 255;
        L_PWM = 255;
        
        L_dir = 0;
        R_dir = 0;   
    }
    
    else if (localOmega > 135){
        R_PWM = 255;
        L_PWM = 128;
        
        L_dir = 1;
        R_dir = 1;   
    }
    
    else if (localOmega < 121){
        R_PWM = 128;
        L_PWM = 255;
        
        L_dir = 1;
        R_dir = 1;   
    }
    
    else
    {
        R_PWM = 0;
        L_PWM = 0;
                
    }
    
    
    CCPR1L = (char) R_PWM;
    CCPR2L = (char) L_PWM;    //  some sort of error check?

    direction(L_dir,R_dir);            // update direction of wheels
}



/*
//void moveForward(int y){
//    if (y > 135){
//        R_PWM = 255;
//        L_PWM = 255;
//        
//        L_dir = 1;
//        R_dir = 1;   
//    }
//}
//
//void moveBackward(int y){
//    if (y < 121){
//        R_PWM = 255;
//        L_PWM = 255;
//        
//        L_dir = 0;
//        R_dir = 0;   
//    }
//}
//
//void moveLeft(int x){
//    if (x > 135){
//        R_PWM = 255;
//        L_PWM = 128;
//        
//        L_dir = 1;
//        R_dir = 1;   
//    }
//}
//
//void moveRight(int x){
//    if (x < 121){
//        R_PWM = 255;
//        L_PWM = 255;
//        
//        L_dir = 1;
//        R_dir = 1;   
//    }
//}
*/
