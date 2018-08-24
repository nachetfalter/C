#include <stdio.h>

int main(void){
	int size,column,row;
  int counter1,counter2,counter3,counter4,counter5,counter6,counter7,counter8,counter9,counter10;
  int total_elements,first_number,first_number2,first_number3,number,sum;
  double recursion_number;
	printf("Enter size: ");
	scanf("%d",&size);
	total_elements = size;
	for(counter1 = 0;counter1 <= size;++counter1){
       (counter1 % 2 != 0) && (total_elements += 2*(size - counter1));
	}
  first_number = total_elements % 10 - 1;
  first_number2 = first_number;
  first_number3 = first_number;
  sum = 0;
  column = 1;
  row = 1;
  counter5 = 0;
  counter6 = 0;
  counter10 = 0;
  while(row <= size && row * column <= size*size){
     for(counter2 = 1;counter2 < (row + 1) / 2; ++counter2){
                    sum += size + 2*(size - 1) + size - 3 + 1 - (counter2 - 1)*16;
                  }
     if(row % 2 != 0){
     	   if(row < (size + 1) / 2){
            if((column % 2 != 0 && column < row)|| column == row - 1){
               if(row > 1 && column < row && column > row - 3){
                  number = 9 - (sum - (2 - (column - (row - 3)) + 1) - first_number - 1) % 10;            
        	        printf("%d",number);
                }
               if((column % 2 !=0 && column >= 1, column < row - 3)){
                  counter4 = (row - 1)/2 - 1;
                  for((counter3 = (row - column)/2 - 1);counter3 >= 1;--counter3){
                    sum -= (size + 2*(size - 1) + size - 3 + 1) - 16*counter4;
                    counter4 -=1;
                  }
                  number = 9 - (sum - (row - column) - first_number - 1) % 10;
                  printf("%d",number);
                }
            }  
            else if((column >= row) && (column < size - row + 1)){
                  row == 1 && (number = first_number2);
                  row == 1 && (first_number2 -= 1);
                  number < 0 && (number = 9) && (first_number2 = 8);
                  counter5 = column - row;
                  row != 1 && (number = 9 - (sum + counter5 - first_number - 1) % 10);
                  column != size && printf("%d",number);
            }
            else if((column % 2 != 0) && (row >= 3) && (column >= size - (row - 1)) && (column < size)){                
                  column == size - 1 && (counter6 == 0);
                  counter6 == 0 && (number -= 1);
                  number < 0 && number == 9; 
                  counter6 == 0 && printf("%d",number);
                  sum = 0;
                  if(counter6 != 0){
                     for(counter7 = 1;counter7 < (size - column)/2 + 1;++counter7){
                       sum += size + 2*(size - 1) + size - 3 + 1 - (counter7 - 1)*16;
                     }
                     number = 10 - (sum + (size - (size - column)*2) + (row - (size - column + 1)) + 10 - first_number - 1) % 10;
                     number == 10 && (number = 0);
           	         row > 3 && column != size && printf("%d",number);
                  }
                  counter6 += 1;
            } 
            else{
           	 column != size && printf("-");
            }
          }
         if(row == (size + 1) / 2){
             if((column % 2 != 0) && (column >= 1) && (column <= row - 3)){
      	         if(row > 1 && column < row && column > row - 3){
                    number = 9 - (sum - (2 - (column - (row - 3)) + 1) - first_number - 1) % 10;            
                    printf("%d",number);
                  }
                 if((column % 2 !=0 && column >= 1, column < row - 3)){
                    counter4 = (row - 1)/2 - 1;
                    for((counter3 = (row - column)/2 - 1);counter3 >= 1;--counter3){
                      sum -= (size + 2*(size - 1) + size - 3 + 1) - 16*counter4;
                      counter4 -=1;
                    }
                    number = 9 - (sum - (row - column) - first_number - 1) % 10;
                    printf("%d",number);
                  }
    	       }
    	       else if((column >= row - 2) && (column < size - row + 2)){
           	      row == 1 && (number = first_number2);
                  row == 1 && (first_number2 -= 1);
                  number < 0 && (number = 9) && (first_number2 = 8);
                  counter5 = column - row;
                  row != 1 && (number = 9 - (sum + counter5 - first_number - 1) % 10);
                  column != size && printf("%d",number);
             }
             else if((column % 2 != 0) && (column >= size - (row - 2)) && (column < size)){
                  sum = 0;
                 	for(counter8 = 1;counter8 < (size - column)/2 + 1;++counter8){
                           sum += size + 2*(size - 1) + size - 3 + 1 - (counter8 - 1)*16;
                  }
                  number = 10 - (sum + (size - (size - column)*2) + (row - (size - column + 1)) + 10 - first_number - 1) % 10;
                  number >= 10 && (number %= 10);
                  row > 3 && column != size && printf("%d",number);
             }
             else{
    	  	     column != size && printf("-");
    	       }
          } 
         if(row > (size + 1) / 2){
         	  if((column % 2 != 0) && (column >= 1) && (column <= size - row)){ 
         	  	   if(row > 1 && column < row && column > row - 3){
                    number = 9 - (sum - (2 - (column - (row - 3)) + 1) - first_number - 1) % 10;            
                    printf("%d",number);
                  }
                 if((column % 2 !=0 && column >= 1, column < row - 3)){
                    counter4 = (row - 1)/2 - 1;
                    for((counter3 = (row - column)/2 - 1);counter3 >= 1;--counter3){
                      sum -= (size + 2*(size - 1) + size - 3 + 1) - 16*counter4;
                      counter4 -=1;
                    }
                    number = 9 - (sum - (row - column) - first_number - 1) % 10;
                    printf("%d",number);
                  }
         	  }
         	  else if((column > size - row) && (column <= size - (size - row))){ 
                  sum = 0;
                  for(counter10 = 1;counter10 <= (size - row)/ 2 + 1;++counter10){
                    sum += size + 2*(size - 1) + size - 3 + 1 - (counter10 - 1)*16;
                  }
                  (number = 9 - (sum - (size - (size - row)*2 - 2) - first_number - 1 - (column - (size - row))) % 10);
                  column != size && printf("%d",number);
         	  }
            else if((column % 2 != 0) && (column >= size - (size - row) + 1) && (column <= size - 1)){ 
                  sum = 0;
                  for(counter9 = 1;counter9 < (size - column)/2 + 1;++counter9){
                      sum += size + 2*(size - 1) + size - 3 + 1 - (counter9 - 1)*16;
                  }
                  number = 10 - (sum + (size - (size - column)*2) + (row - (size - column + 1)) + 10 - first_number - 1) % 10;
                  number == 10 && (number = 0);
                  row > 3 && column != size && printf("%d",number);
            }
         	  else{
           	column != size && printf("-");
            }
         	}
      }
     if(row % 2 == 0){
      	    if(row < (size + 1) / 2){
      	          if((column % 2 != 0) && (column >= 1) && (column <= row - 3)){
                      sum = 0;
                      for(counter3 = 0;counter3 < (column + 1)/2; ++counter3){
                          sum += (size + 2*(size - 1) + (size - 3)+ 1) - 16*counter3;
                      }
                      number = 10 - (sum - (row - column - 1) + 10 - first_number - 1) % 10;
                      number > 10 && (number %= 10);
                      number < 0 && (number = 9);
                      printf("%d",number);                      
      	          }
      	          else if((column % 2 != 0) && (column >= size - (row - 1)) && (column < size)){
    	                sum = 0;
                      for(counter9 = 1;counter9 < (size - column)/2 + 1;++counter9){
                          sum += size + 2*(size - 1) + size - 3 + 1 - (counter9 - 1)*16;
                      }
                      number = 10 - (sum + (size - (size - column)*2) + (row - (size - column + 1)) + 10 - first_number - 1) % 10;
                      number == 10 && (number = 0);
                      number > 10 && (number %= 10);
                      number < 0 && (number = 9);
                      column != size && printf("%d",number);
      	          }
      	      else{
      	  	     column != size && printf("-");
      	      }
            }
            if(row == (size + 1) / 2){
               if((column % 2 != 0) && (column >= 1) && (column <= row - 3)){
      	            sum = 0;
                    for(counter3 = 0;counter3 < (column + 1)/2; ++counter3){
                        sum += (size + 2*(size - 1) + (size - 3)+ 1) - 16*counter3;
                    }
                    number = 10 - (sum - (row - column - 1) + 10 - first_number - 1) % 10;
                    number > 10 && (number %= 10);
                    number < 0 && (number = 9);
                    printf("%d",number);  
      	       }
               else if((column % 2 != 0) && (column >= size - (row - 2)) && (column < size)){
             	      sum = 0;
                    for(counter9 = 1;counter9 < (size - column)/2 + 1;++counter9){
                        sum += size + 2*(size - 1) + size - 3 + 1 - (counter9 - 1)*16;
                    }
                    number = 10 - (sum + (size - (size - column)*2) + (row - (size - column + 1)) + 10 - first_number - 1) % 10;
                    number == 10 && (number = 0);
                    number > 10 && (number %= 10);
                    number < 0 && (number = 9);
                    column != size && printf("%d",number);
               }
               else{
      	  	     column != size && printf("-");
      	       }
            } 
            if(row > (size + 1) / 2){
               if((column % 2 != 0) && (column >= 1) && (column <= size - row)){
                    sum = 0;
                    for(counter3 = 0;counter3 < (column + 1)/2; ++counter3){
                        sum += (size + 2*(size - 1) + (size - 3)+ 1) - 16*counter3;
                    }
                    number = 10 - (sum - (row - column - 1) + 10 - first_number - 1) % 10;
                    number > 10 && (number %= 10);
                    number < 0 && (number = 9);
                    printf("%d",number);  
               }
               else if((column % 2 != 0) && (column >= size - (size - row)) && (column <= size - 1)){
                    sum = 0;
                    for(counter9 = 1;counter9 < (size - column)/2 + 1;++counter9){
                        sum += size + 2*(size - 1) + size - 3 + 1 - (counter9 - 1)*16;
                    }
                    number = 10 - (sum + (size - (size - column)*2) + (row - (size - column + 1)) + 10 - first_number - 1) % 10;
                    number == 10 && (number = 0);
                    number > 10 && (number %= 10);
                    number < 0 && (number = 9);
                    column != size && printf("%d",number);
               }
               else{
      	  	     column != size && printf("-");
      	       }
      	    }
      }
     if(row * column <= size * column && column == size){
      (number = 10 - (size + row - 1 + 10 - first_number - 1) % 10);
      number == 10 && (number = 0);
     	printf("%d\n",number);
     	column = 0;
     	row += 1;
      number = 0;
     }
     column += 1;
     sum = 0;
     (column  == size - 1) && (counter6 = 0);
  }
  return 0;
}





