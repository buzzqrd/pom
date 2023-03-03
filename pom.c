#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#include "argp.h"

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define cursor_hide() printf("\e[?25l")
#define cursor_show() printf("\e[?25h")

#define POM_LEN 25
#define POM_CNT 4
#define POM_LOOPS 4
#define BREAK_SMALL 5
#define BREAK_LONG 30


void sig_hand(int errno){
	cursor_show();
	clear();
	exit(0);
}


int main(int argc, const char *argv[]){
	signal(SIGINT, sig_hand);

	int pom_len = POM_LEN;
	int pom_cnt = POM_CNT;
	int pom_loops = POM_LOOPS;
	int break_small = BREAK_SMALL;
	int break_long = BREAK_LONG;

	int pos;
	struct argpc pc;
	argpc_init(argc, &pc);

	if((pos = has_argp(argc, argv, &pc, 4, 2, "c", "-custom"))){
		pom_len = atoi(argv[++pos]);
		pom_cnt = atoi(argv[++pos]);
		break_small = atoi(argv[++pos]);
		break_long = atoi(argv[++pos]);
		pom_loops = atoi(argv[++pos]);
	}
	if((pos = has_argp(argc, argv, &pc, 0, 2, "h", "-help"))){
		fprintf(stderr, "POM - simple cli pomodoro.\nUsage:\tpom [OPTIONS] [ARGS]\n-c [POMTIME] [POMCOUNT] [BREAKSIZE] [LONGBREAKSIZE] [REPEATCOUNT]\n");
		return(0);
	}

	if(pos = inval_argp(argc, argv, &pc)){
		fprintf(stderr, "Error: invalid argument at: %d.\n", pos);
		return(-1);
	}
	
	if(!unuse_argp(argc, argv, &pc)){
		; /* free args not required */
	}


	clock_t begin;
	double clock_time = 0;
	int clock_mins = 0;
	int secs = 0;
	int mins = 0;

	int pom_cur = 0;
	int total_loops = 0;
	int pom_time = 0;
	int state = 0;


	int total_mins = ((((pom_len+break_small)*pom_cnt)*pom_loops)-break_small) + (break_long * pom_loops);
	printf("Total pomodoro time: %d hrs, %d mins. Proceed? [y/n]: ", total_mins/60, total_mins%60);
	if(fgetc(stdin) == 'n'){
		return(-1);
	}

	clear();
	cursor_hide();
	begin = clock();
	for(;;){
		clock_time = (double)(clock() - begin) / CLOCKS_PER_SEC;
		clock_mins = (int)(clock_time/60);
		gotoxy(0,0);
	
		mins = (int)(clock_time/60);
		secs = ((int)(clock_time)%60);
	
		switch(state){
			case 0: /* in pomo */
				printf("Working!\nTime: [%02d:%02d]", mins, secs);
				if(clock_mins < pom_len){break;}
				clear();
				begin = clock();
				pom_cur++;
				if(pom_cur == pom_cnt){
					state = 2; /*big break*/
				}
				else{
					state = 1; /*small break*/
				}
				break;
			case 1: /* small break */
				printf("Small break...\nTime: [%02d:%02d]", mins, secs);
				if(clock_mins < break_small){break;}
				clear();
				begin = clock();
				state = 0;	
				break;
			case 2: /* large break */
				printf("Long break . . . \nTime: [%02d:%02d]", mins, secs);
				if(clock_mins < break_long){break;}
				clear();
				begin = clock();
				total_loops++;
				pom_cur = 0;
				if(total_loops == pom_loops){
					state = 3;
				}
				break;
			case 3:
				clear();
				break;
			default:
				break;
		}

		if(state == 3){
			gotoxy(0,0);
			printf("Pomodoro Complete!\n");
			cursor_show();
			break;
		}

				
	}

	return(0);
}


