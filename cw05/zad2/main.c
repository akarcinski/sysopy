#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

char buff[1024] = "";

double f(double x) {
    return 4/(x*x + 1);
}

double rectangle(double a, double b, double h) {
    double res = 0;
//    printf("%lf %lf %lf\n", a, b, h);
    for (double begin = a; begin < b; begin += h) {
        res += f(begin)*h;
//        printf("begin = %lf\n", begin);
    }
//    printf("res = %lf\n", res);
    return res;
}

int main(int argc, char **argv) {
    double h = strtod(argv[1], NULL);
    int n = atoi(argv[2]);
    if (h*(double)n > 1) h = 1.0/n;
    double interval = 1.0/n;
//    printf("h: %lf  n: %d  interval: %lf\n", h, n, interval);
    int *fd = calloc(n, sizeof(int));

    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    for (int i=0; i<n; i++) {
        int p[2];
        pipe(p);

        if (fork() == 0) { // dziecko
            close(p[0]);
//            printf("%d:\n", i);
            double rec = rectangle(i*interval, (i+1)*interval, h);
//            printf("%d: %lf\n", i, rec);
            snprintf(buff, 1024, "%lf", rec);
            write(p[1], buff, 1024);
            return 0;

        } else { // rodzic
            close(p[1]);
            fd[i] = p[0];
//            printf("closed: %d\n", i);
        }
    }

    while(wait(NULL) > 0);

    double integral = 0.0;
    char new_buff[1024] = "";
    for (int i=0; i<n; i++) {
        size_t size = read(fd[i], new_buff, 1024);
        buff[size] = 0;
        double part = strtod(new_buff, NULL);
        integral += part;
    }
    free(fd);

    clock_gettime(CLOCK_REALTIME, &end);

    if (end.tv_nsec-begin.tv_nsec < 0)
        printf("time: %ld,%ld s\n", end.tv_sec-begin.tv_sec, end.tv_nsec-begin.tv_nsec+1000000000);
    else
        printf("time: %ld ns\n", end.tv_nsec-begin.tv_nsec);

    printf("h: %0.9f  n: %d\n", h, n);
    printf("integral: %lf\n", integral);
    return 0;
}