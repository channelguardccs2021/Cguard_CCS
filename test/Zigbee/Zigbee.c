/*
Zigbee control demo
 */
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include <pthread.h>  
#include <stdlib.h>

#include "common.h"
#include "VendorFunction.h"

bool gLightBulbState = false;
pthread_mutex_t gMutexLightBulb;

void mqtt_log(char *l){
    printf("%s\n", l);
}

void check_err(int err, int good, char* err_mqtt_log){
    if(err != good){
        printf("%s\n", err_mqtt_log);
        exit(1);
    }
    return;
}

int main() {	
	char *channel_names[] = {"Zigbee"};
    int err;
	pthread_mutex_init(&gMutexLightBulb, NULL);

    err = initChannelList(channel_names, 1);
    check_err(err, 0, "Failed to initialize.");
    mqtt_log("zigbee channel initialized!");

    err = enableChannel("Zigbee");
    check_err(err, 0, "Failed to enable zigbee channel.");
    mqtt_log("zigbee channel enabled!");

    if(CHANNEL_LIST[0].channel_thread == 0){
        printf("Process error.\n");
        exit(1);
    } else{
        pthread_join(CHANNEL_LIST[0].channel_thread, NULL);
    }

    printf("End of main process.\n");

    return 0;
}
