#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#define HOME_PATH getenv("HOME")
#define SERVER_ID 1
#define MAX_CLIENTS 10
#define MAX_MESSAGE_SIZE 512

struct message_buff {
    long type;
    key_t key;
    char message[MAX_MESSAGE_SIZE];
    int client_id;
    int receiver;
    struct tm time;
};

const int MSG_SIZE = sizeof(struct message_buff);

#endif