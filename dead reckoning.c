#include "abdrive.h"
#include "simpletools.h"
#include <math.h>
#include "ping.h"
#define WANTED 14

int irLeft, irRight;

float calculateangle (float left, float right){
    return ((left-right)/10.58); 
}

int main() // main function
{
    
    low(26);
    low(27);
    int right;
    int left;
    float factor;
    
    
    
    while(1) // Main loop
        
    {
        irLeft = 0;                                  
        irRight = 0;                                    
        
        for(int dacVal = 0; dacVal < 160; dacVal += 8) 
        {                                               
            dac_ctr(26, 0, dacVal);                     
            freqout(11, 1, 38000);                    
            irLeft += input(10);                    
            
            dac_ctr(27, 1, dacVal);                     
            freqout(1, 1, 38000);
            irRight += input(2);                        
        }                                               
        
       //print("%c irLeft = %d, irRight = %d%c, factor: %d",       
        //      HOME,   irLeft*2-6, irRight, CLREOL, factor);        
        
        pause(100);
        factor = 4*(WANTED-(irLeft*2-6));
        drive_speed((90+factor)/4,(90-factor)/4);  
        
        if (ping_cm(8) < 7){
            break;
        }           
    }
    drive_getTicks(&left, &right); 
    float walked_left_cm = 0.325*(left);
    float walked_right_cm = 0.325*(right);
    float angle = calculateangle(walked_left_cm, walked_right_cm);
    float rm = ((walked_left_cm/angle) + (walked_right_cm/angle))/2;;
    double distance = sqrt(pow((rm - rm*cos(angle)),2) + pow(rm * sin(angle),2));
    drive_speed(0,0);
    sd_mount(22, 23, 24, 25); 
    FILE* fp = fopen("test.txt", "w");
    fwrite(&angle, sizeof(float), 1, fp); 
    fwrite("\n", 1, 1, fp); 
    fwrite(&distance, sizeof(double), 1, fp);  
    fclose(fp);
}
