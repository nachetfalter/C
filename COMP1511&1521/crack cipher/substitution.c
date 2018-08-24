//Date:4/09/2017
//Author: Victor Wang
//Topic:substitution cipher
//Description:encoding using substitution cipher

/*The manual crack answer:
	When the routine bites hard And ambitions are low And the resentment rides high But emotions wont grow And were changing our ways, 
	Taking different roads Then love, love will tear us apart again*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int cipher(int character); //Declare prototype

int main(int argc, char** argv){
   int input = 0;
   char output;
   int index;

   input = getchar();
   while(input != EOF){
    	index = cipher(input);
    	
    	//Substitution
        if((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')){
           output = argv[1][index - 1];
        }
        else{
           output = input;
        }
        
        if(input != '\n' && input >= 'A' && input <= 'Z'){
            putchar(output - 'a' + 'A');
        }
        else{
            putchar(output);
        }
        
        input = getchar();   			
    }
    return 0;  
}

//Change input
int cipher(int character){
	if(character >= 'A' && character <= 'Z'){
		character -= ('A'- 1);
	}
	else if(character >= 'a' && character <= 'z'){
		character -= ('a' - 1);
    }
	return character;
}
