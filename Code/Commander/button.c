/*
 * File:   button.c
 * Author: Bevan
 * 
 * Has the setup for 2 button interrupts on PORT B bits 0 & 1 
 * PORT B bit 0 is for motors on/off
 * bit 1 is for menu
 *
 * Created on 14 October 2015, 2:12 PM
 * 
 */


#include <p18f452.h>


int menu_count = 0;
int motor_on_off = 0;

void high_interrupt(void);
void highPriorityIsr(void);

#pragma code highPriorityInterruptAddress=0x0008
void high_interrupt(void)
{
    _asm GOTO highPriorityIsr _endasm
}




void Button_Setup(void)
{
    TRISB = 0x00; 
    PORTB = 0x00;    
    
    //Interrupt setup
    INTCONbits.GIEL = 0; //disable interrupts
    INTCONbits.GIEH = 0;
    INTCON2bits.RBPU = 0; //enable port B pull-up interrupts
    
    INTCONbits.INT0IE = 1;  //enable external interrupt 0 PortB bit 0
    INTCON2bits.INTEDG0 = 1; //rising edge trigger, default high priority
    
    INTCON3bits.INT1IE = 1; //enable external interrupt 1 PortB bit 1
    INTCON2bits.INTEDG1 = 1; //rising edge trigger 
    INTCON3bits.INT1IP = 1; //set high priority
     
    INTCONbits.INT0IF = 0; //clear flags
    INTCON3bits.INT1IF = 0;
    PIR1 = 0x00;
    PIR2 = 0x00;
    INTCONbits.GIEL = 1; //enable Interrupts
    INTCONbits.GIEH = 1; 

}

void highPriorityIsr(void)
{    
    //PORT B bit 0 external interrupt for motors on/off button
    if(INTCONbits.INT0IF == 1)
    {
        if(motor_on_off == 0) motor_on_off = 1;
        if(motor_on_off == 1) motor_on_off = 0;

        INTCONbits.INT0IF = 0; //clear flag
        INTCONbits.GIEH = 1; //re-enable interrupts
        PORTBbits.RB0 = 0;  //clear port B bit
    }

    //Port B bit 1 external interrupt for menu button
    if(INTCON3bits.INT1IF == 1)  
    {
        menu_count++;
        if(menu_count == 5)
            {
                menu_count = 0;
            }
        INTCON3bits.INT1IF = 0; //clear flag
        INTCONbits.GIEH = 1; //re-enable interrupts
        PORTBbits.RB1 = 0; //clear port B bit
    }        
}

void main(void) {
    Button_Setup();
    while(1);;
}
