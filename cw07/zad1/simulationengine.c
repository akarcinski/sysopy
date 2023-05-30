#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#include "settings.h"
#include "memhandling.h"
#include "semhandling.h"
#include "quehandling.h"

int sem_queue;
int sem_armchair;

int my_pid;

int main() {
	printf("Let's start the party :(\n");
	sem_queue =       semaphore_create(SEM_QUEUE, QUEUE_MAX); // free queue seats
	sem_armchair =    semaphore_create(SEM_ARMCHAIR, NO_ARMCHAIRS); // free armchairs
	
	my_pid = getpid();
	for (int i=0; i<NO_HARIDRESSERS; i++) {
		if (getpid() == my_pid) {
			if (fork() == 0) {
				printf("%d hairdresser revives\n", i+1);
				execl("./hairdresser", "hairdresser", NULL);
			}
		}
	}
	//while(waitpid(0, NULL, 0) > 0);
	for (int i=0; i<NO_CLIENTS; i++) {
		if (getpid() == my_pid) {
			if (fork() == 0) {
				printf("%d client revives\n", i+1);
				execl("./client", "client", NULL);
			}
		}
	}
	
	while(waitpid(0, NULL, 0) > 0);
	
	
	
	semaphore_close(SEM_QUEUE);
	semaphore_close(SEM_ARMCHAIR);
	
	memory_delete(HOME_PATH);
	printf("The End :))\n");
	return 0;
}

