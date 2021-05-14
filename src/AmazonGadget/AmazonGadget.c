#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "common.h"
//#include <Python.h>
#include <pthread.h>
#include <unistd.h>
#include "VendorFunction.h"

//cmd
char setup_cmd[85] = "sudo python3 ../../lib/Alexa-Gadgets-Raspberry-Pi-Samples/launch.py --setup";
char choose[20];
char wake_pair_cmd[85] = "sudo python3 ../../src/AmazonGadget/GadgetLed.py --pair";
char wake_clear_cmd[85] = "sudo python3 ../../src/AmazonGadget/GadgetLed.py --clear";
char input;//keyboard input

//int to bool
bool intToBool(int i){
  return (i == 1 ? true : false);
}

//pair thread
pthread_t threadPair;
//int ret_thrd1;
void *pair(void* args) {
	printf("\ngadget start pair ok\n");
	system(wake_pair_cmd);//start gadget
}
// pthread_create(&threadPair, NULL, pair, NULL);

//gadget run 
void runGadget(void){
	//cancel auto setup
	//setup 
	//system(setup_cmd);

	//led file 
	int gadget_led_status = 0;
	int temp_status = 0;
	FILE *fp = NULL;

	pthread_create(&threadPair, NULL, pair, NULL);//pair

 	//fp = fopen("gadget_led", "r");//open file 
	//gadget_led_status = fgetc(fp);//get gadget led status
	// printf("\ngadget start pair ok\n");
	// system(wake_pair_cmd);//start gadget

	//gadget loop
	while(1){
		
		// printf("\n+---------------------------------------------+\n");
		// printf("|             gadget running                  |\n");
		// printf("|        make sure setup gadget in lib        |\n");
		// printf("+---------------------------------------------+\n");
		// exit if this channel is not avaliable
		if (checkChannel("Gadget") == 0){
			printf("Gadget Channel closed. \n");
			system(wake_clear_cmd);
			printf("\ngadget unpair and clear ok\n");

			system("sudo ps aux|grep python|grep -v grep|cut -c 9-15|xargs kill -9");//kill python
			printf("\ngadget quit\n");
			//fclose(fp);//close file
			break;
			return;
		}
		//temp_status init = 0
		fp = fopen("gadget_led", "r");//open file
		fseek( fp, 0, SEEK_SET );
		gadget_led_status = fgetc(fp)-48;//get gadget led status   "0" = 48 in ascii
		fclose(fp);//close file

		if(gadget_led_status != temp_status){
			if(gadget_led_status == 0 || gadget_led_status == 1){
				OperateDevice(intToBool(gadget_led_status),"Gadget");
				printf("\n****Gadget Set light status to %d ****\n", gadget_led_status);
				temp_status = gadget_led_status;//update temp_status
			}
		}
		sleep(2);
	}

	//fclose(fp);//close file
	return;
} 

