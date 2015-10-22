#ifndef SERIAL_COMMANDER_H
#define SERIAL_COMMANDER_H


void setupSerial(void);
int sendByte(char *byte);
void sendNewLine(void); 
void sendString(char *byte); 
void sendNum(int result); 
void sendHello(void);
void sendMaxSpeed(void);
void sendMaxYaw(void);
void sendIRsampleE(void);
void sendIRsampleR(void);
void sendMode(void);

//char *rxptr = GLOBAL_RXBUFFER;

#endif
