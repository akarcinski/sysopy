#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("podaj sciezke\n");
        return 1;
    }
    char* path = argv[1];
    printf("program: %s ", argv[0]);
    setbuf(stdout, NULL);
    execl("/bin/ls", "ls", path, NULL);

    return 0;
}