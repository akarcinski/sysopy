#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "settings.h"
#include "semhandling.h"

int semaphore_create(const char * file, int value) {
	key_t key = ftok(HOME_PATH, file[0]);
	int sem = semget(key, 1, 0666 |IPC_CREAT);
	semctl(sem, 0, SETVAL, value);
	return sem;
}
int semaphore_open(const char * file) {
	key_t key = ftok(HOME_PATH, file[0]);
	return semget(key, 1, 0);
}
void semaphore_close(const char * file) {
	key_t key = ftok(HOME_PATH, file[0]);
	int sem = semget(key, 1, 0);
	semctl(sem, 0, IPC_RMID);
}
void semaphore_pp(int sem) {
	struct sembuf adding = {0,1,0};
	semop(sem, &adding, 1);
}
void semaphore_mm(int sem) {
	struct sembuf diff = {0,-1,0};
	semop(sem, &diff, 1);
}

