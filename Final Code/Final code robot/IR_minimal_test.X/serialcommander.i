#line 1 "serialcommander.c"
#line 1 "serialcommander.c"

#line 20 "serialcommander.c"
 







unsigned char *rxptr = GLOBAL_RXBUFFER;


int sendByte(char *byte){ 
    if(*byte != 0x00){

    while(TXSTAbits.TRMT == 0);
    TXREG = *byte;
    return 1;
    
    }
    return 0;    
}

void sendNum(int result){
    while(TXSTAbits.TRMT == 0);
    TXREG = result;
    return;
}

void sendNewLine(void){
    while(TXSTAbits.TRMT == 0);
    TXREG = 0x0a; 
    
    while(TXSTAbits.TRMT == 0);
    TXREG = 0x0d; 
    
}

void sendString(char *byte){
    
    while(sendByte(byte)){
        byte++;
    }
    sendNewLine();
    return;
    
}

void sendMaxSpeed(void)
{
    sendNum(MaxSpeed);
    sendNum(GLOBAL_MAX_SPEED);
    sendNum(MaxSpeedEnd);
}
void sendMaxYaw(void)
{
    sendNum(MaxYaw);
    sendNum(GLOBAL_MAX_YAW);
    sendNum(MaxYawEnd);
}
void sendIRsampleR(void)
{
    sendNum(IRsampleR);
    sendNum(GLOBAL_IRsampleR);
    sendNum(IRsampleREnd);
}
void sendMode(void)
{
    sendNum(Mode);
    sendNum(GLOBAL_MODE);
    sendNum(ModeEnd);
}
void sendHello(void)
{
    sendNum(Hello);
    sendNum(HelloEnd);
}

void sendVelocity(void)
{
    if (GLOBAL_VELOCITY > 238)
    {
        GLOBAL_VELOCITY = 238;
    }
    sendNum(Velocity);
    delayms(5);
    sendNum(GLOBAL_VELOCITY);
    delayms(5);
    sendNum(VelocityEnd);
}

void sendOmega(void)
{
    if (GLOBAL_OMEGA > 238)
    {
        GLOBAL_OMEGA = 238;
    }
    sendNum(Turn);
    delayms(5);
    sendNum(GLOBAL_OMEGA);
    delayms(5);
    sendNum(TurnEnd);
}

void sendRun (void)
{
    sendNum(RunStart);
    sendNum(GLOBAL_RUN);
    sendNum(RunEnd);
}

#line 134 "serialcommander.c"
 
void setupSerial(void){
  IPR1bits.RCIP = 1;
  RCONbits.IPEN = 1;
  
  TRISBbits.RB4 = 0;
  
  TRISCbits.RC6 = 0;              
  TRISCbits.RC7 = 1;              
  TXSTAbits.SYNC = 0;             
  TXSTAbits.BRGH = 1;             
  TXSTAbits.TXEN = 1;
  
  SPBRG = 64;                     
  RCSTAbits.SPEN = 1;             
  RCSTAbits.CREN = 1;             
  PIE1bits.RCIE = 1;
  
  INTCONbits.GIE = 1;
  INTCONbits.PEIE = 1;
}








void TMR0Init(void)
{
    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 0;
    T0CONbits.PSA = 1;
    T0CONbits.T0CS = 0;
    INTCON2bits.TMR0IP = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
}


void rx232Isr (void)
{    
    if (RCREG == Mode)
    {
        rxptr = &GLOBAL_MODE;
        
    }
    else if (RCREG == ModeEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == MaxSpeed)
    {
        rxptr = &GLOBAL_MAX_SPEED;
    }
    else if (RCREG == MaxSpeedEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == MaxYaw)
    {
        rxptr = &GLOBAL_MAX_YAW;
    }
    else if (RCREG == MaxYawEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == Hello)
    {
        sendNum(Hello);
        PORTBbits.RB4 = 1;
        sendNum(HelloEnd);
    }
    else if (RCREG == HelloEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == Velocity)
    {
        rxptr = &GLOBAL_VELOCITY;
    }
    else if (RCREG == VelocityEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == IRsampleR)
    {
        rxptr = &GLOBAL_IRsampleR;
    }
    else if (RCREG == IRsampleREnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == Turn)
    {
        rxptr = &GLOBAL_OMEGA;
    }
    else if (RCREG == TurnEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else if (RCREG == RunStart)
    {
        rxptr = &GLOBAL_RUN;
    }
    else if (RCREG == RunEnd)
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
    else 
    {
        *rxptr = RCREG;
        rxptr++;
    }
    *rxptr = RCREG ;
    
    
    if (RCREG == '\r'||rxptr == &GLOBAL_RXBUFFER[BUFFSIZE])
    {
        rxptr = &GLOBAL_RXBUFFER[0];
    }
}

void TMR0Isr (){
    
    INTCONbits.TMR0IE = 0;
    
    
    
    
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    
}
