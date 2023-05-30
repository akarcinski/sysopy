#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>



char *location;
char *template;
int flag = 0;

int check(char* path) {
//    printf("dziala\n");
    char buffer[256] = "";
    FILE *file = fopen(path, "r");
    size_t size = fread(buffer, sizeof(char), strlen(template), file);
    buffer[size] = 0;
//    printf("template: %s buff: %s path: %s \n",template, buffer, path);
    if (strcmp(buffer, template) == 0)
        return 1;
    return 0;
}

int traversal(char* directory) {
    DIR* dir;
    if ( (dir= opendir(directory)) == NULL ) {
        printf("BLAD OTWARCIA KATALOGU: %s\n", directory);
        return -1;
    }

    struct dirent* entry;
//    struct stat info;


    while ((entry = readdir(dir)) != NULL) {
        if (flag == 1)
            break;
        if (entry->d_type == DT_DIR && (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)) {
            int PID = fork();

            if (PID == 0) {
                char path_buffer[1024];
                snprintf(path_buffer, sizeof(path_buffer), "%s/%s", directory, entry->d_name);
                traversal(path_buffer);
            } else {
                int status = 0;
                wait(&status);
            }
        } else if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//            printf("dir: %s  file: %s\n", directory, entry->d_name);
//            char * file_to_check;
//            strcpy(file_to_check, directory);
//            strcat(file_to_check, "/");
//            strcat(file_to_check, entry->d_name);
            char path_buffer[1024];
            snprintf(path_buffer, sizeof(path_buffer), "%s/%s", directory, entry->d_name);
            int b = check(path_buffer);
            if (b == 1) {
                printf("path: %s  PID: %d\n", path_buffer, (int)getpid());
                flag = 1;
                break;
            }
        }
    }
    closedir(dir);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("podaj sciezke i ciag znakow\n");
        return 1;
    }
    location = argv[1];
    template = argv[2];
    setbuf(stdout, NULL);
    traversal(argv[1]);
//    DIR* dir;
//    if ( (dir= opendir(argv[1])) == NULL ) {
//        printf("BLAD OTWARCIA KATALOGU\n");
//        return -1;
//    }
//    struct dirent* entry;
//    struct stat info;
//
//
//    while ((entry = readdir(dir)) != NULL) {
//        stat(entry->d_name, &info);
//        if (!S_ISDIR(info.st_mode)) { // plik
//            printf("plik: %s\n", entry->d_name);
//        }
//        else {
////            if (strcmp(".", entry->d_name) != 0 && strcmp("..", entry->d_name) != 0) {
//            printf("katalog: %s\n", entry->d_name);
////            }
//        }
//    }

    return 0;
}