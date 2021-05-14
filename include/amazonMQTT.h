#ifndef AMAZON_MQTT_H
#define AMAZON_MQTT_H

#include "aws_iot_mqtt_client.h"

//void runAmazonMQTT(void);

// runAmazonMQTT's arguments.
// Only pApplicationHandlerData can be NULL.
extern char *certDirectory ;
extern char *pTopicName ;
extern QoS qos ;
extern pApplicationHandler_t pApplicationHandler ;
extern void *pApplicationHandlerData ;

#endif
