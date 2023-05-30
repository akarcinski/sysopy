#ifndef MEMHANDLING_H
#define MEMHANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

char * memory_open(const char * file, int size);
void memory_unlink(char * mem);
void memory_delete(const char * file);

#endif

