//Date:4/09/2017
//Author: Victor Wang
//Topic:caesar cipher automatic decode
//Description:decode using caesar cipher

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char unigram(char text[10001], int index);

char samp_text[10001];
char sec_text[10001];

int main(int argc, char** argv){
  FILE *sample;
  int counter;
  int input = 0;
  char dif = 'a';
  int che_index = 0;
  int fin_che = 1;
  int end = 0;

  //read the sample file into a string
  sample = fopen(argv[1],"r");
  counter = 0;
  while(!feof(sample)){
    samp_text[counter] = fgetc(sample);
    if(samp_text[counter] == EOF){
      samp_text[counter] = 0;
    }
    counter += 1;

    if(counter == 10001){
      break;
    }
  }

  fclose(sample);
 
  counter = 0;
  while(input != EOF && counter < 10001){
    input = getchar();
    if(input > 0){
      sec_text[counter] = input;
    }
    counter += 1;
  }
  sec_text[counter + 1] = -1; //Set the end of text
  end = counter + 1 - 2;

  //This is the most important module which deduct any unfit results.(if there are characters other than letters)
  while(fin_che == 1){
     fin_che = 0;
     dif = (unigram(sec_text,che_index)) - (unigram(samp_text,0));
     counter = 0;
     while(sec_text[counter] != -1){
       if(sec_text[counter] >= 'A' && sec_text[counter] <= 'Z'){
         if((sec_text[counter] - dif) < 'A'){
           if(('Z' - (dif -  (sec_text[counter] - 'A')) + 1) < 'A' || (('Z' - (dif -  (sec_text[counter] - 'A')) + 1) > 'Z')){
             fin_che = 1;
           }
         }
         else{
           if((sec_text[counter] - dif) < 'A' || (sec_text[counter] - dif) > 'Z'){
             fin_che = 1;
           }
         }
       }
       else if(sec_text[counter] > 'a' && sec_text[counter] < 'z'){
         if((sec_text[counter] - dif) < 'a'){
           if(('z' - (dif -  (sec_text[counter] - 'a')) + 1) < 'a' || (('z' - (dif -  (sec_text[counter] - 'a')) + 1) > 'z')){
             fin_che = 1;
           }
         }
         else{
           if((sec_text[counter] - dif) < 'a' || (sec_text[counter] - dif) > 'z'){
             fin_che = 1;
           }
         }
       }
       counter += 1;
     }
     che_index += 1;
   }
   
  //The actual printing
  counter = 0;
  while(counter < end){
    if(sec_text[counter] >= 'A' && sec_text[counter] <= 'Z'){
      if((sec_text[counter] - dif) < 'A'){
        putchar('Z' - (dif -  (sec_text[counter] - 'A')) + 1);
      }
      else{
        putchar(sec_text[counter] - dif);
      }
    }
    else if(sec_text[counter] >= 'a' && sec_text[counter] <= 'z'){
      if((sec_text[counter] - dif) < 'a'){
        putchar('z' - (dif -  (sec_text[counter] - 'a')) + 1);
      }
      else{
        putchar(sec_text[counter] - dif);
      }
    }
    else {
        putchar(sec_text[counter]);
    }
    counter += 1;
  }
  return 0;
}

//This function found the frequency of every letter and put them in a roll. If the result is unfit, use the next most frequent letter.
char unigram(char text[10001], int index){
   int input = 0;
   double repeat[26];
   int number = 0;
   int counter1,counter2;
   int best_index = 'a';
   double frequency = 0;
   double freq_array[26];
   int guess[26];
   int check = 0;
   int i = 0;

   for(counter1 = 0; counter1 < 26; ++counter1){
        repeat[counter1] = 0;
        guess[counter1] = 0;
   }
   input = text[1];
   
   for(counter1 = 0; counter1 < 10001; ++counter1){
        input = text[counter1];
        if((input > 0 && input != ' ' && input != '\n') && ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z'))){
          number += 1;   
          if (input >= 'A' && input <= 'Z'){
              input += 32;    
          }
          repeat[input - 'a'] += 1;
        } 
   }
   for(counter1 = 'a'; counter1 <= 'z'; ++counter1){
        frequency = repeat[counter1 - 'a'] / number;
        freq_array[counter1 - 'a'] = frequency;
   }

   for(counter1 = 0; counter1 < 26; ++counter1){
      for(counter2 = 0; counter2 < 26; ++counter2){
         if(counter1 != counter2){
            if(freq_array[counter1] - freq_array[counter2]>= 0){
               check += 1;
            }
         }   
      }
      if(guess[25 - check] != 0){
         while(guess[25 - check + i] != 0){
            i += 1;
         }
         guess[25 - check + i] = counter1 + 'a';
      }
      else{
         guess[25 - check] = counter1 + 'a';
      }
      i = 0;
      check = 0;
   }
   return guess[index];  
}
