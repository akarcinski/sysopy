#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "settings.h"
#include "memhandling.h"

char * memory_open(const char * file, int size) {
	key_t key = ftok(file, 0);
	int mem = shmget(key, size, 0666 |IPC_CREAT);
	char * queue = shmat(mem, NULL, 0);
	return queue;
}
void memory_unlink(char * mem) {
	shmdt(mem);
}
void memory_delete(const char * file) {
	key_t key = ftok(file, 0);
	int mem = shmget(key, 0, 0666 |IPC_CREAT);
	shmctl(mem, IPC_RMID, NULL);
}

