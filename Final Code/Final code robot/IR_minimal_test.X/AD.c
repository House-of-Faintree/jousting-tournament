#include <p18cxxx.h>
#include "AD.h"


void ADC_setup(void){
    T0CON = 0xC7; //  11000111  Enables TMR0, 8bit with 256 prescale (p103)
    ADCON0 = 0x01;  //A/D enabled, Channel AN0 by default
    ADCON1 = 0x00;  // 0000 0000 Make All channels analog input
    ADCON2 = ADCON2 & 0x38; //FOSC/2, left justify
    while(INTCONbits.TMR0IF==0){} //wait for ADC to charge up
}

unsigned char doADC(void){
    ADCON0bits.GO = 1;
    while(PIR1bits.ADIF == 0){}   // wait for conversion to finish
    return ADRESH;
}

void switchChannels(int channel){
    ADCON0 = ADCON0 & 0xc3; //mask adcon with 1100 0011 to reset channel
    ADCON0 = ADCON0 | (channel << 2); //or with 00xx x000 for channel select
}
