#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#define NO_REINDEERS 9
#define NO_ELVES 9
#define REQUIRED_NO_ELVES 3

pthread_t reindeer[NO_REINDEERS];
pthread_t elf[NO_ELVES];
pthread_t santa;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_barrier_t reindeer_barrier, elves_barrier;

int giveaway = 0;
int running = 1;
int reindeers = 0;
int elves = 0;

void *reindeer_handler(void * args) {
	while(running) {
		sleep(3 + rand() % 3);
		pthread_mutex_lock(&mutex);
		reindeers++;
		printf("<REINDEER> Reindeer %d is on holidays. Reindeers: %d\n", gettid(), reindeers);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		pthread_barrier_wait(&reindeer_barrier);
	}
	return NULL;
}

void *elf_handler(void * args) {
	while(running) {
		sleep(2 + rand() % 3);
		pthread_mutex_lock(&mutex);
		if (elves < REQUIRED_NO_ELVES) {
			elves++;
			printf("<ELF> Elf %d is waiting for Santa. Elves: %d\n", gettid(), elves);
			pthread_cond_signal(&cond);
		}
		else {
			pthread_mutex_unlock(&mutex);
			printf("<ELF> Elf %d solves the problem. Elves: %d\n", gettid(), elves);
			continue;
		}
		pthread_mutex_unlock(&mutex);
		pthread_barrier_wait(&elves_barrier);
	}
	return NULL;
}

void *santa_handler(void * args) {
	while(running) {
	
		pthread_mutex_lock(&mutex);
		while (reindeers < NO_REINDEERS && elves < REQUIRED_NO_ELVES) {
			pthread_cond_wait(&cond, &mutex);
		}
		
		if (reindeers == NO_REINDEERS) {
			giveaway++;
			printf("<SANTA> Reindeers awakes Santa! reindeers: %d, Giveaway: %d\n", reindeers, giveaway);
			sleep(2);
			reindeers = 0;
			pthread_barrier_wait(&reindeer_barrier);
		}
		else if (elves == REQUIRED_NO_ELVES) {
			giveaway++;
			printf("<SANTA> Elves awakes Santa! elves: %d, Giveaway: %d\n", elves, giveaway);
			sleep(2);
			elves=0;
			pthread_barrier_wait(&elves_barrier);
		}
		else {
			printf("OOPS, WE HAVE PROBLEMS\n");
		}
		pthread_mutex_unlock(&mutex);
		
		
		
		if (giveaway == 3) {
			running = 0;
			//exit(0);
		}
	}
	
	exit(0);
}

int main() {
	pthread_barrier_init(&reindeer_barrier, NULL, NO_REINDEERS + 1);
	pthread_barrier_init(&elves_barrier, NULL, REQUIRED_NO_ELVES + 1);
	
	for (int i=0; i<NO_REINDEERS; i++) {
		pthread_create(&reindeer[i], NULL, reindeer_handler, NULL);
	}
	
	for (int i=0; i<NO_ELVES; i++) {
		pthread_create(&elf[i], NULL, elf_handler, NULL);
	}
	
	pthread_create(&santa, NULL, santa_handler, NULL);
	
	
	while(1);
	
	return 0;
}

