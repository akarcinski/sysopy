#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

#include <sys/epoll.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#define MSG_LEN 1024
#define MAX_USERS 8
#define WAITING_TIME 30

typedef enum {
    msg_ping,
    msg_username_taken,
    msg_server_full,
    msg_disconnect,
    msg_get,
    msg_init,
    msg_list,
    msg_tone,
    msg_tall,
    msg_stop,
  } message_type;

typedef struct message {
  message_type type;
  char text[MSG_LEN]; 
  char other_nickname[MSG_LEN];
} message;

typedef enum event_type {
  socket_event,
  client_event
} event_type;

typedef struct event_data {
  event_type type;
  union {
    int socket;
    struct client* client;
  } payload;
} event_data;

typedef struct client {
  int fd;
  enum client_state {
    empty = 0,
    init,
    ready
  } state;
  char nickname[MSG_LEN];
  char symbol;
  struct game_state* game_state;
  bool responding;
} client;
