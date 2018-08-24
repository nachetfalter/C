//Topic: print_world.c
//Author: Victor Wang
//Date: 11/May/2017
//Description:print_world should print a description of all locations in the bot's world

#include <stdio.h>
#include "trader_bot.h"

void print_world(struct bot *b){
    char* start = b -> location -> name; //Record the name of the first place, so if the list return to this node, stop the loop.
	int end_switch = 0;
	
    while(end_switch != 1){
		printf("%s: ",b -> location -> name);
		if(b -> location -> type == 0){
			printf("start");
	    }
	    else if(b -> location -> type == 1){
	    	printf("will sell %d units of %s for $%d",b -> location -> quantity,b -> location -> commodity -> name,b -> location -> price);
	    }
	    else if(b -> location -> type == 2){
	    	printf("will buy %d units of %s for $%d",b -> location -> quantity,b -> location -> commodity -> name,b -> location -> price);
	    }
	    else if(b -> location -> type == 3){
	    	printf("Petrol station %d units of available fuel for $%d",b -> location -> quantity,b -> location -> price);
	    }	    
	    else if(b -> location -> type == 4){
	    	printf("dump");
	    }
	    else if(b -> location -> type == 5){
	    	printf("other");
	    }
	    b -> location = b -> location -> next;
	    if(b -> location -> name == start) end_switch += 1; 	
		printf("\n");    	    
   }
}
