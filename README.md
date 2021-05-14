# CGuard_CCS

# GuardOn Version

1. Combine four channels: AWS MQTT, Zigbee, Homekit, Amazon gadget.
2. Run with Channel Guard and DMC Control APP.
3. Ensure only one channel controlling LED light to protect the device.



# GuardOff Version

1. Combine four channels: AWS MQTT, Zigbee, Homekit, Amazon gadget.
2. Run without Channel Guard, DMC Control APP.
3. Four Channels can control LED at the same time and don't interact with each other. 
4. Function Changes:

>In GuardOff   version,
>int checkChannel(char *channel_name) {return 1;}
>int disableChannel(char *channel_name) {return 0;}
>int closeotherChannel(char *channel_name){return 1;}
