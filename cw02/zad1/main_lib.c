#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <time.h>

int size_of_file = 0;

char* read_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("blad otwarcia pliku\n");
        return NULL; //blad otwarcia pliku
    }
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    size_of_file = size;
    rewind(file);

    unsigned long err;
    char* content = calloc(size+1, sizeof(char));
    err = fread(content, sizeof(char), size, file);
    err = fclose(file);
    err += 1;

    return content;
}


void write_file(char* content, char* path) {
    int result = mkstemp(path);

    FILE* file = fopen(path, "w");
    if (file == NULL) {
        printf("blad otwarcia pliku\n");
        return; //blad otwarcia pliku
    }
    result = fputs(content, file);
    if(result == EOF) {
        printf("sth went wrong\n");
    }
    fclose(file);
    free(content);
}


int main(int argc, char** argv) {
    if (argc < 4) {
        printf("PODANO ZA MALO ARGUMENTOW!\n");
        return 1;
    }
//    else if (argc > 4) {
//        printf("PODANO ZA DUZO ARGUMENTOW!\n");
//        return 1;
//    }
    char from = argv[1][0];
    char to = argv[2][0];
    char* file_input = argv[3];
    char* file_output = argv[4];
    struct timespec start, end;
//    printf("%c %c %s %s\n", from, to, file_input, file_output);
    clock_gettime(CLOCK_REALTIME, &start);
    char * content = read_file(file_input);
    for (int i=0; i<size_of_file; i++) {
        if (content[i] == from) {
            content[i] = to;
        }
    }
//    printf("%s", content);
//    printf("%d\n", size_of_file);
    write_file(content, file_output);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("Time: % ldns\n", end.tv_nsec-start.tv_nsec);
    return 0;
}