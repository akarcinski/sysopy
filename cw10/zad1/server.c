#include "settings.h"
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int epoll_fd;
int number_clients;

client clients[MAX_USERS];

void delete_client(client* client) {
    printf("Remove client %s\n", client->nickname);
    client->state = empty;
    client->nickname[0] = '\0';
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client->fd, NULL);
    close(client->fd);
    number_clients--;
}

void send_msg(client* client, message_type type, char text[MSG_LEN]) {
    message msg;
    msg.type = type;
    strncpy(msg.text, text, MSG_LEN);
    write(client->fd, &msg, sizeof(msg));
}

void on_client_message(client* client) {
    if (client->state == init) {
        int nick_size = read(client->fd, client->nickname, sizeof(client->nickname) - 1);
        pthread_mutex_lock(&mutex);
        int i;
        for (i = 0; i < MAX_USERS; i++) {
            if (i != number_clients && strncmp(client->nickname, clients[i].nickname, sizeof(clients->nickname)) == 0) {
                break;
            }
        } 
        if (i == MAX_USERS) {
            client->nickname[nick_size] = '\0';
            client->state = ready;
            printf("New client %s\n", client->nickname);
            number_clients++;
        } 
        else {
            message msg = {.type = msg_username_taken};
            printf("Nickname %s already taken\n", client->nickname);
            write(client->fd, &msg, sizeof(msg));
            strcpy(client->nickname, "new client");
            delete_client(client); 
        }
        pthread_mutex_unlock(&mutex);
    } 
    else {
        message msg;
	read(client->fd, &msg, sizeof(msg));
	printf("%s ", client->nickname);
	if (msg.type == 0)
		printf("pinging\n");
	else if (msg.type == 6)
		printf("listing\n");
	else if (msg.type == 7)
		printf("sending to one %s\n", msg.other_nickname);
	else if (msg.type == 8)
		printf("sending to all\n");
	else if (msg.type == 9)
		printf("stopping\n");	

        if (msg.type == msg_ping) {
            pthread_mutex_lock(&mutex);
            printf("%s is responding\n", client->nickname);
            client->responding = true;
            pthread_mutex_unlock(&mutex);
        } 
        else if (msg.type == msg_disconnect || msg.type == msg_stop) {
            pthread_mutex_lock(&mutex);
            delete_client(client);
            pthread_mutex_unlock(&mutex);
        } 
        else if (msg.type == msg_tall) {
            char out[MSG_LEN] = "";
            strcat(out, client->nickname);
            strcat(out, ": ");
            strcat(out, msg.text);

            for (int i = 0; i < MAX_USERS; i++) {
                if (clients[i].state != empty) {
                    send_msg(clients + i, msg_get, out);
                }
            }
        } 
        else if (msg.type == msg_list) {
            for (int i = 0; i < MAX_USERS; i++) {
                if (clients[i].state != empty) {
                    send_msg(client, msg_get, clients[i].nickname);
                }
            }
        } 
        else if (msg.type == msg_tone) {
            char out[MSG_LEN] = "";
            strcat(out, client->nickname);
            strcat(out, ": ");
            strcat(out, msg.text);

            for (int i = 0; i < MAX_USERS; i++) {
                if (clients[i].state != empty) {
                    if (strcmp(clients[i].nickname, msg.other_nickname) == 0) {
                        send_msg(clients + i, msg_get, out);
                    }
                }
            }
        }
    }
}

void init_socket(int socket, void* addr, int addr_size) {
    bind(socket, (struct sockaddr*)addr, addr_size);
    listen(socket, MAX_USERS);
    struct epoll_event event = {.events = EPOLLIN | EPOLLPRI};
    event_data* event_data_ptr = malloc(sizeof(event_data));
    event_data_ptr->type = socket_event;
    event_data_ptr->payload.socket = socket;
    event.data.ptr = event_data_ptr;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, &event);
}

client* new_client(int client_fd) {
    pthread_mutex_lock(&mutex);
    int i;
    for (i = 0; i < MAX_USERS; i++) {
        if (clients[i].state == empty) {
            break;
        }
    }
    if (i == MAX_USERS) {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    client* client = &clients[i];

    client->fd = client_fd;
    client->state = init;
    client->responding = true;
    pthread_mutex_unlock(&mutex);
    return client;
}

void* ping() {
    static message msg = {.type = msg_ping};
    while (true) {
        sleep(WAITING_TIME);
        pthread_mutex_lock(&mutex);
        printf("Pinging clients\n");
        for (int i = 0; i < MAX_USERS; i++) {
            if (clients[i].state != empty) {
                if (clients[i].responding) {
                    clients[i].responding = false;
                    write(clients[i].fd, &msg, sizeof(msg));
                } else {
                    delete_client(&clients[i]);
                }
            }
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("use port path\n");
        exit(0);
    }
    int port = atoi(argv[1]);
    char* socket_path = argv[2];
   
    epoll_fd = epoll_create1(0);
    struct sockaddr_un local_addr = {.sun_family = AF_UNIX};
    strncpy(local_addr.sun_path, socket_path, sizeof(local_addr.sun_path));

    struct sockaddr_in web_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {.s_addr = htonl(INADDR_ANY)},
    };

    unlink(socket_path);
    int local_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    init_socket(local_sock, &local_addr, sizeof(local_addr));

    int web_sock = socket(AF_INET, SOCK_STREAM, 0);
    init_socket(web_sock, &web_addr, sizeof(web_addr));

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, ping, NULL);

    printf("Server listening on *:%d and '%s'\n", port, socket_path);
    number_clients = 0;
    struct epoll_event events[128];
    while (true) {
        int nread = epoll_wait(epoll_fd, events, 128, -1);
        for (int i = 0; i < nread; i++) {
            event_data* data = events[i].data.ptr;
            if (data->type == socket_event) { 
                int client_fd = accept(data->payload.socket, NULL, NULL);
                client* client = new_client(client_fd);
                if (client == NULL) {
                    printf("Server is full\n");
                    message msg = {.type = msg_server_full};
                    write(client_fd, &msg, sizeof(msg));
                    close(client_fd);
                    continue;
                }

                event_data* event_data_ptr = malloc(sizeof(event_data));
                event_data_ptr->type = client_event;
                event_data_ptr->payload.client = client;
                struct epoll_event event = {.events = EPOLLIN | EPOLLET | EPOLLHUP, .data.ptr = event_data_ptr};

                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
            } 
            else if (data->type == client_event) {
                if (events[i].events & EPOLLHUP) {
                    pthread_mutex_lock(&mutex);
                    delete_client(data->payload.client);
                    pthread_mutex_unlock(&mutex);
                } 
                else {
                    on_client_message(data->payload.client);
                }
            }
        }
    }
}