/*printf("%d",column);
       if(row % 2 != 0){
          if((column % 2 !=0 && column <= row)|| column == row - 1){
          	printf("%d",number);
          } 
       }
       else 
if((row == 1) && (column == 1)){
               printf("%d",first_number);
            }
if(recursion_number == 1){
                  (row == size) && (number = 10 - column % 10);
                  column == size && row == size && (number = 10 - (size + size - 1) % 10);
                  row == size && row < size && (number = 10 - (size + size - 1 + size - column) % 10);
                  row 

  /*recursion_number += 0.5;
               (recursion_number == 1) && (sum = 0);
               (recursion_number > 1.5) && (sum += (size + 2*(size - 1) + size - 3 + 1) - counter2*16);
               printf("<%lf> +%d+",recursion_number,sum);   + (row - 3) 
               (number = 10 - (sum + column - (row - 1) + 10 - first_number - 1) % 10);*/ 
                  //printf("+%d+",sum);  



  /*for(counter2 = 1; counter2 <= size*size; ++counter2){
        //Special consideration of the first row.  
        //printf("<%d>",counter2);
        (counter2 == 1) && printf("%d",number_horizontal1);
        (counter2 > 1)  && (counter2 < size) && (number_horizontal1 -= 1) && printf("%d",number_horizontal1) && (number_vertical1 = number_horizontal1);
        if((counter2 != size*size) && (counter2 == (1 + 2*round)*size - 2*round) && ((counter2 + 2*round) % size == 0)){
    	    number_vertical1 -= 1;
    	    printf("%d\n",number_vertical1);
    	    number_horizontal1 = number_vertical1;
    	}
    	/*if(row % 2 != 0){
         /*
         for(counter2 = 0; counter2 <= recursion_number; ++counter2){
             (recursion_number == 1) && (sum = 0);
             (recursion_number != 1) && (sum += (size + 2*(size - 1) + size - 3 + 1) - counter2*16);

             


          }*
           (row % 2 != 0) && (row != 1) && (column == row - 1) && (recursion_number += 1);
           (recursion_number == 1) && (sum = 0);
           (recursion_number != 1) && (sum += (size + 2*(size - 1) + size - 3 + 1) - counter2*16);
           printf("<%d>",recursion_number);
           (row % 2 != 0) && (number = 10 - (sum + column - (row - 1) + 10 - first_number - 1) % 10);     
      }*/
    	
    	/*
    	
    	else if(counter2 == 2*(round + 1)*size + 2*(round + 1) - 1){
    		number_horizontal2 -= 1;
    	    printf("%d",number_horizontal2);
    	    //round += 1;
    	}
    	else if((counter2 != size) && (counter2 <= size*(size - 2*round) - 2*round) && (counter2 >= size*(size - 2*round - 1) + 2*round + 1 )){
    	    (turn_off == 1) && (number_horizontal1 -= size + 1) && (turn_off = 2);
    	    number_horizontal1 += 1;
            printf("%d",number_horizontal1);
            number_horizontal2 = number_horizontal1; 
    	}
    	
        else if(counter2 > size){
    		printf("-");
    	}
    	*/
        /*
    	else if(counter2 > size && (counter2 > 2*round*size + 2*round - 1) && (counter2 <= size*(size - 2*round) - 2*round)){
    		//printf("<%d>",counter2);
    		number -= 1;
    	    printf("%d",number);
    	}
    	
    	
    	
    	else if(counter2 > size){
    		printf("-");
    	}
        
    	(number_horizontal1 == 0) && (number_horizontal1 = 10) && (number_vertical1 = 10);
        */