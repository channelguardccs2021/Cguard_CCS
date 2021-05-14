/*
Amazon Gadget demo
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.h"
#include "VendorFunction.h"


bool gLightBulbState = false;
pthread_mutex_t gMutexLightBulb;

void gadget_log(char *l){
    printf("%s\n", l);
}


void check_err(int err, int good, char* err_gadget_log){
    if(err != good){
        printf("%s\n", err_gadget_log);
        exit(1);
    }
    return;
}

int main(void){
    char *channel_names[] = {"Gadget"};
    int err;
	pthread_mutex_init(&gMutexLightBulb, NULL);
    err = initChannelList(channel_names, 1);
    check_err(err, 0, "Failed to initialize.");
    gadget_log("channel initialized!");

    err = enableChannel("Gadget");
    check_err(err, 0, "Failed to enable Gadget channel.");
    gadget_log("Gadget channel enabled!");

    if(CHANNEL_LIST[0].channel_thread == 0){
        printf("Process error.\n");
        exit(1);
    } else{
        pthread_join(CHANNEL_LIST[0].channel_thread, NULL);
    }

    printf("End of main process.\n");
    return 0;
}

