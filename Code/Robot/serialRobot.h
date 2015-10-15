/* 
 * File:   serialRobot.h
 * Author: zzha1258
 *
 * Created on 15 October 2015, 6:00 PM
 */

#ifndef SERIALROBOT_H
#define	SERIALROBOT_H

void setupSerial(void);
int sendByte(char *byte);
void sendNewLine(void);
void sendString(char *byte);
void sendNum(int result);
extern char GLOBAL_RXBUFFER[];
extern char *rxptr;
#endif	/* SERIALROBOT_H */

