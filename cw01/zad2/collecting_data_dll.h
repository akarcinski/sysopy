#ifndef LIB_COLL_DATA
#define LIB_COLL_DATA

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

typedef struct collecting_data{
    char** data;
    unsigned int size;
    unsigned int max_size;
}collecting_data;

void (*collecting_data_init)(unsigned int max_size, collecting_data* a);
collecting_data* (*collecting_data_new)(unsigned int max_size);
void (*collecting_data_add)(collecting_data* a, char* file_name);
char* (*collecting_data_get)(collecting_data* a, unsigned int index);
void (*collecting_data_free_index)(collecting_data* a, unsigned int index);
void (*collecting_data_destruct)(collecting_data* a);

#endif