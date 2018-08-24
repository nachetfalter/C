//Date:4/10/2017
//Author: Victor Wang
//Topic:substitution cipher automatic decode
//Description:decode using substitution cipher


/*Unfortunately I didn't have time to debugg this, but I explained my general idea of approach. Another way to do this is by building a bigram language model. 
And use variance to calculate possibilities of each letter follows the other in the alphabet, but it would be even more time consuming.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void bigram(char text1[10001], char text2[10001], int last);

char samp_text[10001];
char sec_text[10001];

int main(int argc, char** argv){
  FILE *sample;
  int counter;
  int input = 0;
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
  
  bigram(samp_text, sec_text, end);
  
  return 0;
}



//This function found the frequency of every bigram and put them in a roll. 
void bigram(char text1[10001], char text2[10001], int last){
   int input1[2];
   int input2[2];
   double repeat1[26][26];
   double repeat2[26][26];
   int number1 = 0;
   int number2 = 0;
   int counter1,counter2,counter3,counter4;
   double frequency1 = 0;
   double frequency2 = 0;
   double freq_array1[26][26];
   double freq_array2[26][26];
   int guess1[26][26];
   int guess2[26][26];
   int check = 0;
   int i = 0;
   int dif[26];
   int che_index = 0;
   int fin_che = 1;
   int end = 0;
   int sec1[1352];
   int sec2[1352];
   int fin_alpha1[26];
   int fin_alpha2[26];

   
   //initialize repeat array. 
   for(counter1 = 0; counter1 < 26; ++counter1){
      for(counter2 = 0; counter2 < 26; ++counter2){
        repeat1[counter1][counter2] = 0;
        repeat2[counter1][counter2] = 0;
      }
    }  
   

   //Input repeat
   for(counter1 = 0; counter1 < 10001; ++counter1){
        input1[0] = text1[counter1];
        input1[1] = text1[counter1 + 1];
        input2[0] = text2[counter1];
        input2[1] = text2[counter1 + 1];

        if((input1[0] > 0 && input1[0] != ' ' && input1[0] != '\n') && ((input1[0] >= 'A' && input1[0] <= 'Z') || (input1[0] >= 'a' && input1[0] <= 'z'))){   
          if (input1[0] >= 'A' && input1[0] <= 'Z'){
              input1[0] += 32;    
          }
        }
        if((input1[1] > 0 && input1[1] != ' ' && input1[1] != '\n') && ((input1[1] >= 'A' && input1[1] <= 'Z') || (input1[1] >= 'a' && input1[1] <= 'z'))){
              number1 += 1;   
              if (input1[1] >= 'A' && input1[1] <= 'Z'){
                  input1[1] += 32;    
              }
             //printf("%c ",input1[1]);
              
        }
        repeat1[input1[0] - 'a'][input1[1] - 'a'] += 1; 
        if((input2[0] > 0 && input2[0] != ' ' && input2[0] != '\n') && ((input2[0] >= 'A' && input2[0] <= 'Z') || (input2[0] >= 'a' && input2[0] <= 'z'))){   
          if (input2[0] >= 'A' && input2[0] <= 'Z'){
              input2[0] += 32;    
          }
          if((input2[1] > 0 && input2[1] != ' ' && input2[1] != '\n') && ((input2[1] >= 'A' && input2[1] <= 'Z') || (input2[1] >= 'a' && input2[1] <= 'z'))){
              number2 += 1;   
              if (input2[1] >= 'A' && input2[1] <= 'Z'){
                  input2[1] += 32;    
              }
              
          }
          repeat2[input2[0] - 'a'][input2[1] - 'a'] += 1; 
        }
    }    
 
   //Calculate the frequency of bigrams.
   for(counter1 = 'a'; counter1 <= 'z'; ++counter1){
        for(counter2 = 'a'; counter2 <= 'z'; ++counter2){
          frequency1 = repeat1[counter1 - 'a'][counter2 - 'a'] / number1;
          frequency2 = repeat2[counter1 - 'a'][counter2 - 'a'] / number2;
          freq_array1[counter1 - 'a'][counter2 - 'a'] = frequency1;
          freq_array2[counter1 - 'a'][counter2 - 'a'] = frequency2;
        }     
    }

   //Shuffle the bigrams into order according to frequency
   for(counter1 = 0; counter1 < 26; ++counter1){
      for(counter2 = 0; counter2 < 26; ++counter2){
        for(counter3 = 0; counter3 < 26; ++counter3){
          for(counter4 = 0; counter4 < 26; ++counter4){
             if(counter1 != counter3 && counter2 != counter4){
                if(freq_array1[counter1][counter2] - freq_array1[counter3][counter4]>= 0){
                   check += 1;
                }
              }   
          }
        }
      guess1[(676 - check) / 26][(676 - check) % 26] = 100*(counter1 + 'a') + (counter2 + 'a');
      i = 0;
      check = 0;
      }
       //Prevent bigram with the same frequency to settle in the same coordinate.
      /*if(guess1[(676 - check) / 26][(676 - check) % 26] != 0){
         while(guess1[(676 - check) / 26][(676 - check) % 26 + i] != 0){
            i += 1;
         }
         printf("miao\n");
         guess1[(676 - check) / 26][(676 - check) % 26] = 100*(counter1 + 'a') + (counter2 + 'a');
      }
      else{
         guess1[(676 - check) / 26][(676 - check) % 26] = 100*(counter1 + 'a') + (counter2 + 'a');
      }*/
      
    }
    

   for(counter1 = 0; counter1 < 26; ++counter1){
      for(counter2 = 0; counter2 < 26; ++counter2){
        for(counter3 = 0; counter3 < 26; ++counter3){
          for(counter4 = 0; counter4 < 26; ++counter4){
             if(counter1 != counter3 && counter2 != counter4){
                if(freq_array2[counter1][counter2] - freq_array2[counter3][counter4]>= 0){
                   check += 1;
                }
              }   
          }
        }
      guess2[(676 - check) / 26][(676 - check) % 26] = 100*(counter1 + 'a') + (counter2 + 'a');
      i = 0;
      check = 0;
      }
      
      /*if(guess2[(676 - check) / 26][(676 - check) % 26] != 0){
         while(guess2[(676 - check) / 26][(676 - check) % 26 + i] != 0){
            i += 1;
         }
         guess2[(676 - check) / 26][(676 - check) % 26] = 100*(counter1 + 'a') + (counter2 + 'a');
      }
      else{
         guess2[(676 - check) / 26][(676 - check) % 26] = 100*(counter1 + 'a') + (counter2 + 'a');
      }*/

    }
    
    //Decode the bigram and put all into coresponding array
    counter3 = 0;
    for(counter1 = 0; counter1 < 26; ++counter1){
      for(counter2 = 0; counter2 < 26; ++counter2){
          if(counter1 % 2 == 0) sec1[counter3] = guess1[counter1][counter2] / 100;
          if(counter1 % 2 == 0) sec2[counter3] = guess2[counter1][counter2] / 100;
          if(counter1 % 2 != 0) sec1[counter3] = guess1[counter1][counter2] % 100;
          if(counter1 % 2 != 0) sec2[counter3] = guess2[counter1][counter2] % 100;
          counter3 += 1;
      }
    }
    //Eliminate repeat elements
    counter3 = 0;
    for(counter1 = 0; counter1 < 1352; ++counter1){
       for(counter2 = 0; counter2 < 1352; ++counter2){
          if(sec1[counter1] == sec1[counter2]) sec1[counter2] = -1;
          if(sec2[counter1] == sec2[counter2]) sec2[counter2] = -1;
       }
    }      
    //Arrange the alphabet of both samples
    for(counter1 = 0; counter1 < 26; ++counter1){
       for(counter2 = 0; counter2 < 1352; ++counter2){
          if(sec1[counter2] > 0) fin_alpha1[counter1] = sec1[counter2];
          if(sec2[counter2] > 0) fin_alpha2[counter1] = sec2[counter2];
       }
    }
  
  /*for (counter1 = 0; counter1 < 26; ++counter1)
  {
    printf("<%d> ",repeat2[1][counter1]);
  }*/
  //Decode text2 according to the alphabet from text1
  counter1 = 0;
  while(counter1 < last){
    if(text2[counter1] >= 'A' && text2[counter1] <= 'Z'){
       for(counter2 = 0; counter2 < 26; ++counter2){
          if(text2[counter1] - 'A' == text1[counter2]){
             putchar(text1[counter2] - 'a' + 'A');
             break;
          } 
       }  
    }
    else if(text2[counter1] >= 'a' && text2[counter1] <= 'z'){
       for(counter2 = 0; counter2 < 26; ++counter2){
          if(text2[counter1] == text1[counter2]){
             putchar(text1[counter2]);
             break;
          } 
       }    
    }
    else {
        putchar(sec_text[counter1]);
    }
    counter1 += 1;
  }  
}

