/* a sample code to put a range of the variable into an array, this variable
 * keep changing so that the value in the array is different,
 * then we calculate the average value of this array, which is return a variable call
 * avg
 */

#include <stdio.h>
#include <stdlib.h>
#include <adc.h>
#include <p18f452.h>
#include <usart.h>
#include <string.h>

double avg;
float array[255],source;
int size;

// store the value into an array
void ToArray(float* array, int sampleSize,float source){
    
    int i
    for (i = 0; i < sampleSize;++i)
        array[i] = source;
       
}
// calculate the average value from the array
double getAverage( float* array,int size){
    
    int i;
    double avg,sum;
    
    for(i = 0;i<size;++i)
        sum += array[i];
    
    avg = sum / size;
    return avg;
}

void main(void){
    
    
    source = ADRESH;    // a example that the source value is taken from the ADC variable
    size = 100;         // assume the sample size is 100 (example)
   
    ToArray(array,size,source);     // put the source variable into the array
                                    // which means record the data
    
    avg = getAverage (array,size);  // calculate the average value from the array.
    
    while(1);
    
    }
}

