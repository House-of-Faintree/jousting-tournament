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

//NTS exercise care with string lengths, no more than 16 characters
const char s0[LCDL]="PID gain:";
const char s1[LCDL]="Max speed:";
const char s2[LCDL]="Max yaw:";
const char s3[LCDL]="IR samp/est:";

//Menu titles
const char t0[LCDL]="Manual Mode";
const char t1[LCDL]="Assisted Mode";
const char t2[LCDL]="Full Auto";
const char t3[LCDL]="Factory Mode";

#endif	/* PHRASES_H */
