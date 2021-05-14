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
     
		receiveVal=  serialGetchar(fd);
		//printf("receiveVal: %d\n", receiveVal);

		switch(receiveVal)
		{
			case 235:   //on-n
			  printf("received command:>>n---turn on the light");
			  OperateDevice(true);
			  break;
			case 233:   //off-f
			  printf("received command:>>f---turn off the light");
			  OperateDevice(false);
			  break;
			case 236:  //query-q
			  printf("received command:>>q---check the staus of light");
			  ReadDeviceStatus();
			  break;			
	  }
	  
	}
	return;
}



