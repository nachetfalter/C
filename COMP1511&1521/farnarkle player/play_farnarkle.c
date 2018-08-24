//Topic:Farnarkle
//Author:Victor Wang
//Date:29/3/17

#include <stdio.h>
#include "farnarkle.h"

int main(void) {
	int win,turn;
    int hidden_sequence[N_TILES];
    int guess[N_TILES];
    create_random_tiles(hidden_sequence);
    win = 0;
    for(turn = 1; win < 1; ++turn){
    //printf("<%d %d %d %d>\n",hidden_sequence[0],hidden_sequence[1],hidden_sequence[2],hidden_sequence[3]);
    	printf("Enter guess for turn %d: ",turn);
    	if(read_tiles(guess) != 1){
            printf("Could not read guess\n");
            return 1;
        }
    //printf("<%d %d %d %d>\n",guess[0],guess[1],guess[2],guess[3]);
        printf("%d farnarkles ", count_farnarkles(hidden_sequence, guess));
        printf("%d arkles\n", count_arkles(hidden_sequence, guess));
        if(count_farnarkles(hidden_sequence, guess) == N_TILES){
        	printf("You win\n");
        	win = 1;
        }
    }
    return 0;
}