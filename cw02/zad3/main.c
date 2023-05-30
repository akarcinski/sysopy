#include<sys/stat.h>
#include<stdio.h>
#include<dirent.h>

int main(int argc, char** argv) {
    DIR* dir;
    if ( (dir= opendir(".")) == NULL ) {
        printf("BLAD OTWARCIA KATALOGU\n");
        return -1;
    }
    struct dirent* entry;
    struct stat info;
    long long result = 0;

    while ((entry = readdir(dir)) != NULL) {
        stat(entry->d_name, &info);
        if (!S_ISDIR(info.st_mode)) {
            printf("%ld %s\n", info.st_size, entry->d_name);
            result += info.st_size;
        }
    }
    printf("\nsum size: %lld\n", result);
    return 0;
}
