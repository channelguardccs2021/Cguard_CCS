#include <stdio.h>
#include <pthread.h>  
#include <stdlib.h>

#include "aws_iot_log.h"

#include "common.h"
#include "amazonMQTT.h"
#include "homekit.h"
#include "VendorFunction.h"

bool gLightBulbState = false;
pthread_mutex_t gMutexLightBulb;
char *cert_dir = "./cert";

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

void iot_subscribe_callback_handler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
									IoT_Publish_Message_Params *params, void *pData) {
    char cmd = 0;
    int err;

    cmd = ((char*) params->payload)[0];

    printf("received command:>>%c", cmd);

    switch(cmd) {
        case 't':
			OperateDevice(true);
            break;
        case 'f':
			OperateDevice(false);
            break;
        case 'q':
            pthread_mutex_lock(&gMutexLightBulb);
			ReadDeviceStatus();
            pthread_mutex_unlock(&gMutexLightBulb);
            break;
        case 'e':
            IOT_INFO("Exiting from AWSChannel.");
            pthread_exit(NULL);
            break;
        default:
            printf("[Error:] Unsupported command.\n");
            break;
		}
}

int main(void){

	pthread_mutex_init(&gMutexLightBulb, NULL);


    certDirectory = "./cert";
    pTopicName = "myLightBulb";
    qos = QOS0;
    pApplicationHandler = &iot_subscribe_callback_handler;
    pApplicationHandlerData = NULL;

    
     int ThreadSuccess1 = 0;
    pthread_t *channel_thread_id1 = (pthread_t *)malloc(sizeof(pthread_t));
    ThreadSuccess1 = pthread_create(channel_thread_id1, NULL, (void *)runAmazonMQTT, NULL); 
    if(0 != ThreadSuccess1){
        printf("Create AWS pthread error\n");
        exit(1);
    }

    int ThreadSuccess2 = 0;
    pthread_t *channel_thread_id2 = (pthread_t *)malloc(sizeof(pthread_t));
    ThreadSuccess2 = pthread_create(channel_thread_id2, NULL, (void *)runHomekit, NULL); 
    if(0 != ThreadSuccess2){
        printf("Create homekit pthread error\n");
        exit(1);
    }

    int ThreadSuccess3 = 0;
    pthread_t *channel_thread_id3 = (pthread_t *)malloc(sizeof(pthread_t));
    ThreadSuccess3 = pthread_create(channel_thread_id3, NULL, (void *)runZigbee, NULL); 
    if(0 != ThreadSuccess3){
        printf("Create Zigbee pthread error\n");
        exit(1);
    }


    int ThreadSuccess4 = 0;
    pthread_t *channel_thread_id4 = (pthread_t *)malloc(sizeof(pthread_t));
    ThreadSuccess4 = pthread_create(channel_thread_id4, NULL, (void *)runGadget, NULL); 
    if(0 != ThreadSuccess4){
        printf("Create Gadget pthread error\n");
        exit(1);
    }

    if(*channel_thread_id1 == 0 && *channel_thread_id2 == 0 && *channel_thread_id3== 0 && *channel_thread_id4 == 0){
        printf("Process error.\n");
        exit(1);
    } else{
        pthread_join(*channel_thread_id1, NULL);
        pthread_join(*channel_thread_id2, NULL);
        pthread_join(*channel_thread_id3, NULL);
        pthread_join(*channel_thread_id4, NULL);
    }
    printf("End of main process.\n");

    return 0;
}
