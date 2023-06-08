#include "settings.h"

int connectUnix(char* path) {
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

    int socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));

    return socketFd;
}

int connectWeb(char* ipv4, int port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipv4, &addr.sin_addr) <= 0) {
        puts("Invalid address\n");
        exit(0);
    }

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    connect(socketFd, (struct sockaddr*)&addr, sizeof(addr));

    return socketFd;
}

int socketFd;
void handleSIGINT(int tmp) {
    message msg = {.type = msg_disconnect};
    tmp = write(socketFd, &msg, sizeof(msg));
    exit(0);
}

int main(int argc, char** argv) {
    if (strcmp(argv[2], "web") == 0 && argc == 5)
        socketFd = connectWeb(argv[3], atoi(argv[4]));
    else if (strcmp(argv[2], "unix") == 0 && argc == 4)
        socketFd = connectUnix(argv[3]);
    else {
        puts("use ./client nick web ip_port or ./client unix path\n");
        exit(0);
    }

    signal(SIGINT, handleSIGINT);
    char* nickname = argv[1];
    write(socketFd, nickname, strlen(nickname));

    int epollFd = epoll_create1(0);
    
    struct epoll_event stdinEvent = {.events = EPOLLIN | EPOLLPRI, .data = {.fd = STDIN_FILENO}};
    epoll_ctl(epollFd, EPOLL_CTL_ADD, STDIN_FILENO, &stdinEvent);
    
    struct epoll_event socketEvent = {.events = EPOLLIN | EPOLLPRI | EPOLLHUP, .data = {.fd = socketFd}};
    epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &socketEvent);

    struct epoll_event events[2];
    while (1) {
        int nread = epoll_wait(epollFd, events, 2, 1);
        for (int i = 0; i < nread; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                char buffer[512] = {};

                int size = read(STDIN_FILENO, &buffer, 512);
                buffer[size] = 0;

                char separator[] = " \t\n";
                char* token;
                token = strtok(buffer, separator);

                message_type messageType = -1;
                char otherNickname[MSG_LEN] = {};
                char text[MSG_LEN] = {};

                if (token == NULL)
                    continue;
		
		if (strcmp(token, "LIST") == 0) {
			messageType = msg_list;
		}
		else if (strcmp(token, "2ALL") == 0) {
			messageType = msg_tall;
			
			token = strtok(NULL, "\n");
			memcpy(text, token, strlen(token) * sizeof(char));
                        text[strlen(token)] = 0;
		}
		else if (strcmp(token, "2ONE") == 0) {
			messageType = msg_tone;
			
			token = strtok(NULL, separator);
			memcpy(otherNickname, token, strlen(token) * sizeof(char));
                        otherNickname[strlen(token)] = 0;
			
			token = strtok(NULL, "\n");
			memcpy(text, token, strlen(token) * sizeof(char));
                        text[strlen(token)] = 0;
		}
		else if (strcmp(token, "STOP") == 0) {
			messageType = msg_stop;
		}
		else {
			puts("Invalid command");
			messageType = -1;
		}

                message msg;
                msg.type = messageType;
                memcpy(&msg.other_nickname, otherNickname, strlen(otherNickname) + 1);
                memcpy(&msg.text, text, strlen(text) + 1);

            	write(socketFd, &msg, sizeof(msg));
            } 
            else {
                message msg;
		read(socketFd, &msg, sizeof(msg));
		
                if (msg.type == msg_username_taken) {
                    puts("This username is already taken\n");
                    close(socketFd);
                    exit(0);
                } 
                else if (msg.type == msg_server_full) {
                    puts("Server is full\n");
                    close(socketFd);
                    exit(0);
                } 
                else if (events[i].events & EPOLLHUP) {
                    puts("Disconnected\n");
                    exit(0);
                } 
                else if (msg.type == msg_ping) {
                    write(socketFd, &msg, sizeof(msg));
                } 
                else if (msg.type == msg_stop) {
                    puts("Stopping\n");
                    close(socketFd);
                    exit(0);
                } 
                else if (msg.type == msg_get) {
                    puts(msg.text);
                }
            }
        }
    }
}

