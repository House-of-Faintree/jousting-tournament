#include <p18f4520.h>
#include "AD.h"

void ADC_setup(void);
void switchChannels(int channel);
int doADC(void);


void ADC_setup(void){
    T0CON = 0xC7; //  11000111  Enables TMR0, 8bit with 256 prescale (p103)
    ADCON0 = 0x41;  //0100 0001 Fosc/8, A/D enabled, Channel AN0 by default
    ADCON1 = 0x00;  // 0000 0000 Make All channels analog input, Left justify,
    ADCON2 = 0x04 // left justify, manual acq, Tad = Fosc/4
    while(INTCONbits.TMR0IF==0){} //wait for ADC to charge up
}

int doADC(void){
    ADCON0bits.GO=1;
    while(PIR1bits.ADIF==0){}   // wait for conversion to finish
    return ADRESH;
}
/*
Takes in an argument between 0 and 7 to change sameple rate
  0 = manual acquisition
  1 = 2Tad
  2 = 4Tad
  3 = 6Tad
  4 = 8Tad
  5 = 12Tad
  6 = 16Tad
  7 = 20Tad
*/
void changeSampleRate(int rate){
  ADCON2 = ADCON2 & 0xc7; //mask with 1100 0111 to reset rate
  ADCON2 = ADCON2 |(rate << 3); //or with 00xx x000 to set new rate
}
void switchChannels(int channel){
    ADCON0 = ADCON0 & 0xc7; //mask adcon with 1100 0111 to reset channel
    ADCON0 = ADCON0 | (channel << 3); //or with 00xx x000 for channel select
}
