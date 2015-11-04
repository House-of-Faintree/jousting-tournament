/*******************************************************************************
 * MODULE:  IR_Sensors
 *..............................................................................
 *  Created By:         BEVAN
 *  Created Date:       21.10.2015
 *..............................................................................
 *  Last Edit Date:     28.10.2015
 *  Last Edit By:       BEVAN
 *  Last Edit Details:  cleaned up 
 *..............................................................................
 * File/Function Property/Usage:
 *  See description below.
 *
 *..............................................................................
 * Description:
 *  Controls the setup and use of IR Sensors
 *  To be used for the robot
 *  Outputs the current distance from the tilt 
 *  As well as the state of the IR_Sensor - ERROR,FRONT_IR_CLOSEST,PARALLEL,BACK_IR_CLOSEST (0-3) 
 *
 * Dependancies:
 *  ADC must first be set up
 *  Uses global variables IR_SAMPLE_R, IR_SAMPLE_E
 * 
 *..............................................................................
 * Current Issues:
 *  
*******************************************************************************/

#include  <p18f4520.h>
#include  "GlobalVariblesRobot.h"
#include  "AD.h"
#include  "IR_Sensors.h"



float S_IR_cm = 0; //distance in cm of side IR
char S_IR_inrange = 0; //0 not in range, 1 in range
char S_IR_state = 0; //uses above defines IR_ERROR, BACK_IR_CLOSEST, IR_PARALLEl, FRONT_IR_CLOSEST

int Sample_Finished = 0; //flag set when a full sample is finished by interrupts
int newdistance = 0; //flag set when a new distance has been calculated
int newstate = 0; //flag set when the system changes IR state
int dist_count = 0; //counts number of distances calculated, used in assist mode

float old_S_IR_cm = 0; //the previous distance calculated
float dist_diff = 0; //difference between previous distance and current distance

unsigned int IR_Sample_count = 0; //counter used in sampling until = Sample Rate
unsigned int IR_count = 0; //counter used for sampling until = Sample per estimate
unsigned int Side_IR_Buff[IR_BUFFSIZE]; //stores raw IR samples
unsigned int *S_IR; //points to buffer
unsigned int avg_S_IR = 0; //avg IR raw value

float m = 0.095; //used to convert raw IR value to cm
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
/*
 * function IR_Sample is to be called from an interrupt
 * reads value from ADC and stores in buffer
 * only samples when IR_Sample_Count = the Global variable IR sample rate set by user
 * stores a number of values in buffer according to Global variable IR sample per Estimate
 * when all samples taken sets Sample_Finished flag to 1
 */
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
            }
        
        }
      
    }
        

/*
 * function IR_Calculate is to be called from project main during auto and assist mode
 * when a sample is finished this function calculates the current distance in cm to the tilt and 
 * the state of the IR system. It also checks to see if the new state is different to the previous 
 * state calculated and if so sets the newstate flag. When finished clears Sample_Finished flag. 
 */
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

/*
 * This function returns the average value of an int array 
 */
unsigned int getAverage( unsigned int* array,unsigned int size){
    
    int i;
    unsigned int avg = 0;
    unsigned long sum = 0;
    
    for(i = 0;i<size;i++)
        sum += array[i];
    
    avg = sum / size;
    return avg;
}

/*
 * This function returns 1 if the IR sensor is within range ( < 30cm)
 * or 0 if out of range.
 */
char check_in_IR_range(unsigned int avg_value)
{
    if((avg_value >= LOWER_IR_LIMIT))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * this function converts the int raw IR value to a float in cm
 * must first check to make sure IR is in range to get accurate result
 */
float convert_IR_to_cm( unsigned int IR_value)
{
    float cm_value = (1/((m*IR_value*5/255)-b))-0.42; 

    return cm_value;
}

/*
 * this function calculates the state of the IR system where:
 * 1 = too close
 * 2 = too parallel
 * 3 = too far
 */
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
    }
    
    return state;
}






