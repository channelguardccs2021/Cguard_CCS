/*VendorFunction.c is implemented by vendor*/

#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "VendorFunction.h"
#include "common.h"
#include <wiringPi.h>


void OperateDevice(bool value)
{   
    //led init
    char LED = 4;// <wiringPi.h> gpio23 Eighth on the upper right
    if(wiringPiSetup() < 0){
        printf("wiringPi error\n");
    }
    pinMode (LED,OUTPUT) ;

    //mutex_lock
    pthread_mutex_lock(&gMutexLightBulb); 
    gLightBulbState = value;

    if(gLightBulbState == true){
        digitalWrite(LED, 1);
        delay(100);
    }
    if(gLightBulbState == false){
        digitalWrite(LED, 0);
        delay(100);
    }   
    //mutex_unlock
    pthread_mutex_unlock(&gMutexLightBulb);
    printf("\n****Set light status to %d ****\n", gLightBulbState);
}

bool ReadDeviceStatus(void)
{
    printf("\n****Light status is %d ****\n", gLightBulbState);
    return gLightBulbState;
}

//<wiringPi.h> and led test code
/* <wiringPi.h> control led example  */
// #include <wiringPi.h>
// char LED = 15; 
 
// int main(void)
// {
//     if(wiringPiSetup() < 0)return 1;
//     pinMode (LED,OUTPUT) ;
 
//     while(1)
//     {   
//         digitalWrite(LED, 1) ;
//         delay (200);
//         digitalWrite(LED, 0) ;
//         delay (200);
//     }   
// }
