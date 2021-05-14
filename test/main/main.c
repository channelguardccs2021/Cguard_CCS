#include <stdio.h>
#include <pthread.h>  
#include <stdlib.h>

#include "aws_iot_log.h"

#include "common.h"
#include "amazonMQTT.h"
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
			OperateDevice(true,"amazon_MQTT");
            break;
        case 'f':
			OperateDevice(false,"amazon_MQTT");
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
        // enable Homekit Chennel
        case 'H':
            err = enableChannel("homekit");
            check_err(err, 0, "Failed to enable homekit channel.");
            mqtt_log("homekit channel enabled!");
            break;
        // diable Homekit Chennel
        case 'h':
            disableChannel("homekit");
            break;
        // enable Zigbee channel
        case 'Z':
            err = enableChannel("Zigbee");
            check_err(err, 0, "Failed to enable Zigbee channel.");
            mqtt_log("Zigbee channel enabled!");
            break;
        // diable Zigbee Chennel
        case 'z':
            disableChannel("Zigbee");
            break;
        // enable Gadget channel
        case 'G':
            err = enableChannel("Gadget");
            check_err(err, 0, "Failed to enable Gadget channel.");
            mqtt_log("Gadget channel enabled!");
            break;
        // diable Gadget Chennel
        case 'g':
            disableChannel("Gadget");
            break;
        default:
            printf("[Error:] Unsupported command.\n");
            break;
		}
}

int main(void){
    char *channel_names[] = {"amazon_MQTT", "homekit", "Gadget", "Zigbee"};
    int err;
	pthread_mutex_init(&gMutexLightBulb, NULL);

    err = initChannelList(channel_names, 4);
    check_err(err, 0, "Failed to initialize.");
    mqtt_log("channel initialized!");

    certDirectory = "./cert";
    pTopicName = "myLightBulb";
    qos = QOS0;
    pApplicationHandler = &iot_subscribe_callback_handler;
    pApplicationHandlerData = NULL;

    err = enableChannel("amazon_MQTT");
    check_err(err, 0, "Failed to enable amazon_MQTT channel.");
    mqtt_log("amazon_MQTT channel enabled!");

    err = enableChannel("homekit");
    check_err(err, 0, "Failed to enable homekit channel.");
    mqtt_log("homekit channel enabled!");

    err = enableChannel("Gadget");
    check_err(err, 0, "Failed to enable Gadget channel.");
    mqtt_log("Gadget channel enabled!");


    err = enableChannel("Zigbee");
    check_err(err, 0, "Failed to enable Zigbee channel.");
    mqtt_log("Zigbee channel enabled!");


    if(CHANNEL_LIST[0].channel_thread == 0 && CHANNEL_LIST[1].channel_thread == 0 && CHANNEL_LIST[2].channel_thread == 0 && CHANNEL_LIST[3].channel_thread == 0){
        printf("Process error.\n");
        exit(1);
    } else{
        pthread_join(CHANNEL_LIST[0].channel_thread, NULL);
        pthread_join(CHANNEL_LIST[1].channel_thread, NULL);
        pthread_join(CHANNEL_LIST[2].channel_thread, NULL);
        pthread_join(CHANNEL_LIST[3].channel_thread, NULL);
    }

    printf("End of main process.\n");

    return 0;
}
