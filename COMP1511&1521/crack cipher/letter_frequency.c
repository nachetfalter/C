//Date:4/09/2017
//Author: Victor Wang
//Topic:letter frequency
//Description:check the appearance of certain letters in a sequence

#include <stdio.h>

int main(void){
   int input = 0;
   double repeat[26];
   int number1 = 0,number2 = 0;
   int counter1;
   double frequency = 0;

   for(counter1 = 0; counter1 < 26; ++counter1){
   	    repeat[counter1] = 0;
   }

   while(input != EOF){
   	    input = getchar();
   	    if((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')){
   	    	number1 += 1;   
   	    	if (input >= 'A' && input <= 'Z'){
   	    	    input += 32;    
   	    	}
            repeat[input - 'a'] += 1;
        }
   }
   
   //check frequency and occurance.
   for(counter1 = 'a'; counter1 <= 'z'; ++counter1){
   	    frequency = repeat[counter1 - 'a'] / (number1);
        number2 = repeat[counter1 - 'a'];
   	    printf("'%c' %.6lf %d\n",counter1,frequency,number2);
   }
   return 0;  
}
