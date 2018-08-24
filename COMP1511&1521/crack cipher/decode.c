//Date:4/09/2017
//Author: Victor Wang
//Topic:substitution decoder
//Description:decode using substitution cipher


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int decipher(int character); //Declare prototype

int main(int argc, char** argv){
   int input = 0;
   char output;
   int index;
   int counter;
   
   while(input != EOF){
   	    input = getchar();	 

    	  //Substitution
        if((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')){
            for(counter = 0; counter <= 25; ++counter){
               if(decipher(input) - argv[1][counter] == 0){
                  break;
               } 
            }
            if(input > 0 && input != '\n' && input >= 'A' && input <='Z'){
            putchar(counter + 'A');
            }
            else if(input >0){
            putchar(counter + 'a');
            }
        }
        else if (input > 0){
            putchar(input);
        }  	    			
    }
return 0;  
}

//change case
int decipher(int character){
	if(character >= 'A' && character <= 'Z'){
		character = character - 'A' + 'a';
	}
	return character;
}
