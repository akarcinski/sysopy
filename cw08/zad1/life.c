#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

const int width = 30;
const int height = 30;

typedef struct data {
  int row;
  int col;
  char* src;
  char* dst;
}data;

void *calculate(void* args) {
    struct data* dat = (data*)args;
    while(1){
        dat->dst[dat->row * width + dat->col] = is_alive(dat->row, dat->col, dat->src);
        pause();
        char* tmp = dat->src;
        dat->src = dat->dst;
        dat->dst = tmp;
    }
}

void handler(int signo, siginfo_t* info, void* context) {}



int main()
{
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;
        
	init_grid(foreground);
	
	int iter = 0;
        pthread_t thread[width][height];
    
        struct sigaction action;
        sigemptyset(&action.sa_mask);
        action.sa_sigaction = handler;
        sigaction(SIGUSR1, &action, NULL);
        
        for (int i=0; i<width; i++) {
            for (int j=0; j<height; j++) {
                struct data* args = malloc(sizeof(data));
                args->row = j;
                args->col = i;
                args->src = foreground;
                args-> dst = background;
                pthread_create(&thread[j][i], NULL, calculate, (void*)args);
            }
        }
        
	while (true)
	{
		draw_grid(foreground);
		usleep(500 * 1000);
                
                  // Step simulation
                //update_grid(foreground, background);
                for (int i=0; i<width; i++) {
                    for (int j=0; j<height; j++) {
                        pthread_kill(thread[j][i], SIGUSR1);
                    }
                }
                //update_grid_thread(foreground, background, iter);
                iter++;
                
                tmp = foreground;
                foreground = background;
                background = tmp;
	}
    
	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
