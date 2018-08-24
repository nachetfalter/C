//Topic:Farnarkle
//Author:Victor Wang
//Date:29/3/17

#include <stdio.h>
#include "farnarkle.h"

// read N_TILES tiles into array tiles
// return 1 if successful, 0 otherwise
int read_tiles(int tiles[N_TILES]){
	int number,i;
	number = 0;
	for(i = 0; i <= N_TILES - 1; ++i){
		scanf("%d",&number);
		if(number-MAX_TILE > 0) return 0;
		tiles[i] = number;	
	}
    return 1;
}

// print tiles on a single line
void print_tiles(int tiles[N_TILES]){
	int i;
	for(i = 0; i <= N_TILES - 1; ++i){
		printf("%d ",tiles[i]);
	}
	printf("\n");
}
