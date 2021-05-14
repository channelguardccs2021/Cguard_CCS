#include <stdio.h>
#include <pthread.h>  
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "aws_iot_log.h"

#include "common.h"
#include "amazonMQTT.h"
#include "VendorFunction.h"

char *cert_dir = "./cert";

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

void iot_subscribe_callback_handler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
									IoT_Publish_Message_Params *params, void *pData) {
    char cmd = 0;
    cmd = ((char*) params->payload)[0];
    int err;

    printf("received command:>>%c", cmd);

    switch(cmd) {
        case 'n':
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
        case 'z':
            IOT_INFO("Enable Zigbee channel.");
            err = enableChannel("Zigbee");
            check_err(err, 0, "Failed to enable zigbee channel.");
            mqtt_log("zigbee channel enabled!");
            break;
        case 'Z':
            IOT_INFO("Disable Zigbee Channel");
            err = disableChannel("Zigbee");
            check_err(err, 0, "Failed to diable zigbee channel.");
            mqtt_log("zigbee channel disabled!");
            break;
        default:
            printf("[Error:] Unsupported command.\n");
            break;
		}
}

int main(void){
    char *channel_names[] = {"amazon_MQTT", "Zigbee"};
    int _channel_num = 2;
    int err;
	pthread_mutex_init(&gMutexLightBulb, NULL);

    err = initChannelList(channel_names, _channel_num);
    check_err(err, 0, "Failed to initialize.");
    mqtt_log("channel initialized!");

    certDirectory = "./cert";
    pTopicName = "myLightBulb";
    qos = QOS0;
    pApplicationHandler = &iot_subscribe_callback_handler;
    pApplicationHandlerData = NULL;

    err = enableChannel("amazon_MQTT");
    check_err(err, 0, "Failed to enable channel.");
    mqtt_log("channel enabled!");

    if(CHANNEL_LIST[0].channel_thread == 0){
        printf("Process error.\n");
        exit(1);
    } else{
        pthread_join(CHANNEL_LIST[0].channel_thread, NULL);
    }

    printf("End of main process.\n");

    return 0;
}