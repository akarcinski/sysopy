#include "collecting_data.h"

void collecting_data_init(unsigned int max_size, collecting_data* a) {
    if(!a) {
        printf("collecting_data: collecting_data_init: wskaznik null");
        return; // wskaznik null
    }
	a->size = 0;
	a->max_size = max_size;
	char** data = (char**)calloc(max_size, sizeof(char*));
	if(data == NULL) { // blad alokacji
        printf("collecting_data: collecting_data_init: blad alokacji");
        return;
    }
    a->data = data;
}

collecting_data* collecting_data_new(unsigned int max_size) {
    collecting_data* a = malloc(sizeof(collecting_data));
    collecting_data_init(max_size, a);
    return a;
}

void collecting_data_add(collecting_data* a, char* file_name) {
    int err;
    if(!a) {
        printf("collecting_data: collecting_data_add: wskaznik null");
        return; // wskaznik null
    }
    if(a->size >= a->max_size) {
        printf("collecting_data: collecting_data_add: nie ma miejsca w strukturze");
        return; // nie ma miejsca w strukturze
    }
	char tmp_file_name[] = "/tmp/wc_XXXXXX";
    int tmp = mkstemp(tmp_file_name);

    if (tmp == -1 || tmp == 0) {
        printf("collecting_data: collecting_data_add: blad utworzenia pliku");
        return; // blad utworzenia pliku
    }
    unsigned long buf = strlen(file_name) + strlen(tmp_file_name);
    char* command = calloc(64+buf, sizeof(char));
    snprintf(command, 64+buf, "wc %s > %s", file_name, tmp_file_name);
    err = system(command);
    free(command);

    FILE* file = fopen(tmp_file_name, "r");
    if (file == NULL) {
        printf("collecting_data: collecting_data_add: blad otwarcia pliku");
        return; //blad otwarcia pliku
    }
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* wc_out = calloc(size+1, sizeof(char));
    err = fread(wc_out, sizeof(char), size, file);
    err = fclose(file);
    if (err == 0)
        err = 0;
    remove(tmp_file_name);
    for (int i=0; i<a->max_size; i++) {
        if (a->data[i] == NULL) {
            a->data[i]=wc_out;
            break;
        }
    }
    a->size++;
}

char* collecting_data_get(collecting_data* a, unsigned int index) {
    if(!a) {
        printf("collecting_data: collecting_data_get: wskaznik null");
        // wskaznik null
    }
    if (index < a->max_size)
	    return a->data[index];
    printf("collecting_data: collecting_data_get: odwolanie poza zakres");
    return a->data[0];
}

void collecting_data_free_index(collecting_data* a, unsigned int index) {
    if(!a) {
        printf("collecting_data: collecting_data_free_index: wskaznik null");
        return; // wskaznik null
    }
    if (index < a->max_size) {
        char * pointer = a->data[index];
        a->data[index] = NULL;
        free(pointer);
        return;
    }
    printf("collecting_data: collecting_data_free_index: odwolanie poza zakres");
}

void collecting_data_destruct(collecting_data* a) {
    if(!a) {
        printf("collecting_data: collecting_data_destruct: wskaznik null");
        return; // wskaznik null
    }
	for(int i=0; i<a->max_size; i++) {
		free(a->data[i]);
	}
	free(a->data);
    free(a);
}

//int main()
//{
//    collecting_data* a = collecting_data_new(16);
//    collecting_data_add(a, "example.txt");
//    printf("%s", collecting_data_get(a, 0));
//    collecting_data_destruct(a);
//    return 0;
//}