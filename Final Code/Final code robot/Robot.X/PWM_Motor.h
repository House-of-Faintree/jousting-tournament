#ifndef PWM_MOTOR_H

#include <p18cxxx.h>
#define bit(x) (1<<(x)) 
#define test_bit(var,pos) ((var) & (1<<(pos)))

// might be done elsewhere?

void PWM_motor_setup(void);
void Inputs_motor_setup(void);
//void ADC_setup(void);
void direction(unsigned int, unsigned int);
void turn(unsigned int localVel,unsigned int localOmega);
void new_turn(int V, float distdiff);

void noob_turn(unsigned int localVel,unsigned int localOmega);

#endif	/* PWM_MOTOR_H */

