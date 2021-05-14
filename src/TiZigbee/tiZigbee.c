#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "common.h"
#include "VendorFunction.h"



void runZigbee(void){
	int fd;
	int receiveVal;  
	if (wiringPiSetup() < 0)
		return;
	if ((fd = serialOpen("/dev/ttyS0", 115200)) < 0)
		return;
	serialPrintf(fd, "1");
		
	printf("waiting... \n");
	while(1)
	{  
		if (checkChannel("Zigbee") == 0){
			printf("Channel closed. \n");
			serialPrintf(fd, "0");
			serialClose(fd);
			return;
		}	
     
		receiveVal=  serialGetchar(fd);
		//printf("receiveVal: %d\n", receiveVal);

		switch(receiveVal)
		{
			case 235:   //on-n
			  printf("received command:>>n---turn on the light");
			  OperateDevice(true,"Zigbee");
			  break;
			case 233:   //off-f
			  printf("received command:>>f---turn off the light");
			  OperateDevice(false,"Zigbee");
			  break;
			case 236:  //query-q
			  printf("received command:>>q---check the staus of light");
			  ReadDeviceStatus();
			  break;
	        case 232:  //query-e
			  printf("received command:>>e---stop Zigbee Control\n");
			  disableChannel("Zigbee");
			  break;				
	  }
	  
	}
	return;
}



