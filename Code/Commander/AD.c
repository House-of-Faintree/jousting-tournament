#include <p18f4520.h>
#include "AD.h"

void ADC_setup(void);
void switchChannels(int channel);
int doADC(void);


void ADC_setup(void){
    T0CON = 0xC7; //  11000111  Enables TMR0, 8bit with 256 prescale (p103)
    ADCON0 = 0x41;  //0100 0001 Fosc/8, A/D enabled, Channel AN0 by default
    ADCON1 = 0x00;  // 0000 0000 Make All channels analog input, Left justify,
    while(INTCONbits.TMR0IF==0){} //wait for ADC to charge up
}

int doADC(void){
    ADCON0bits.GO=1;
    while(PIR1bits.ADIF==0){}   // wait for conversion to finish
    return ADRESH;
}

void switchChannels(int channel){
    ADCON0 = ADCON0 & 0xc7; //mask adcon with 1100 0111 to reset channel
    ADCON0 = ADCON0 | (channel << 3); //or with 00xx x000 for channel select
}
