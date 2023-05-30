#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <time.h>
#include <malloc.h>
#include <string.h>

#include <fcntl.h>

int size_of_file = 0;

char* read_file(const char* path) {
    int fd = open(path, O_RDONLY);

    off_t current_position = lseek(fd, (size_t)0,SEEK_CUR);
    size_t size = lseek(fd, (size_t)0, SEEK_END);
    lseek(fd, current_position, SEEK_SET);
    size_of_file = size;
    char* content = calloc(size+1, sizeof(char));
    read(fd, content, size);
    close(fd);

    return content;
}


void write_file(char* content, char* path) {
    int result = mkstemp(path);
    int fd = open(path, O_WRONLY | O_CREAT);
    result = write(fd, (void*)content, size_of_file*sizeof(char));
    result++;
    close(fd);
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