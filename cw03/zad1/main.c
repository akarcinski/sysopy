#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


int main(int argc, char ** argv) {
    if (argc < 2) {
        printf("podaj liczbe\n");
        return 1;
    }

    pid_t child_pid, wpid;
    int status = 0;

    char * inp = argv[1];
    int n = atoi(inp);

    for (int i=0; i<n; i++) {
        if ((child_pid = fork()) == 0) { // jestesmy w procesie potomnym
            printf("PID rodzica: %d,   PID procesu: %d\n", (int)getppid(), (int)getpid());
            exit(0);
        }
    }

//    printf("%d\n", n);
//    child_pid = fork();
//    if(child_pid != 0) {
//        printf("Proces rodzica: Proces rodzica ma pid %d\n", (int)getpid());
//        printf("Proces rodzica: Proces dziecka ma pid %d\n", (int)child_pid);
//    } else {
//        printf("Proces dziecka: Proces rodzica ma pid %d\n", (int)getppid());
//        printf("Proces dziecka: Proces dziecka ma pid %d\n", (int)getpid());
//    }

    while ((wpid = wait(&status)) > 0);
    printf("Podana liczba: %d\n", n);
    return 0;
}