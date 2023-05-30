#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "settings.h"
#include "memhandling.h"
#include "semhandling.h"
#include "quehandling.h"

int sem_queue;

int main() {
	sem_queue = semaphore_open(SEM_QUEUE);
	
	char * queue = memory_open(HOME_PATH, QUEUE_SIZE);
	char * init = "0";
	strcpy(queue, init);
	if (queue_full(queue) == 1) {
		printf("CLIENT: Queue is full\n");
		memory_unlink(queue);
		return 1;
	}
	
	printf("CLIENT: New Client\n");
	queue_push(queue);
	semaphore_mm(sem_queue);
	
	printf("CLIENT: Client is being serviced\n");
	memory_unlink(queue);
	
	return 0;
}
