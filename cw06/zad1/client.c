#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "settings.h"

key_t server_key;
key_t client_key;
int server_queue_id;
int client_queue_id;
int client_id;

void handler_LIST() {
    struct message_buff* message_buff = malloc(MSG_SIZE);
    message_buff->client_id = client_id;
    message_buff->type = 2;

    time_t client_time = time(NULL);
    message_buff->time = *localtime(&client_time);

    msgsnd(server_queue_id, message_buff, MSG_SIZE, 0);
}

void handler_2ALL(char* content) {
    struct message_buff* message_buff = malloc(MSG_SIZE);
    message_buff->client_id = client_id;
    message_buff->type = 3;
    strcpy(message_buff->message, content);

    time_t client_time = time(NULL);
    message_buff->time = *localtime(&client_time);

    printf("Message sent to all\n");
    msgsnd(server_queue_id, message_buff, MSG_SIZE, 0);
}

void handler_2ONE(int receiver, char* content) {
    struct message_buff* message_buff = malloc(MSG_SIZE);
    message_buff->client_id = client_id;
    message_buff->receiver = receiver;
    message_buff->type = 4;
    strcpy(message_buff->message, content);

    time_t client_time = time(NULL);
    message_buff->time = *localtime(&client_time);

    printf("Message sent to: %d\n", receiver);
    msgsnd(server_queue_id, message_buff, MSG_SIZE, 0);
}

void handler_STOP() {
    struct message_buff* message_buff = malloc(MSG_SIZE);
    message_buff->client_id = client_id;
    message_buff->type = 5;

    time_t client_time = time(NULL);
    message_buff->time = *localtime(&client_time);

    msgsnd(server_queue_id, message_buff, MSG_SIZE, 0);
    msgctl(client_queue_id, IPC_RMID, NULL);
    printf("Disconnected\n");
    exit(0);
}

void handler_SERVER() {
    struct message_buff* message_buff = malloc(MSG_SIZE);
    while (msgrcv(client_queue_id, message_buff, MSG_SIZE, 0, 0) >= 0) {
        if (message_buff->type == 5) {
            kill(0, SIGINT);
            return;
        }

        printf("%02d:%02d:%02d received:\n%s\n", message_buff->time.tm_hour, message_buff->time.tm_min,
               message_buff->time.tm_sec,message_buff->message);
    }
}

int main() {
    signal(SIGINT, handler_STOP);
    srand((time(NULL)));
    client_key = ftok(HOME_PATH, rand() % 500 + 2);
    client_queue_id = msgget(client_key, IPC_CREAT | 0666);
    server_key = ftok(HOME_PATH, SERVER_ID);
    server_queue_id = msgget(server_key, 0);

    struct message_buff* message_buff = malloc(MSG_SIZE);
    message_buff -> type = 1;
    message_buff -> key = client_key;
    time_t client_time = time(NULL);
    message_buff->time = *localtime(&client_time);

    msgsnd(server_queue_id, message_buff, MSG_SIZE, 0);
    msgrcv(client_queue_id, message_buff, MSG_SIZE, 0, 0);

    client_id = message_buff -> client_id;


    int running = 1;
    if (fork() == 0) {
        while (running) {
            handler_SERVER();
        }
        exit(0);
    }

    size_t tmp = 0;
    char* line = NULL;
    char* command = NULL;
    char delim[] = " \n\t";

    while (1) {
        size_t size;
        size = getline(&line, &tmp, stdin);
        line[size-1] = '\0';

        command = strtok(line, delim);
        if (command == NULL) continue;

        if (strcmp("LIST", command) == 0) {
            handler_LIST();
        }
        else if (strcmp("2ALL", command) == 0) {
            char * content = NULL;
            content = strtok(NULL, delim);
            if (content != NULL) {
                handler_2ALL(content);
            }
        }
        else if (strcmp("2ONE", command) == 0) {
            command = strtok(NULL, delim);
            if (command != NULL) {
                int id = atoi(command);
                char * content = NULL;
                content = strtok(NULL, delim);
                if (content != NULL) {
                    handler_2ONE(id, content);
                }
            }
        }
        else if (strcmp("STOP", command) == 0) {
            running = 0;
//            handler_STOP();
            kill(0, SIGINT);
        }
        else {
            printf("Unavailable command\n");
        }
    }
}