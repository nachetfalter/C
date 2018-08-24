//Topic: nearest_fuel.c
//Author: Victor Wang
//Date: 11/May/2017
//Description:nearest_fuel should return the distance to the nearest Petrol Station. 

#include <stdio.h>
#include "trader_bot.h"

int nearest_fuel(struct bot *b){

	int distance_1 = 0; //The distance to the nearest gas station forwards
	int distance_2 = 0; //The distance to the nearest gas station backwards

	struct location *temp_loc; //This is the dummy struct, I use this so the bot doesn't actually move

	if(b -> location -> type == 3){ //If already on station, return 0;
		return 0;
	}
    else{

        temp_loc = b -> location;
        while(temp_loc -> type != 3){
			temp_loc = temp_loc -> next;
			distance_1 += 1;
		}

		temp_loc = b -> location;
		while(temp_loc -> type != 3){
			temp_loc = temp_loc -> previous;
			distance_2 -= 1;
		}
        
        //Compare the two distances, choose the smaller one
		if(-(distance_2) >= distance_1){
			return distance_1;
		}
		else{
			return distance_2;
		}
    }

}
