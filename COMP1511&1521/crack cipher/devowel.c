//Date:4/09/2017
//Author: Victor Wang
//Topic:devowel
//Description: Write a C program devowel.c which reads characters from its input and writes the same characters to its output, except it does not write lower case vowels ('a', 'e','i', 'o', 'u'). 

#include <stdio.h>

int main(void){
   int input = 0;
   
   //Filter the vowels.
   while(input != EOF){
   	    input = getchar();
   	    if(input > 0 && input != 'a' && input != 'e' && input != 'i' && input != 'o' && input != 'u'){
		    putchar(input);
		   
		}		
   }
   
return 0;  
}
