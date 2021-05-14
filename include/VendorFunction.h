#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>  
#include <wiringPi.h>

extern bool gLightBulbState;
extern pthread_mutex_t gMutexLightBulb;

//operate device 
void OperateDevice(bool value,char *channel);

//read device status
bool ReadDeviceStatus(void);