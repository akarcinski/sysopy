#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
int PID = 1;

int signal_type(const char* command) {
    if (strcmp(command, "ignore") == 0) return 0;
    if (strcmp(command, "handler") == 0) return -1;
    if (strcmp(command, "mask") == 0) return 2;
    if (strcmp(command, "pending") == 0) return 3;
    return -1;
}

void handler(int sign) {
    fprintf(stderr,"<HANDLER> PID: %d received\n", (int)getpid());
}

void run(int command) {
    if (command == 0) {
        signal(SIGUSR1, SIG_IGN);
        fprintf(stderr,"<IGNORE>  PID: %d raised\n", (int)getpid());
        raise(SIGUSR1);
    }
    else if (command == 1) {
        return;
    }
    else if (command == 2) {
        signal(SIGUSR1, handler);

        if( PID != 0 ) {
            sigset_t sigset;
            sigemptyset(&sigset);
            sigaddset(&sigset, SIGUSR1);
            sigprocmask(SIG_BLOCK, &sigset, NULL);
            fprintf(stderr,"<MASK>    PID: %d raised\n", (int)getpid());
            raise(SIGUSR1);
        }
    }
    else if (command == 3) {
        signal(SIGUSR1, handler);

        if( PID != 0 ) {
            sigset_t sigset;
            sigemptyset(&sigset);
            sigaddset(&sigset, SIGUSR1);
            sigprocmask(SIG_BLOCK, &sigset, NULL);
            fprintf(stderr,"<PENDING> PID: %d raised\n", (int)getpid());
            raise(SIGUSR1);
        }

        sigset_t pending_set;
        sigpending(&pending_set);
        if (sigismember(&pending_set, SIGUSR1))
            fprintf(stderr,"<PENDING> PID: %d pending\n", (int)getpid());
        else
            fprintf(stderr,"<PENDING> PID: %d blocked\n", (int)getpid());
    }
    else {
        return;
    }
}

int main(int argc, char **argv) {
    if (argc < 2 && PID == 1) {
        printf("Podaj 1 argument!\n");
        return 1;
    }
    int command = signal_type(argv[1]);
//    printf("%d\n", command);
    fflush(NULL);

    if (argc == 3)
        if (strcmp(argv[2], "0") == 0)
            PID = 0;

    run(command);

    if (argc == 2)
        execl(argv[0], argv[0], argv[1], "0", NULL);

    fflush(NULL);
    return 0;
}