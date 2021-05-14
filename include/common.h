#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>


// First run flag
extern int FIRST_RUN;

// To determine the accessibility status of each channel
struct type_channel_list{
    char *channel_name;
    pthread_t channel_thread;
    int enabled;
};
extern struct type_channel_list *CHANNEL_LIST;
// The length of the channel list should be defined by the developer. 
extern int CHANNEL_LIST_LENGTH;

// Initiate the channel list. First argument is the channels' names. Second argument
// is the length of the channel list (How many channels should be enabled).
// Initially, every channel is disabled.
// Return -1 if error occurs. Otherwise return 0.
int initChannelList(char **channel_names, size_t channel_num);

// Check the channel's availability. Return 1 if the channel is avaliable. Otherwise return 0.
// If there's no such channel or the status is broken(not 1 or 0), return -1.
int checkChannel(char *channel_name);

// Enable a channel. This function will only change the status of CHANNEL_LIST.
// Return -1 if there's no such channel name. Otherwise return 0.
int enableChannel(char *channel_name);

// Disable a channel. This function will only change the status of CHANNEL_LIST.
// Return -1 if there's no such channel name. Otherwise return 0.
int disableChannel(char *channel_name);

int closeotherChannel(char *channel_name);

void runAmazonMQTT(void);

void runZigbee(void);

void runGadget(void);


#endif
