//Topic:Farnarkle
//Author:Victor Wang
//Date:29/3/17

#include <stdio.h>
#include "farnarkle.h"

// return number of farnarkles
int count_farnarkles(int sequence1[N_TILES], int sequence2[N_TILES]){
	int i,counter;
	counter = 0;
	for(i=0; i<=N_TILES - 1; ++i){
		if(sequence2[i] == sequence1[i]) counter += 1; 
	}
    return counter; 
}
