#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char buff[1024] = "";

double f(double x) {
    return 4/(x*x + 1);
}

double rectangle(double a, double b, double h) {
    double res = 0;
    for (double begin = a; begin < b; begin += h) {
        res += f(begin)*h;
    }
    return res;
}

int main(int argc, char** argv) {

    double a = strtod(argv[1], NULL);
    double b = strtod(argv[2], NULL);
    double h = strtod(argv[3], NULL);

    double part = rectangle(a, b, h);
//    printf("part: %lf\n", part);
    size_t size = snprintf(buff, 1024, "%lf\n", part);

    int fifo = open("/tmp/integral_util", O_WRONLY);
    write(fifo, buff, size);
    close(fifo);
    return 0;
}