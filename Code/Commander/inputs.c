/*
Authored: Ayush Sharma
Created 13/10/2015
*******************************************************************************

This file contains functionality for input to the controller from joysticks,
buttons, additional potentiometer etc.
functions from here should be called externally by including a header file
inputs.h.

*TODO:
 * 
 * - Implement multiple joysticks and buttons with interrupts. 
 * 
*/


#include <adc.h>
#include <p18f4520.h>
#include <usart.h>
#include "serialcommander.h"
#include "PWM_motor.h"

#pragma config  WDT = OFF
#pragma config  LVP = OFF
#pragma config  OSC = HS  

//These will be used in a function and correspond to bits in port A.
#define leftjoyX    0;  //RA0
#define leftjoyY    1;  //RA1
#define rightjoyX   2;  //RA2
#define rightjoyY   3;  //RA3

void ADC_setup(void);
char doADC(void);
void switchChannels(int channel);

char LXstring[10] = "leftjoyX";
char LYstring[10] = "leftjoyY";
char *ptr;

/*A test main function which can evolve into the main function of the controller,
 since when idle, we want the main function to stay monitoring the joystick positions.
 */
void main (void){
    int result;
    int i;

    i = 0;
    ADC_setup();
    setupSerial();  //Can be implemented to send data via serial to xbee/Robot
    //PWM_motor_setup();
    //Inputs_motor_setup();
    while(INTCONbits.TMR0IF==0){}   // wait for adc to charge up
    while(1){
        /*Following code block keeps scanning for changes to joystick positions*/
        for(i=0; i<2; i++){
            result = doADC();
            ptr = LXstring;
//            sendByte(ptr);
            sendByte(result); //this will work very well if i just make result a char, 
                              //and I can find no reason not to.
            /*if(i = 0){
                turn((int)result);
            }
            else if(i = 1){
                vel = (int)result;
            }
            */
            switchChannels(i); //using variable 'i' rather than leftjoyX etc for convenience.
        }
        i = 0;
    }
}

/*
@brief  Initiates conversion, waits for completion then returns result.
 * joystick potentiometers return  0x02 < values < 0x48 with nominal being 0x0F.
 */
char doADC(void){
    ADCON0bits.GO=1;
    while(PIR1bits.ADIF==0){}   // wait for conversion to finish
    return ADRESH;
}

/*
 @brief Setup of ADC in default channel AN0, all channels analog, with
 reference voltages of Vdd and Gnd. Also modifies TMR0, so be careful if using
 it elsewhere.
 */
void ADC_setup(void){
    T0CON = 0xC7; //  11000111  Enables TMR0, 8bit with 256 prescale (p103)
    ADCON0 = 0x41;  //0100 0001 Fosc/8, A/D enabled, Channel AN0 by default
    ADCON1 = 0x00;  // 0000 0000 Make All channels analog input, Left justify,
}

/*
@brief A function which exploits the useful property that in ADCON0 the bits
 * that control which channel is being monitored are modified by putting the 3 bits
 * as the literal number from 0-8. As a result, the input to this function *has*
 * to be between 0 and 7. To save clock cycles there is no check to see if this is
 * true, so be careful. When in doubt use the #defines of the channel names.
 */
void switchChannels(int channel){
    ADCON0 = ADCON0 & 0xc7; //mask adcon with 1100 0111 to reset channel
    ADCON0 = ADCON0 | (channel << 3); //or with 00xx x000 for channel select
}
