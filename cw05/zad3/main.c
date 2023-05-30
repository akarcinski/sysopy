#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

char buff[2048] = "";

int main(int argc, char** argv) {
    double h = strtod(argv[1], NULL);
    int n = atoi(argv[2]);
    if (n*h > 1)
        n = (int)(1/h);
    double interval = 1.0/n;

    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    mkfifo("/tmp/integral_util", 0666);

    for (int i = 0; i < n; i++)
        if (!fork()) {
            char arg2[2048];
            char arg3[2048];
            snprintf(arg2, 2048, "%lf", i*interval);
            snprintf(arg3, 2048, "%lf", (i+1)*interval);
            execl("./util", "util", arg2, arg3, argv[1], NULL);
        }

    double integral = 0.0;
    int fifo = open("/tmp/integral_util", O_RDONLY);
    int i=0;
    while(i<n) {
        size_t size = read(fifo, buff, 2048);
        buff[size] = 0;

        char delimiters[] = "\n";
        char* s;
        s = strtok(buff, delimiters);

        while (s) {
            integral += strtod(s, NULL);
            s = strtok(NULL, delimiters);
            i++;
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);

    if (end.tv_nsec-begin.tv_nsec < 0)
        printf("time: %ld,%ld s\n", end.tv_sec-begin.tv_sec, end.tv_nsec-begin.tv_nsec+1000000000);
    else
        printf("time: %ld ns\n", end.tv_nsec-begin.tv_nsec);

    printf("h: %0.9f  n: %d\n", h, n);
    printf("integral: %lf\n", integral);
    remove("/tmp/integral_util");
    return 0;
}