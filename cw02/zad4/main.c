#include<sys/stat.h>
#include<stdio.h>
#include<ftw.h>

long long sum = 0;

int traversal(const char* path, const struct stat *info, int flag) {
    if (flag == FTW_F) {
        struct stat st;
        stat(path, &st);
        sum += st.st_size;
        printf("%ld %s\n", st.st_size, path);
    }
    return 0;
}

int main(int argc, char** argv) {
    if(argc<2) {
        printf("PODAJ KATALOG\n");
        return -1;
    }

    char* path = argv[1];

    ftw(path, *traversal, 1);

    printf("\nsum size: %lld\n", sum);
    return 0;
}