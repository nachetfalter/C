//Date:4/09/2017
//Author: Victor Wang
//Topic:caesar cipher
//Description:encoding using caesar cipher

/*The manually crack answer is:
And we'll never be royals It don't run in our blood That kind of lux just ain't for us We crave a different kind of buzz*/

#include <stdio.h>
#include <stdlib.h>
void cipher(int character, int cipher); //Declare prototype

int main(int argc, char** argv){
   int input = 0;
   int cipher_key = atoi(argv[1]);
   
   
   while(input != EOF){
   	    input = getchar();
   	    if(input > 0){
   	      cipher(input, cipher_key);
   	    }	
    }
   
return 0;  
}

//Cipher
void cipher(int character, int cipher){
	if(cipher < 0){
		cipher = cipher %  26 + 26;
	}

	if(character >= 'A' && character <= 'Z'){
		character -= 'A';
		character = character + cipher;
		
		character %=  26;
		character += 'A';
		
	}
	else if(character >= 'a' && character <= 'z'){
		character -= 'a';
		character = character + cipher;
		character %=  26;
		character += 'a';
    }
	else{
		character = character;
	}
	putchar(character);
}

