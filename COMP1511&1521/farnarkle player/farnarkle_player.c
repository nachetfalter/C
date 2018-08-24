//Topic:Farnarkle Player
//Author:Victor Wang
//Date:31/3/17

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "farnarkle.h"

// an automated farnarkle_player
// given all previous guesses and their farnarkles and arkle counts
// make a new guess
//
// inputs:
// turn - which turn this is
// previous_guesses contains the turn - 1 previous_guesses
// farnarkles[i] contains the number of farnarkles for previous_guess[i]
// arkles[i] contains the number of arkles for previous_guess[i]
//
// output:
// guess - the next guess

void farnarkle_player(int turn, int prev[N_TILES], int far, int ark, int ans[N_TILES]){
	int i,n;
	static int pool[MAX_TILE];
	int pool_index;
	static int sequence1[N_TILES];
	static int sequence2[N_TILES];
	int arry1[N_TILES];
	int arry2[N_TILES];
	int dummy_far = 0;
	int dummy_ark = 0;
	static int far_his[2];
	static int ark_his[2];
	static int ban_list[N_TILES*MAX_TILE];
	static int switch1 = 0;
    static int good_list[N_TILES*N_TILES];
    
    //This is the pool which holds every possible numbers.
	for(i = 0; i <= MAX_TILE - 1; ++i){
	    pool[i] = i + 1; 
	}   
    
    //Turn 1 special treatment :D
    if(turn == 1){
	    for(i = 0; i <= N_TILES - 1; ++i){
	        ans[i] = i + 1; 
	    }
    }
    else{
	    //Prepare the dummy 'guess',which is the last guess.
	    for(i = 0; i <= N_TILES - 1; ++i){
		    sequence2[i] = prev[i]; 
		}   

	    //Number Filtering Process
	    while(dummy_far < far || dummy_ark < ark){
	    	//Prepare the dummy hidden sequence.
	    	for(i = 0; i <= N_TILES - 1; ++i){
	    		pool_index = rand() % MAX_TILE;
	    	    sequence1[i] = pool[pool_index];
	    	}
	    	
	    	//Farnarkle Counter,reset is necessary if failed to match hidden sequence to guess.
	    	dummy_far = 0;
		    for(i=0; i<=N_TILES - 1; ++i){
			    if(sequence2[i] == sequence1[i]) dummy_far += 1; 
		    }
	        //Arkle Counter
	        dummy_ark = 0;
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
			          dummy_ark += 1;
			          break;
			        } 
				}
				n = 0;
			}    	
	    }
	    /*for(i = 0; i <= N_TILES - 1; ++i){
        printf("*%d* <%d>",sequence1[i],i);
        }
		printf("\n");*/
	    //Time to give our guess/answer :)
	    for(i = 0; i <= N_TILES - 1; ++i){
		    ans[i] = sequence1[i]; 
		} 
    }
}


    





    /*int i;
    static int farnarkles[MAX_TURNS];
    static int arkles[MAX_TURNS];
    static int dummy[N_TILES];
    static int correct[N_TILES];
    static int number = 1;
    static int amount[MAX_TURNS];
    static int amount_index = 1;
    static int correct_amount = sizeof(correct);
    amount[0] = 1;
    correct[N_TILES] = 0;
    int n = 0;
    
    
    farnarkles[turn-1] = far;
    arkles[turn-1] = ark;
    
    for(i = 0; i < N_TILES; ++i){
	       ans[i] = dummy[i]; 
	}   

    //STAGE 1:
    if(farnarkles[turn-1] == farnarkles[turn-2] && arkles[turn-1] == arkles[turn-2]){
    	if(correct[N_TILES] == 0){
    	   for(i = 0; i < N_TILES; ++i){
	       ans[i] = number; 
	       } 
    	}
        else{

        }  
    }

    //STAGE 2:
    if(farnarkles[turn-1] != farnarkles[turn-2] || arkles[turn-1] != arkles[turn-2]){
    	amount[amount_index] = far + ark;
    	amount[0] = 0;
	    for(i = amount[amount_index-1]; i < far + ark; ++i){
           correct[i] = number; 
        }
        for(i = 0; i < amount[amount_index]; ++i){
           ans[i] = correct[i];
        }  
        for(i > amount[amount_index]; i < N_TILES; ++i){
           ans[i] = number + 1;
        }
        
        amount_index += 1;
	}
	
    number += 1;
}


//far + ark == 0
//far + ark != 0

    /*
    if(farnarkles[turn-1] == farnarkles[turn-2] && arkles[turn-1] == arkles[turn-2]){
    	//If there is no farnarkle but arkles
    	if(farnarkles[turn - 1] == 0){
    	   //right shift the existing right numbers
    	   for(i = 0; i < N_TILES; ++i){
	          ans[i+1] = dummy[i]; 
	        }
            ans[0] = number + 1;
    	}
    	//
    	//If there is no farnarkle but arkles
    	if(arkles[turn - 1] == 0){
    	   //right shift the existing right numbers
    	   for(i = 0; i < N_TILES; ++i){
	          ans[i+1] = dummy[i]; 
	        }
            ans[0] = number + 1;
    	}
		for(i = 0; i < N_TILES; ++i){
	       ans[i] = number + 1; 
	    }   
	}

    //STAGE 2:
	if(farnarkles[turn-1] != farnarkles[turn-2] || arkles[turn-1] != arkles[turn-2]){
		amount += far + ark;
		for(i = amount - 1; i < amount + far + ark - 1; ++i){
	       correct[i] = number; 
	    }
	    for(i = amount - 1; i < amount + far + ark - 1; ++i){
	       ans[i] = correct[i]
	    }  
        for(i = amount + far + ark; i < N_TILES; ++i){
	       ans[i] = number + 1;
	    } 

        
	}
}


 /*if(turn > 1){
    	for((i = turn * N_TILES - (N_TILES - 2));i < turn * N_TILES;++i){
    	prev[i] = ans[n]
    	n += 1;
        }
    }*/