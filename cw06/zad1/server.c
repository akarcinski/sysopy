#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#include "settings.h"

key_t clients_queues[MAX_CLIENTS];
int server_queue;
FILE* logs;

void logged(struct message_buff* message_buff) {
    fprintf(logs, "%02d:%02d:%02d    ", message_buff->time.tm_hour, message_buff->time.tm_min, message_buff->time.tm_sec);
    switch (message_buff->type) {
        case 1:
            fprintf(logs, "New client ID: %d\n", message_buff->client_id);
            break;
        case 2:
            fprintf(logs, "LIST from client ID: %d\n", message_buff->client_id);
            break;
        case 3:
            fprintf(logs, "2ALL from client ID: %d\n", message_buff->client_id);
            break;
        case 4:
            fprintf(logs, "2ONE from client ID: %d to: %d \n", message_buff->client_id, message_buff->receiver);
            break;
        case 5:
            fprintf(logs, "STOP from client ID: %d\n", message_buff->client_id);
            break;
    }
}

void handle_INIT(struct message_buff* message_buff) {
    int new_client_id = -1;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients_queues[i] == -1) {
            new_client_id = i;
            break;
        }
    }
    if (new_client_id == -1) {
        printf("Clients array overflow\n");
        return;
    }
    printf("New client ID: %d\n", new_client_id);
    clients_queues[new_client_id] = message_buff->key;
    message_buff->client_id = new_client_id;
    int new_client_queue_id = msgget(message_buff->key, 0);
    msgsnd(new_client_queue_id, message_buff, MSG_SIZE, 0);
}

void handle_LIST() {
    printf("LIST:\n");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients_queues[i] != -1) {
            printf("Client: %d\n", i);
        }
    }
}

void handle_2ALL(struct message_buff* message_buff) {
    printf("Client ID: %d sent to all\n", message_buff->client_id);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients_queues[i] != -1 && i != message_buff->client_id) {
            int client = msgget(clients_queues[i], 0);
            msgsnd(client, message_buff, MSG_SIZE,0);
        }
    }
}

void handle_2ONE(struct message_buff* message_buff) {
    int client_id = message_buff->receiver;
    if (client_id < 0 || client_id >= MAX_CLIENTS || clients_queues[client_id] == -1) {
        printf("Client ID doesn't exist\n");
        return;
    }
    printf("Client ID: %d sent to %d\n", message_buff->client_id, client_id);
    int client = msgget(clients_queues[client_id], 0);
    msgsnd(client,message_buff, MSG_SIZE, 0);
}

void handle_STOP(struct message_buff* message_buff) {
    clients_queues[message_buff->client_id] = -1;
    printf("Client ID: %d disconnected\n", message_buff->client_id);
}

void handle_CLOSE() {
    struct message_buff* message_buff = malloc(MSG_SIZE);
    message_buff->type = 5;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients_queues[i] != -1) {
            message_buff->client_id = clients_queues[i];
            int client = msgget(clients_queues[i], 0);
            msgsnd(client,message_buff, MSG_SIZE, 0);
            msgrcv(server_queue, message_buff, MSG_SIZE, 5, 0);
        }
    }
    msgctl(server_queue, IPC_RMID, NULL);

    fprintf(logs, "Server closed\n\n");
    fclose(logs);
    exit(0);
}

int main() {
    logs = fopen("logs.txt", "a");
    signal(SIGINT, handle_CLOSE);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients_queues[i] = -1;
    }

    key_t main_key = ftok(HOME_PATH, SERVER_ID);
    server_queue = msgget(main_key, IPC_CREAT | 0666);

    struct message_buff* message_buff = malloc(MSG_SIZE);
    fprintf(logs, "Server start\n");
    printf( "Server start\n");

    while (1) {
        msgrcv(server_queue, message_buff, MSG_SIZE, -10, 0);
        switch (message_buff->type) {
            case 1:
                handle_INIT(message_buff);
                logged(message_buff);
                break;
            case 2:
                handle_LIST();
                logged(message_buff);
                break;
            case 3:
                handle_2ALL(message_buff);
                logged(message_buff);
                break;
            case 4:
                handle_2ONE(message_buff);
                logged(message_buff);
                break;
            case 5:
                handle_STOP(message_buff);
                logged(message_buff);
                break;
            default:
                printf("Unavailable command\n");
        }

    }
}