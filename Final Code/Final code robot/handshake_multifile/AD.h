/*
 * File:   AD.h
 * Author: asha6047
 *
 * Created on 15 October 2015, 1:53 PM
 */

#ifndef AD_H
#define	AD_H

void ADC_setup(void);
void switchChannels(int channel);
int doADC(void);
void changeSampleRate(int rate);

#endif	/* AD_H */
