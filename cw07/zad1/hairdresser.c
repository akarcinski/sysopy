#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "settings.h"
#include "memhandling.h"
#include "semhandling.h"
#include "quehandling.h"

int sem_queue;
int sem_armchair;

int main() {
	srand(time(NULL));
	sem_queue       = semaphore_open(SEM_QUEUE);
	sem_armchair    = semaphore_open(SEM_ARMCHAIR);
	
	char * queue = memory_open(HOME_PATH, QUEUE_SIZE);
	
	while (1) {
		if (queue_empty(queue) == 1) {
			printf("HAIRDRESSER: queue empty\n");
			int flag = 0;
			int t = WAITING_MAX;
			while (t--) {
				if (queue_empty(queue) == 0) {
					flag = 1;
					printf("HAIRDRESSER: new client\n");
					break;
				}
				usleep(1);
				//printf("HAIRDRESSER: WAIT\n");
			}
			
			if (flag == 0) {
				break;
			}
		}
		
		semaphore_mm(sem_armchair); // take free armchair
		semaphore_pp(sem_queue); // take new client
		queue_pop(queue);
		
		printf("HAIRDRESSER: working\n");
		usleep(HAIRCUT_BASE_TIME + (rand() % HAIRCUTS_TYPES) + 1);
		printf("HAIRDRESSER: done\n");
		
		semaphore_pp(sem_armchair);	
	}
	
	memory_unlink(queue);
	
	return 0;
}
