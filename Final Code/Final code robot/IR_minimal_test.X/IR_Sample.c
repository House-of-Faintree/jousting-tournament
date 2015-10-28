
#include  <p18f4520.h>
#include  "GlobalVariblesRobot.h"
#include  "AD.h"
#include  "IR_Sensors.h"


unsigned char urgency = 0;

float S_IR_cm = 0;
char S_IR_inrange = 0; //0 not in range, 1 in range
char S_IR_state = 0; //uses above defines IR_ERROR, BACK_IR_CLOSEST, IR_PARALLEl, FRONT_IR_CLOSEST

int Sample_Finished = 0;
int newdistance = 0;
int newstate = 0;
int dist_count = 0;

float old_S_IR_cm = 0;
float dist_diff = 0;

unsigned int IR_Sample_count = 0;
unsigned int IR_count = 0;
unsigned int Side_IR_Buff[IR_BUFFSIZE];
unsigned int *S_IR;
unsigned int avg_S_IR = 0;

float m = 0.095;
float b = 0.021;


char Get_S_IR_state(void);
float Get_Current_distance(void);
void IR_Setup(void);
void IR_Sample(void);
void IR_Calculate(void);
unsigned int getAverage( unsigned int* array,unsigned int size);
char check_in_IR_range(unsigned int avg_value);
float convert_IR_to_cm( unsigned int IR_value);
char determine_IR_parallel(float S_IR);


char Get_S_IR_state(void)
{
    return S_IR_state;
}

float Get_Current_distance(void)
{
    return S_IR_cm;
}

float Get_dist_diff(void)
{
    return dist_diff;
}

void IR_Setup(void)
{
    S_IR = &Side_IR_Buff[0]; 
    
}

void IR_Sample(void)
{


      IR_Sample_count++;
        if(IR_Sample_count == (GLOBAL_IRsampleR)) 
        {
            IR_Sample_count = 0;
            switchChannels(SIDE_IR);
            *S_IR = doADC();
            S_IR++;
        
            IR_count++;

            if(IR_count ==  (GLOBAL_IRsampleE))
            {
                S_IR = &Side_IR_Buff[0];
                IR_count = 0;
                Sample_Finished = 1;
                //IR_Calculate();       
            }
        
        }
//      if(newstate != IR_PARALLEL)
//      {
//          urgency++;
//      }
      
    }
        






void IR_Calculate(void)
{
    char new_state = 0;
    if(Sample_Finished == 1)
    {
    
        avg_S_IR = getAverage(Side_IR_Buff,GLOBAL_IRsampleE);
        
        GLOBAL_IRRAW = avg_S_IR;
        sendIRraw();
            
        S_IR_inrange = check_in_IR_range(avg_S_IR);
    
            
        if(S_IR_inrange == 1)
        {
            old_S_IR_cm = S_IR_cm;
            S_IR_cm = convert_IR_to_cm(avg_S_IR);
            dist_diff = S_IR_cm - old_S_IR_cm;
            dist_count++;
            if(dist_count == 3)
            {
                dist_count = 0;
            }
            
            new_state = determine_IR_parallel(S_IR_cm);          
            
        }
        else
        {
            new_state = IR_ERROR;
            dist_diff = 0;
        }
        newdistance = 1;
        if(new_state != S_IR_state)
        {
            S_IR_state =  new_state;
            newstate = 1;                       
        }
        Sample_Finished = 0;
    }                  
}


unsigned int getAverage( unsigned int* array,unsigned int size){
    
    int i;
    unsigned int avg = 0;
    unsigned long sum = 0;
    
    for(i = 0;i<size;i++)
        sum += array[i];
    
    avg = sum / size;
    return avg;
}

char check_in_IR_range(unsigned int avg_value)
{
    if(/*(avg_value <= UPPER_IR_LIMIT) && */(avg_value >= LOWER_IR_LIMIT))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

float convert_IR_to_cm( unsigned int IR_value)
{
    float cm_value = (1/((m*IR_value*5/255)-b))-0.42; 

    return cm_value;
}

char determine_IR_parallel(float S_IR)
{
    char state = 0;
    S_IR = S_IR - IR_OFFSET;
    if(S_IR < (GLOBAL_AUTO_DISTANCE))
    {
        state = IR_TOO_CLOSE;
    }
    else if(S_IR > (GLOBAL_AUTO_DISTANCE))
    {
        state = IR_TOO_FAR;
    }
    else
    {
        state = IR_PARALLEL;
        urgency = 0;
    }
    
    return state;
}






