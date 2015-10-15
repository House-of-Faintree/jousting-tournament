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


#include <p18f4520.h>
#include <adc.h>
#include <usart.h>
#include "serialcommander.h"
#include "AD.h"

#pragma config  WDT = OFF
#pragma config  LVP = OFF
#pragma config  OSC = HS

#define LEFTJOY_X 0
#define LEFTJOY_Y 1

char init[] = "hello world, bout to send data";
char *letter;
int L_result_X[30] = 0;
int L_result_Y[30] = 0;
int *resX;
int *resY;
char LXstring[10] = "leftjoyX";
char LYstring[10] = "leftjoyY";
char left[] = "LEFT";
char right[] = "RIGHT";
char up[] = "UP";
char down[] = "DOWN";

void main (void ){
    
    setupSerial();
    ADC_setup();
    letter = init;
    resX = &L_result_X[0];
    resY = &L_result_Y[0];
    
    while(1){
    if (resX == (&L_result_X[30]+1)){
        resX = &L_result_X[0];
    }
    if (resY == (&L_result_Y[30]+1)){
        resY = &L_result_Y[0];
    }
    
    switchChannels(LEFTJOY_X);
    
    letter = LXstring;
    sendString(letter);
    
    *resX = doADC();
    resX++;
    sendNum(*resX);
    sendNewLine();
    
    switchChannels(LEFTJOY_Y);
    
    letter = LYstring;
    sendString(letter);
    
    *resY = doADC();
    resY++;
    sendNum(*resY);
    sendNewLine();
    
    }
}
