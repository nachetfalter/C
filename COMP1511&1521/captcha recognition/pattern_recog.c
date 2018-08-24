#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "captcha.h"
//I need to use pi for roundness calculation
#define pi 3.1415926

void pattern_recognition(int height, int width, int pixels[height][width],int start_row[4], int start_column[4], double box_height[4], double box_width[4], int box_area[4], int entity_amount, int final_answer[4]){
	//Utility:
	int row_1 = 0, column_1 = 0, column_2 = 0;
	int first_column1 = 0, first_column2 = 0, last_column1 = 0, last_column2 = 0, last_column3 = 0;
	int counter1 = 0, counter2 = 0;
	int switch_1 = 0, switch_2 = 0, switch_3 = 0;
	int hole_counter1 = 0, hole_counter2 = 0, half_hole_counter = 0;
	time_t t;
	int index = 0;

	//Attributes
	double ratio = 0;
	double hole_area = 0;
	int dummy_pixels[height][width];
	int holed_guess1[7], holed_guess2[2], guess3[3], guess4[9];
	double perimeter1 = 0, perimeter2 = 0;
	double area1 = 0, area2 = 0, area3 = 0, area4 = 0;
	double roundness1 = 0, roundness2 = 0;

    /*-----------------------------------------------------------------------------------------------------------------------------*/
    
    //INITIALIZATION

    //Seed the rand
    srand((unsigned) time(&t));
    
    //Copy the final image from capture
	for(row_1 = 0; row_1 < height - 1; ++row_1){
		for(column_1 = 0; column_1 < width; ++column_1){
			dummy_pixels[row_1][column_1] = pixels[row_1][column_1];
		}
	}
    
    //This guess is for if I somehow got three holes, I can still guess the number. It is likely they are in one of these numbers, especially the two with double hole potential
	holed_guess1[0] = 0;
	holed_guess1[1] = 4;
	holed_guess1[2] = 6;
	holed_guess1[3] = 8;
	holed_guess1[4] = 9;
	holed_guess1[5] = 0;
	holed_guess1[6] = 8;
    
    //This is for if I really can't figure out the number is 4 or 9.
	holed_guess2[0] = 4;
	holed_guess2[1] = 9;

    //This is for the hardest 3 numbers, if somehow I just can't figure them out, then I toss the coin
    guess3[0] = 1;
    guess3[0] = 3;
    guess3[0] = 7;

    //This is the last option, if I completely messed up, I can still guess random numbers
    for(counter2 = 0; counter2 < 9; ++ counter2){
    	guess4[counter2] = counter2;
    }

	/*-----------------------------------------------------------------------------------------------------------------------------*/

    //PREPARE ATTRIBUTES

	//Hole Detection
    switch_1 = 0;
    switch_2 = 0;
    hole_counter1 = 0;
    hole_area = 0;
    //Only loop through as many times as the number count
	for(counter1 = 0; counter1 < entity_amount; ++counter1){
        hole_counter1 = 0;
		//Holes detection. This is basicly just scan through, if got non hole tile, hole tile and non hole tile in a row, add one to hole counter.
        for(row_1 = start_row[counter1]; row_1 < start_row[counter1] + box_height[counter1]; ++row_1){
        	switch_2 = 0;
        	for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
        		if(switch_1 == 0 && pixels[row_1][column_1] == 2){
        			switch_1 = 1;
        			hole_area += 1;
        			hole_counter1 += 1;
        		}
        		if(switch_1 == 1 && pixels[row_1][column_1] == 2){
        			switch_2 = 1;
        			hole_area += 1;
        		}
        	}
        	if(switch_1 == 1 && switch_2 == 0){
        		// This is here just to double check if I messed up the hole elimination, this doesn't clear small holes, but subtract them from total hole counter
        		if(hole_area / (72 * 200) < 0.002) hole_counter2 += 1; 
    			switch_1 = 0;
    			switch_2 = 0;
    			hole_area = 0;
    		}
        }

        hole_counter1 -= hole_counter2;
        //printf("%d\n",hole_counter1);

        /*-----------------------------------------------------------------------------------------------------------------------------*/

        //Find the perimeter and roundness of the digit.
    	perimeter1 = 0;
    	area1 = 0;
    	for(row_1 = start_row[counter1]; row_1 < (start_row[counter1] + box_height[counter1]); ++row_1){
            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
            	if(pixels[row_1][column_1] == 1){
            	   if(pixels[row_1 - 1][column_1] == 0){
            	   	  perimeter1 += 1;
            	   }
            	   if(pixels[row_1 + 1][column_1] == 0){
            	   	  perimeter1 += 1;
            	   }
            	   if(pixels[row_1][column_1 - 1] == 0){
            	   	  perimeter1 += 1;
            	   }
            	   if(pixels[row_1][column_1 + 1] == 0){
            	   	  perimeter1 += 1;
            	   }      
            	}
            	if(pixels[row_1][column_1] == 1 || pixels[row_1][column_1] == 2){
                   area1 += 1;
            	}	
        	}
        }

        //This is the formula I use to determine how close to circle the digit is, the more it likes 0, the closer the roundness is to 1 
        if(perimeter1 > 0){
           roundness1 = (4 * area1 * pi) / (perimeter1 * perimeter1);
        }
        
        /*-----------------------------------------------------------------------------------------------------------------------------*/

        //ANALYSE

        //MORE THAN TWO HOLES

        //If there are more than two holes in the digit, I just guess and pray:
        index = 0;
        index = rand() % 7;
        if(hole_counter1 > 2){
        	final_answer[counter1] = holed_guess1[index];
        }

        /*-----------------------------------------------------------------------------------------------------------------------------*/
         
        //TWO HOLES

        //If there are two holes in the digit, if it is closer to circle, it must be 0:
        else if(hole_counter1 == 2){   	
	        if(roundness1 >= 0.53){
	        	final_answer[counter1] = 0;
	        }
	        else{
	        	final_answer[counter1] = 8;
	        }
        }

        /*-----------------------------------------------------------------------------------------------------------------------------*/

        //ONE HOLE

        //If there are one hole in the digit:
        else if(hole_counter1 == 1){
        	//Exclude 0 first.
        	if(roundness1 >= 0.5){
        		final_answer[counter1] = 0;
        	}
        	else{
        		area1 = 0;
        		area2 = 0;
        		perimeter1 = 0;
        		perimeter2 = 0;
        		roundness1 = 0;
        		roundness2 = 0;

        		//Calculate perimeter and area of half of the digit.
        		//First half
        		for(row_1 = start_row[counter1]; row_1 < start_row[counter1] + (box_height[counter1]) / 2; ++row_1){
		            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
		            	if(pixels[row_1][column_1] == 1){
		            	   if(pixels[row_1 - 1][column_1] == 0){
		            	   	  perimeter1 += 1;
		            	   }
		            	   if(pixels[row_1 + 1][column_1] == 0){
		            	   	  perimeter1 += 1;
		            	   }
		            	   if(pixels[row_1][column_1 - 1] == 0){
		            	   	  perimeter1 += 1;
		            	   }
		            	   if(pixels[row_1][column_1 + 1] == 0){
		            	   	  perimeter1 += 1;
		            	   }      
		            	}
		            	if(pixels[row_1][column_1] == 1 || pixels[row_1][column_1] == 2){
		            		area1 += 1;
		            	}	
		        	}
		        }
                
                //Second half
		        for(row_1 = start_row[counter1] + (box_height[counter1]) / 2; row_1 < (start_row[counter1] + box_height[counter1]); ++row_1){
		            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
		            	if(pixels[row_1][column_1] == 1){
		            	   if(pixels[row_1 - 1][column_1] == 0){
		            	   	  perimeter2 += 1;
		            	   }
		            	   if(pixels[row_1 + 1][column_1] == 0){
		            	   	  perimeter2 += 1;
		            	   }
		            	   if(pixels[row_1][column_1 - 1] == 0){
		            	   	  perimeter2 += 1;
		            	   }
		            	   if(pixels[row_1][column_1 + 1] == 0){
		            	   	  perimeter2 += 1;
		            	   }      
		            	}
		            	if(pixels[row_1][column_1] == 1 || pixels[row_1][column_1] == 2){
		            		area2 += 1;
		            	}	
		        	}
		        }

		        roundness1 = (4 * area1 * pi) / (perimeter1 * perimeter1);
		        roundness2 = (4 * area2 * pi) / (perimeter2 * perimeter2);
                
                //If the bottom half is closer to circle then it must be 6
		        if(roundness1 > roundness2 && roundness1 - roundness2 >= 0.1){
		        	final_answer[counter1] = 6;
		        }
		        else{
		        	area1 = 0;
	        		perimeter1 = 0;
	        		roundness1 = 0;
	        		//This is here to check the roundness of the hole within the digit
	        		for(row_1 = start_row[counter1]; row_1 < (start_row[counter1] + box_height[counter1]); ++row_1){
			            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
			            	if(pixels[row_1][column_1] == 2){
			            	   if(pixels[row_1 - 1][column_1] == 1){
			            	   	  perimeter1 += 1;
			            	   }
			            	   if(pixels[row_1 + 1][column_1] == 1){
			            	   	  perimeter1 += 1;
			            	   }
			            	   if(pixels[row_1][column_1 - 1] == 1){
			            	   	  perimeter1 += 1;
			            	   }
			            	   if(pixels[row_1][column_1 + 1] == 1){
			            	   	  perimeter1 += 1;
			            	   }      
			            	}
			            	if(pixels[row_1][column_1] == 2){
		            		   area1 += 1;
		            	    }		
			        	}
			        }

			        if(perimeter1 > 0) roundness1 = (4 * area1 * pi) / (perimeter1 * perimeter1);
                    
                    //If the hole is close to a circle, it is 9, other wise it is 4, if unsure just guess.
			        if(roundness2 >= 0.93 && roundness1 >= 0.38){
			        	final_answer[counter1] = 9;
			        }
			        else if(roundness2 < 0.93 && roundness1 < 0.45){
			        	final_answer[counter1] = 4;
			        }
			        else{
			        	index = 0;
				        index = rand() % 2;
				        final_answer[counter1] = holed_guess2[index];
			        }    
		        }
        	}		
        }

        /*-----------------------------------------------------------------------------------------------------------------------------*/

        //NO HOLE

        //If there is no hole in the digit:
        else{
        	//Get the smallest and biggest column in the first half
        	area1 = 0;
        	area2 = 0;
        	column_2 = 0;
            first_column1 = 200;
            last_column1 = 0;
            last_column2 = 0;
        	for(row_1 = start_row[counter1]; row_1 < start_row[counter1] + (box_height[counter1]) / 2; ++row_1){
	            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
	            	if(pixels[row_1][column_1] == 1){
	            		area1 += 1;
	            		if(first_column1 > column_1){
	            			first_column1 = column_1;
	            		}
	            		if(last_column1 < column_1){
	            			last_column1 = column_1;
	            		}
	            	}	
	        	}
	        }

            last_column2 = last_column1;
            last_column1 = 0;

            //Reverse fill up each row in the first half to its smallest column
            for(row_1 = start_row[counter1]; row_1 < start_row[counter1] + (box_height[counter1]) / 2; ++row_1){
	            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
	            	if(pixels[row_1][column_1] == 1){
	            		if(last_column1 < column_1){
	            			last_column1 = column_1;
	            		}            		
	            	}
	            	if(column_1 == start_column[counter1] + box_width[counter1] - 1){
	            		for(column_2 = first_column1; column_2 < last_column1; ++column_2){
	            			dummy_pixels[row_1][column_2] = 1;
	            		}
	            	}
	            }
	            last_column1 = 0;	            		
	        }
            
            //Get the smallest and biggest column in the second half
	        first_column2 = 200;
            column_2 = 0;
            last_column1 = 0;
	        for(row_1 = start_row[counter1] + (box_height[counter1]) / 2; row_1 < (start_row[counter1] + box_height[counter1]); ++row_1){
	            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
	            	if(pixels[row_1][column_1] == 1){
	            		area2 += 1;
	            		if(first_column2 > column_1){
	            			first_column2 = column_1;
	            		}
	            		if(last_column1 < column_1){
	            			last_column1 = column_1;
	            		} 
	            	}	
	        	}
	        }

            last_column3 = last_column1;
	        last_column1 = 0;

	        //Reverse fill up each row in the second half to its smallest column
	        for(row_1 = start_row[counter1] + (box_height[counter1]) / 2; row_1 < (start_row[counter1] + box_height[counter1]); ++row_1){
	            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
	            	if(pixels[row_1][column_1] == 1){
	            		if(last_column1 < column_1){
	            			last_column1 = column_1;
	            		}            		
	            	}
	            	if(column_1 == start_column[counter1] + box_width[counter1] - 1){
	            		for(column_2 = first_column2; column_2 < last_column1; ++column_2){
	            			dummy_pixels[row_1][column_2] = 1;
	            		}
	            	}
	            }
	            last_column1 = 0;	 
	        }

    		area3 = 0;
    		area4 = 0;
    		perimeter1 = 0;
    		perimeter2 = 0;
    		roundness1 = 0;
    		roundness2 = 0;
    		//Calculate perimeter and area of the first half of the filled digit.
    		for(row_1 = start_row[counter1]; row_1 < start_row[counter1] + (box_height[counter1]) / 2; ++row_1){
	            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
	            	if(dummy_pixels[row_1][column_1] == 1){
	            	   if(dummy_pixels[row_1 - 1][column_1] == 0){
	            	   	  perimeter1 += 1;
	            	   }
	            	   if(dummy_pixels[row_1 + 1][column_1] == 0){
	            	   	  perimeter1 += 1;
	            	   }
	            	   if(dummy_pixels[row_1][column_1 - 1] == 0){
	            	   	  perimeter1 += 1;
	            	   }
	            	   if(dummy_pixels[row_1][column_1 + 1] == 0){
	            	   	  perimeter1 += 1;
	            	   }      
	            	}
	            	if(dummy_pixels[row_1][column_1] == 1){
	            		area3 += 1;
	            	}	
	        	}
	        }
            
            //Calculate perimeter and area of the first half of the filled digit.
	        for(row_1 = start_row[counter1] + (box_height[counter1]) / 2; row_1 < (start_row[counter1] + box_height[counter1]); ++row_1){
	            for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
	            	if(dummy_pixels[row_1][column_1] == 1){
	            	   if(dummy_pixels[row_1 - 1][column_1] == 0){
	            	   	  perimeter2 += 1;
	            	   }
	            	   if(dummy_pixels[row_1 + 1][column_1] == 0){
	            	   	  perimeter2 += 1;
	            	   }
	            	   if(dummy_pixels[row_1][column_1 - 1] == 0){
	            	   	  perimeter2 += 1;
	            	   }
	            	   if(dummy_pixels[row_1][column_1 + 1] == 0){
	            	   	  perimeter2 += 1;
	            	   }      
	            	}
	            	if(dummy_pixels[row_1][column_1] == 1){
	            		area4 += 1;
	            	}	
	        	}
	        }

	        roundness1 = (4 * area3 * pi) / (perimeter1 * perimeter1);
	        roundness2 = (4 * area4 * pi) / (perimeter2 * perimeter2);

            //Calculate the height/width ratio of the digit
	        ratio = box_height[counter1]/box_width[counter1];

            //If the top half is closer to circle after filling 
            if(roundness2 > roundness1 && roundness1 < 0.68 && roundness2 > 0.86 && ratio < 2.32 && (fabs(roundness2 - roundness1) > 0.25 || fabs(area4 - area3) > 150)){
            	final_answer[counter1] = 2;
            }
            //If the bottom half is closer to circle after filling
            else if(roundness1 > roundness2 && roundness2 < 0.68 && roundness1 > 0.86 && ratio < 2.32 && (fabs(roundness2 - roundness1) > 0.25 || fabs(area4 - area3) > 150)){
            	final_answer[counter1] = 5;
            }
            //If the ratio is really weird
            else if(ratio > 2.4){
            	final_answer[counter1] = 1;
            }
            else{
            	//Check vertical unfilled space between pixels, I didn't get this to work as I wanted.
            	switch_1 = 0;
			    switch_2 = 0;
			    half_hole_counter = 0;
			    counter2 = 0;
		        for(column_1 = start_column[counter1]; column_1 < start_column[counter1] + box_width[counter1]; ++column_1){
		        	half_hole_counter = 0;
		        	switch_2 = 0;
		        	counter2 = 0;
		        	for(row_1 = start_row[counter1]; row_1 < start_row[counter1] + box_height[counter1]; ++row_1){
		        		if(switch_1 == 0 && pixels[row_1][column_1] == 1 && pixels[row_1][column_1 - 1] != 1 && pixels[row_1][column_1 + 1] != 1){
		        			counter2 += 1;
		        		}
		        		if(counter2 >= 2){
		        			break;
		        		}
		        		if(switch_1 == 0 && pixels[row_1][column_1] == 1 && pixels[row_1][column_1 - 1] == 1 && pixels[row_1][column_1 + 1] == 1 ){
		        			switch_1 = 1;
		        			half_hole_counter += 1;
		        		}
		        		if(switch_1 == 1 && pixels[row_1][column_1] == 0){
		        			switch_1 = 0;
		        		}
		        	}
		        	if(half_hole_counter == 2){
		        		break;
		    		}
		        }
                
                //But it still works on some special 1 netherless.
		        if(half_hole_counter == 0){
		        	final_answer[counter1] = 1;
		        }

		        //If the bottom and top are both close to circle and has similar area
		        if(fabs(perimeter1 - perimeter2) < 20 && fabs(area3 - area4) < 300 && fabs(area1 - area2) < 150 
		        	    && fabs(roundness1 - roundness2) < 0.23 && roundness1 > 0.86 && roundness2 > 0.86){
		        	final_answer[counter1] = 3;
		        }

		        //If the last column in the top half is smaller than the one in the bottom, it is likely the digit doesn't have a curved shape, thus 1.
		        else if(last_column2 > last_column3){
		        	final_answer[counter1] = 1;
		        }

		        //If the last column in bottom half is smaller than the one at the top, it is likely the digit is curved, thus 7.
		        else if(last_column2 < last_column3){
		        	final_answer[counter1] = 7;
		        }

		        //Well, I don't know anymore :p
		        else{
		        	index = 0;
        			index = rand() % 3;
		        	final_answer[counter1] = guess3[index];
		        }
		    }
		}
	}
    
    //If I did so poorly, this would be my last option. To fill any numbers that is not captured or captured wrongly with random guess.
	for(counter2 = 0; counter2 < 4; ++counter2){
		if(final_answer[counter2] < 0 || final_answer[counter2] > 9){
			index = 0;
        	index = rand() % 9;
        	final_answer[counter2] = guess4[index];
		}
	}
}		        