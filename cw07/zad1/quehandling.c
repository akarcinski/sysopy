#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"
#include "quehandling.h"

int queue_push(char * queue) {
	if (queue_full(queue) == 1) {
		printf("QUEUE OVERFLOW\n");
		return -1;
	}
	
	int val = atoi(queue);
	val++;
	char buffer[QUEUE_SIZE];
	snprintf(buffer, QUEUE_SIZE, "%d", val);
	strcpy(queue, buffer);

	return 1;
}

int queue_pop(char * queue) {
	if (queue_empty(queue) == 1) {
		printf("QUEUE EMPTY\n");
		return -1;
	}
	
	int val = atoi(queue);
	val--;
	char buffer[QUEUE_SIZE];
	snprintf(buffer, QUEUE_SIZE, "%d", val);
	strcpy(queue, buffer);
	
	return 1;
}

int queue_empty(char * queue) {
	int val = atoi(queue);
	if (val == 0) return 1;
	return 0;
}

int queue_full(char * queue) {
	int val = atoi(queue);
	if (val >= QUEUE_MAX - 1) return 1;
	return 0;
}

int queue_val(char * queue) {
	int val = atoi(queue);
	return val;
}

