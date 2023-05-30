#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

char* reverse(char* content) {
    int n = strlen(content);
    char swap;
    for (int i=0; i<(n/2); ++i) {
        swap = content[i];
        content[i] = content[n-i-1];
        content[n-i-1] = swap;
    }

    return content;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("PODANO ZA MALO ARGUMENTOW!\n");
        return 1;
    }
    char* file_input = argv[1];
    char* file_output = argv[2];
    struct timespec start, end;
    FILE* file = fopen(file_input, "r");
    FILE* destination = fopen(file_output, "w");
    if (file == NULL || destination == NULL) {
        printf("BLAD OTWARCIA PLIKU\n");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file,0,SEEK_SET);

    char buffer[SIZE+1] = "";
    int tmp = size / SIZE;
    int rest = size % SIZE;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i=1; i<=tmp; i++) {
        fseek(file, -SIZE * i, SEEK_END);
        fread(buffer, sizeof(char), SIZE, file);
        fwrite(reverse(buffer), sizeof(char), SIZE, destination);
    }
    fseek(file, 0, SEEK_SET);
    fread(buffer, sizeof(char), rest, file);
    for (int i=rest; i<SIZE; i++)
        buffer[i]=0;
    fwrite(reverse(buffer), sizeof(char), rest, destination);
    clock_gettime(CLOCK_REALTIME, &end);
    fclose(file);
    fclose(destination);
    printf("TIME: %ld ns\n", end.tv_nsec-start.tv_nsec);
    return 0;
}