#ifdef DLL
    #include "collecting_data_dll.h"
#else
    #include "collecting_data.h"
#endif

#include "dll_loader.h"
#include <time.h>
#include <sys/times.h>

int BUFFOR_SIZE = 2048;
int exist = 0;
int running = 1;
collecting_data* data;

void split_command(char* line) {
    line[strlen(line)-1] = ' ';
    char* word;
    word = strtok(line, " ");

    if (word == NULL) {
        printf("[CLI]: PODAJ KOMENDE\n");
        return;
    }

    if (strcmp(word, "destroy") == 0) { // DESTROY
        if (!exist) {
            printf("[CLI]: STRUKTURA NIE ISTNIEJE\n");
            return;
        }
        collecting_data_destruct(data);
        exist = 0;
    }
    else if (strcmp(word, "help") == 0) { // HELP
        printf("[CLI]: DOSTEPNE KOMENDY:\n");
        printf("help\n");
        printf("init size\n");
        printf("count file\n");
        printf("show index\n");
        printf("delete index\n");
        printf("destroy\n");
        printf("exit\n");
    }
    else if (strcmp(word, "exit") == 0) { // EXIT
        if (exist)
            collecting_data_destruct(data);
        running = 0;
    }
    else if (strcmp(word, "init") == 0) { // INIT
        if (exist == 1) {
            printf("[CLI]: JUZ ISTNIEJE STRUKTURA\n");
            return;
        }

        word = strtok(NULL, " ");
        if (word == NULL) {
            printf("[CLI]: PODAJ DRUGI ARGUMENT\n");
            return;
        }
        int size = atoi(word);
        if (size < 1) {
            printf("[CLI]: NIEDOZWOLONY ROZMIAR\n");
            return;
        }
        data = collecting_data_new(size);
        exist = 1;
    }
    else if (strcmp(word, "count") == 0) { // COUNT
        if (exist == 0) {
            printf("[CLI]: STRUKTURA NIE ISTNIEJE\n");
            return;
        }
        word = strtok(NULL, " ");
        if (word == NULL) {
            printf("[CLI]: PODAJ DRUGI ARGUMENT\n");
            return;
        }
        //printf("(%s)\n", word);
        if (data->size >= data->max_size) {
            printf("[CLI]: BRAK MIEJSCA W STRUKTURZE\n");
            return;
        }
        collecting_data_add(data, word);
    }
    else if (strcmp(word, "show") == 0) { // SHOW
        if (exist == 0) {
            printf("[CLI]: STRUKTURA NIE ISTNIEJE\n");
            return;
        }
        word = strtok(NULL, " ");
        if (word == NULL) {
            printf("[CLI]: PODAJ DRUGI ARGUMENT\n");
            return;
        }
        int index = atoi(word);
        if (index < 0 || index >= data->max_size) {
            printf("[CLI]: NIEDOZWOLONY INDEX\n");
            return;
        }
        printf("[CLI]: index %d: %s",index, collecting_data_get(data, index));
    }
    // DELETE
    else if (strcmp(word, "delete") == 0) {
        if (exist == 0) {
            printf("[CLI]: STRUKTURA NIE ISTNIEJE\n");
            return;
        }
        word = strtok(NULL, " ");
        if (word == NULL) {
            printf("[CLI]: PODAJ DRUGI ARGUMENT\n");
            return;
        }

        int index = atoi(word);
        if (index < 0 || index >= data->max_size) {
            printf("[CLI]: NIEDOZWOLONY INDEX\n");
            return;
        }
        collecting_data_free_index(data, index);
    }
    else {
        printf("[CLI]: NIEPOPRAWNA KOMENDA: %s\n", word);
    }
}

int main() {
    load("collecting_data.so");
    char buffor[BUFFOR_SIZE];
    struct timespec t1, t2;
    struct tms start, end;
    char* err;
    printf("[CLI]: W RAZIE PROBLEMOW WPISZ help\n");
    while(running) {
        printf("[CLI]: ");
        err = fgets(buffor, BUFFOR_SIZE, stdin);
        if (err == NULL)
            err = NULL;
        clock_gettime(CLOCK_REALTIME, &t1);
        times(&start);
        split_command(buffor);
        clock_gettime(CLOCK_REALTIME, &t2);
        times(&end);

        printf("CZASY WYKONANIA KOMENDY\n");
        printf("RZECZYWISTY:  %ld ns\n", t2.tv_nsec-t1.tv_nsec);
        printf("URZYTKOWNIKA: %ld clock ticks\n", end.tms_cutime-start.tms_cutime);
        printf("SYSTEMOWY:    %ld clock ticks\n", end.tms_cstime-start.tms_cstime);
    }

    return 0;
}