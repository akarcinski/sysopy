#ifndef SEMHANDLING_H
#define SEMHANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

int semaphore_create(const char * file, int value);
int semaphore_open(const char * file);
void semaphore_close(const char * file);
void semaphore_pp(int sem);
void semaphore_mm(int sem);

#endif

