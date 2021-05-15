# Cguard_CCS

# GuardOn Version

1. Combine four channels: AWS MQTT, Zigbee, Homekit, Amazon gadget.
2. Run with Channel Guard and DMC Control APP.
3. Ensure only one Channel controlling LED to protect the device.



# GuardOff Version

1. Combine four channels: AWS MQTT, Zigbee, Homekit, Amazon gadget.
2. Run without Channel Guard, DMC Control APP.
3. Four Channels can control LED at the same time and  don't interface each other. 
4. Function Changes:

>In GuardOff   version,
>int checkChannel(char *channel_name) {return 1;}
>int disableChannel(char *channel_name) {return 0;}
>int closeotherChannel(char *channel_name){return 1;}



## Lib

### Amazon MQTT

path: ./lib/aws-iot-device-sdk-embedded-C

git clone https://github.com/aws/aws-iot-device-sdk-embedded-C.git

git checkout tags/v3.1.5



# Amazon Gadget

path: /lib/Alexa-Gadgets-Raspberry-Pi-Samples

The original Lib : Amazon Gadget

github：https://github.com/alexa/Alexa-Gadgets-Raspberry-Pi-Samples

> I added my amazon ID,Alexa Gadget Secret and LED-control script in the uploaded lib.

> （In gadget‘s branch ，/lib includes all codes of mqtt and gadget lib）



## gadget-fix

1. add VenderFunctionPy.py

2. add VenderFunction.c/h

3. fix bugs in gadget's branch

4. move amazon ID,Alexa Gadget Secret and LED-control script to   ./test/AmazonGadget/

5. ./lib/Alexa-Gadgets-Raspberry-Pi-Samples clones from  https://github.com/alexa/Alexa-Gadgets-Raspberry-Pi-Samples。(master branch)

   First run ./Gadget may need  configure the environment of amazon gadget. (some automatic pip3 install)


# Alexa Gadget Setup
1. To create a gadget that works with your Echo device, you first need to register a new gadget in the Alexa Voice Service Developer Console to get Amazon ID and Alexa Gadget Secret.
2. Fill Amazon ID and Alexa Gadget Secret in ./src/AmazonGagdet/GadgetLed.ini.
3. Setup Gagdet by
> sudo python3 ./lib/Alexa-Gadgets-Raspberry-Pi-Samples/launch.py --setup
