//Topic:Farnarkle
//Author:Victor Wang
//Date:29/3/17
//Topic:Farnarkle
//Author:Victor Wang
//Date:29/3/17

#include <stdio.h>
#include "farnarkle.h"

// return number of arkles
int count_arkles(int sequence1[N_TILES], int sequence2[N_TILES]){
	int i,n,counter;
	counter  = 0;
	int arry1[N_TILES],arry2[N_TILES];
	for(i=0; i <= N_TILES - 1; ++i){
		arry1[i] = sequence1[i];
		arry2[i] = sequence2[i];
	}	 
	for(i=0; i <= N_TILES - 1; ++i){
		if(arry1[i] == arry2[i]){
			arry1[i] = -1;
			arry2[i] = -1;
		}
	}
    for(i=0; i <= N_TILES - 1; ++i){
		for(n = 0; n <= N_TILES - 1; ++n){
	       if(arry1[i] > 0 && arry2[n] > 0 && arry2[i] != arry1[i] && i != n && arry1[i] == arry2[n]){
	          arry1[i] = -1;
	          arry2[n] = -1;
	          counter += 1;
	          break;
	        } 
		}
		n = 0;
	}
    return counter;
}