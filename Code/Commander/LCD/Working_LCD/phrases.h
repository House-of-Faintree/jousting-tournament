/* 
 * File:   phrases.h
 * Author: llou6558
 *
 * Created on 13 October 2015, 3:19 PM
 */

#ifndef PHRASES_H
#define	PHRASES_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#define LCDL 17         //define lcd length, 17th byte is a null

//Welcome screen
const char w0[LCDL]="Bow, for I am";
const char w1[LCDL]="Charlemagne!";

//NTS exercise care with string lengths, no more than 13 characters
const char s0[LCDL]="Max speed:";
const char s1[LCDL]="PID gains:";
const char s2[LCDL]="Max yaw rate:";
const char s3[LCDL]="IR samp/est:";
const char s4[LCDL]="IR samp rate:";
const char s5[LCDL]="Raw data:";
const char s6[LCDL]="Avg. data:";

//Menu titles
const char t0[LCDL]="Manual Mode";
const char t1[LCDL]="Assisted Mode";
const char t2[LCDL]="Full Auto";
const char t3[LCDL]="Factory Mode";
const char t4[LCDL]="Giddyup!";   //For run time
const char t5[LCDL]="Whoa!";      //Stop

#endif	/* PHRASES_H */
