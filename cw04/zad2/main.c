#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void info_handler(int sig, siginfo_t *info, void *ucontext) {
    printf("signo: %d\n", info->si_signo);
    printf("PID: %d\n", info->si_pid);
    printf("PPID: %d\n", getppid());
    printf("UID: %d\n", info->si_uid);
}

int ct = 0;
void nodefer_handler(int sig, siginfo_t *info, void *ucontext) {
    printf("NODEFER: counter %d\n", ct);

    ct++;
    if (ct<10) {
        kill(getpid(), SIGUSR1);
    }
    ct--;
    printf("NODEFER: counter %d\n", ct);
}

void siginfo_flag() {
    struct sigaction sig;
    sigemptyset(&sig.sa_mask);
    sig.sa_sigaction = info_handler;
    sig.sa_flags = 0;

    sigaction(SIGUSR1, &sig, NULL);
    printf("NOT SET:\n");
    raise(SIGUSR1);

    sig.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &sig, NULL);
    printf("\nPARENT:\n");
    raise(SIGUSR1);

    int pid = fork();
    if (pid == 0) {
        printf("\nCHILD:\n");
        raise(SIGUSR1);
        exit(0);
    } else {
        wait(NULL);
    }
}


void sigreset_flag() {
    ct = 0;
    struct sigaction sig;
    sigemptyset(&sig.sa_mask);
    sig.sa_sigaction = nodefer_handler;
    sig.sa_flags = SA_RESETHAND;

    sigaction(SIGUSR1, &sig, NULL);
    kill(getpid(), SIGUSR1);
}

void signodefer_flag(){
    struct sigaction sig;
    sigemptyset(&sig.sa_mask);
    sig.sa_sigaction = nodefer_handler;
    sig.sa_flags = SA_NODEFER;

    sigaction(SIGUSR1, &sig, NULL);
    kill(getpid(), SIGUSR1);
}

int main(int argc, char **argv) {
    printf("SIGINFO\n");
    siginfo_flag();
    printf("\nNODEFER\n");
    signodefer_flag();
    printf("\nRESETHANDLER\n");
    sigreset_flag();
    return 0;
}
