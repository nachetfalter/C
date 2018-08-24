#include <stdio.h>
#include <stdlib.h>
#include "captcha.h"

void capture(int height, int width, int pixels[height][width],int start_row[4], int start_column[4], double box_height[4], double box_width[4], int box_area[4], int final_image[height][width], int* entity_amount){
	//Untility:
	int column1 = 0, column2 = 0, line_finish = 0, line_start = 0, line_finish2 = 0, line_start2 = 0;
	int counter = 0;
	int temp_row_1 = 0, temp_row_2 = 0, temp_row_3 = 0, temp_row_4 = 0, temp_column_1 = 0, temp_column_2 = 0, temp_column_3 = 0, temp_column_4 = 0; 
	int switch_1 = 0, switch_2 = 0, switch_3 = 0, switch_4 = 0, switch_5 = 0, switch_6 = 0;
    int column123, row123;
    
	//Entity:
	double entity_area[14400 / 4];
	int entity_counter1 = 0, entity_counter2 = 0, entity_counter3 = 0;
	int entity_coordination[height][width];
	int entity_order1[4], entity_order2[4];
	double largest_area1 = 0, largest_area2 = 0, largest_area3 = 0, largest_area4 = 0;
	int entity_index1 = 0, entity_index2 = 0, entity_index3 = 0, entity_index4 = 0;

	//Hole:
	double hole_area[14400 / 4];
	int hole_counter = 0;
	int hole_coordination[height][width];

	//Attributes:
	int temp_start_column[14400 / 4], temp_start_row[14400 / 4]; 
	int max_column = 0;
	int max_row = 0;

	/*-----------------------------------------------------------------------------------------------------------------------------*/

    //INITIALIZATION:

    //Initialize coordination arraies. This array would be used to eliminate noise and record position.
	for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
		for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
			entity_coordination[temp_row_1][temp_column_1] = -1;
			hole_coordination[temp_row_1][temp_column_1] = -1;
		}
	}
    
    //Initialize attribute arraies.
	for(counter = 0; counter < 14400 / 4; ++counter){
		if(counter < 4){
			entity_order1[counter] = -2; //I fill them with unusual numbers so I won't encounter problem later.
			entity_order2[counter] = -2;
			box_height[counter] = -1;
    	    box_width[counter] = -1;
    	    box_area[counter] = -1;
    	    start_row[counter] = -1;
    	    start_column[counter] = width;
        }
		hole_area[counter] = 0;
		entity_area[counter] = 0;
		temp_start_row[counter] = -1;
    	temp_start_column[counter] = -1;
	}

    /*-----------------------------------------------------------------------------------------------------------------------------*/
    
    //FILL UP HOLES:
    
    //You will see I redeclare variable value every time when I use them, this is to be safe.
    switch_1 = 0;
    switch_2 = 0;

    //Hole detection and coordination recording.
    for(temp_row_1 = 1; temp_row_1 < height - 1; ++temp_row_1){
		for(temp_column_1 = 1; temp_column_1 < width; ++temp_column_1){
		    if(pixels[temp_row_1][temp_column_1] == 0 && pixels[temp_row_1][temp_column_1 - 1] == 1 && pixels[temp_row_1 - 1][temp_column_1] == 1){

		    	//Vertical check
		    	//Get upper boundary y coordination then check left and right border in the way.
		    	for(temp_row_2 = temp_row_1; temp_row_2 < height - 1; ++temp_row_2){
		    		if(pixels[temp_row_2][temp_column_1] == 1){
		    			break;
		    		}
		    	}
		    	
                for(temp_row_3 = temp_row_1; temp_row_3 < temp_row_2; ++temp_row_3){
                	switch_1 = 0;
                	for(temp_column_2 = temp_column_1; temp_column_2 - temp_column_1 < 25; ++temp_column_2){ //Half of the width of 1/4 of picture
                		if(temp_column_2 < width){
                			if(pixels[temp_row_3][temp_column_2] == 1){
	                	    	switch_1 += 1;
				    			break;
				    		}
                		}    	    
                	}
                	for(temp_column_2 = temp_column_1; temp_column_1 - temp_column_2 < 25; --temp_column_2){
                		if(temp_column_2 > 0){
	                	    if(pixels[temp_row_3][temp_column_2] == 1){
	                	    	switch_1 += 1;
				    			break;
				    		}
				        }		
                	}
                	if(switch_1 != 2) break;
                }

                //Horizontal check
                //Get right boundary x coordination, same priciple as the module above, but reverse.
                for(temp_column_3 = temp_column_1; temp_column_3 < width; ++temp_column_3){
		    		if(pixels[temp_row_2][temp_column_1] == 1){
		    			break;
		    		}
		    	}

		    	for(temp_column_4 = temp_column_1; temp_column_4 <= temp_column_3; ++temp_column_4){
                	switch_2 = 0;
                	for(temp_row_4 = temp_row_1; temp_row_1 - temp_row_4 < 48; --temp_row_4){ //2/3 of the height
                		if(temp_row_4 > 0){
	                	    if(pixels[temp_row_4][temp_column_4] == 1){
	                	    	switch_2 += 1;
				    			break;
				    		}
				    	}	
                	}
                	if(temp_column_3 - temp_column_1 == 1 && switch_2 == 2) break;
                	if(switch_2 != 1) break;
                }

		    	//If upper and right boundary is discovered then record pixel coordination as hole	    	
		    	if(switch_1 == 2 && switch_2 == 1){
		    	    pixels[temp_row_1][temp_column_1] = 1;
		    	    hole_coordination[temp_row_1][temp_column_1] = 1;
		    	    switch_1 = 0;
		    	    switch_2 = 0;
		    	}
		    	else{
		    		switch_1 = 0;
		    	    switch_2 = 0;
		    	}	    	
		    }
		}
    }

    //Temporarily eliminite small holes to allow better entity reading
	for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
		for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
	        if(hole_coordination[temp_row_1][temp_column_1] != -1 && 
	          ((hole_coordination[temp_row_1 - 1][temp_column_1] == -1 && hole_coordination[temp_row_1 + 1][temp_column_1] == -1) || 
	          (hole_coordination[temp_row_1][temp_column_1 - 1] == -1 && hole_coordination[temp_row_1][temp_column_1 + 1] == -1)))
	        {
	       	   pixels[temp_row_1][temp_column_1] = 0;
	        }		
        }
    }
    
    /*-----------------------------------------------------------------------------------------------------------------------------*/
    
    //ENTITY NOISE ELIMINATION:
    
    //Noise elimination module, basicly this capture entities in the image one by one, start from its first left bottom picture, and grow from it until it reach its proper border.
    switch_6 = 1;
    while(switch_6 != 0){
	    for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
			for(temp_column_1 = 0; temp_column_1 < width - 1; ++temp_column_1){
			    if(pixels[temp_row_1][temp_column_1] == 1){
			    	temp_start_row[entity_counter1] = temp_row_1;
			    	temp_start_column[entity_counter1] = temp_column_1;
	                entity_coordination[temp_row_1][temp_column_1] = entity_counter1;
			    	break;
			    }
			}
			if(pixels[temp_row_1][temp_column_1] == 1){
				break;
			}
		}
	    
	    temp_row_1 = temp_start_row[entity_counter1];
	    for(temp_column_1 = temp_start_column[entity_counter1]; pixels[temp_row_1][temp_column_1] != 0; ++temp_column_1){
	    	line_finish = temp_column_1;
	    }

	    column1 = temp_start_column[entity_counter1];
	    line_start = 1;
	    for(temp_row_1 = temp_start_row[entity_counter1]; line_start != width; ++temp_row_1){
	    	switch_1 = 0;
	    	line_start = width;
	    	for(temp_column_1 = column1; switch_1 == 0 || pixels[temp_row_1][temp_column_1] != 0; ++temp_column_1){
	    		column1 = 0;	   		
	    		if(pixels[temp_row_1][temp_column_1] == 1){
		    		entity_area[entity_counter1] += 1;
		    		entity_coordination[temp_row_1][temp_column_1] = entity_counter1;
		    		if(pixels[temp_row_1 + 1][temp_column_1] == 1 && line_start >= temp_column_1){
		    			line_start = temp_column_1;
		    		}
		    		if(pixels[temp_row_1 + 1][temp_column_1] == 1 && line_finish <= temp_column_1){
		    			line_finish = temp_column_1;
		    		}
		    		pixels[temp_row_1][temp_column_1] = 0;
		    	}
		    	if(line_finish == temp_column_1){
		    		switch_1 = 1;
		    	}
	    	}
	    	column1 = line_start;
	    	if(column1 < width){
	    	   for(temp_column_1 = column1; pixels[temp_row_1 + 1][temp_column_1] != 0; --temp_column_1){
			    	if(pixels[temp_row_1 + 1][column1 - 1] == 1){
				    	column1 -= 1; 
				    }
				}
	    	} 
	    	
			line_start2 = 0;
			for(temp_column_1 = line_start; temp_column_1 <= line_finish; ++temp_column_1){
				if(pixels[temp_row_1 - 1][temp_column_1] == 1){
					line_start2 = 1;
					break;
				}
			}

			if(line_start2 == 1){
				temp_column_2 = temp_column_1;
				temp_row_2 = temp_row_1 - 1;
				column2 = 0;
				while(pixels[temp_row_2][temp_column_2] != 0){
					column2 = temp_column_2;
					temp_column_2 -= 1;
			    }
				for(temp_row_2 = temp_row_1 - 1; line_start2 != width; --temp_row_2){
					line_start2 = width;
					for(temp_column_2 = column2; pixels[temp_row_2][temp_column_2] != 0; ++temp_column_2){
						if(pixels[temp_row_2][temp_column_2] == 1){

							entity_area[entity_counter1] += 1;
	    		            entity_coordination[temp_row_2][temp_column_2] = entity_counter1;
	    		            if(pixels[temp_row_2 - 1][temp_column_2] == 1 && line_start2 >= temp_column_2){
				    			line_start2 = temp_column_2;
				    		}
				    		if(pixels[temp_row_2 - 1][temp_column_2] == 1 && line_finish2 <= temp_column_2){
				    			line_finish2 = temp_column_2;
				    		}
				    		column2 = line_start2;
	    		            pixels[temp_row_2][temp_column_2] = 0;
						}
						while(pixels[temp_row_2][temp_column_2] != 0){
							column2 = temp_column_2;
							temp_column_2 -= 1;
					    }
					}
				}
			}
	    }
	    for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
	    	for(temp_column_1 = 0; temp_column_1 < width - 1; ++temp_column_1){
	    		if(pixels[temp_row_1][temp_column_1] == 1){
	    			switch_6 = 1;
	    			break;
	    		}
	    		else{
	    			switch_6 = 0;
	    		}
	    	}
	    	if(pixels[temp_row_1][temp_column_1] == 1){
    			switch_6 = 1;
    			break;
	    	}
	    }
	    entity_counter1 += 1;
	}

    //Eliminate entities that are too small
	for(counter = 0; counter < entity_counter1; ++counter){
		if(entity_area[counter] / (72 * 200) < 0.015){ //After test I found 0.015 to be good. The reason I hard coded the area is because I use the same module for single digit and 4 digits
			entity_area[counter] = 0;
			for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
				for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
			        if(entity_coordination[temp_row_1][temp_column_1] == counter){
			           entity_coordination[temp_row_1][temp_column_1] = -1;
			       	   pixels[temp_row_1][temp_column_1] = 0;
			        }		
		        }
		    }
        }
    }
    
    largest_area1 = 0;
    largest_area4 = 14400;
	//Entity noise elimination by area order, I only pick four largest entities in the image.
	for(counter = 0; counter < entity_counter1; ++counter){
		if(entity_area[counter] > 0){
			if(largest_area1 <= entity_area[counter]){
				largest_area1 = entity_area[counter];
				entity_index1 = counter;
			}
			if(largest_area4 >= entity_area[counter]){
				largest_area4 = entity_area[counter];
				entity_index4 = counter;
			}
	    }
	}

	entity_area[entity_index1] *= -1;
	entity_area[entity_index4] *= -1;
    
    largest_area2 = 0;
    for(counter = 0; counter < entity_counter1; ++counter){
			if(largest_area2 <= entity_area[counter] && entity_area[counter] <= largest_area1 && entity_area[counter] >= largest_area4){
				largest_area2 = entity_area[counter];
				entity_index2 = counter;
			}
	}
    
    entity_area[entity_index2] *= -1;
    

	largest_area3 = 0;
    for(counter = 0; counter < entity_counter1; ++counter){
			if(largest_area3 <= entity_area[counter] && entity_area[counter] <= largest_area1 && entity_area[counter] >= largest_area4 && entity_area[counter] <= largest_area2){
				largest_area3 = entity_area[counter];
				entity_index3 = counter;
			}
	}  

	for(counter = 0; counter < entity_counter1; ++counter){
    	if(entity_area[counter] < 0){
			entity_area[counter] *= -1;
		}
	}

    //Entity noise elimination implementation. Any pixels in entity with unrecognised index will be reset to 0. And they are all eliminated in the background.
	for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
		for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
			if(entity_coordination[temp_row_1][temp_column_1] != entity_index1 && 
				entity_coordination[temp_row_1][temp_column_1] != entity_index2 && 
				entity_coordination[temp_row_1][temp_column_1] != entity_index3 && 
				entity_coordination[temp_row_1][temp_column_1] != entity_index4)
			{
			   pixels[temp_row_1][temp_column_1] = 0;
			   entity_coordination[temp_row_1][temp_column_1] = -1;
			}
		}
	}
   
    /*-----------------------------------------------------------------------------------------------------------------------------*/   

    //HOLE NOISE ELIMINATION:

    //Unfill holes
	for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
		for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
			if(hole_coordination[temp_row_1][temp_column_1] == 1){
				pixels[temp_row_1][temp_column_1] = 1;
			}
		}
	}
   
    //Initialize again just to be sure
    for(counter = 0; counter < 14400 / 4; ++counter){
		hole_area[counter] = 0;
		temp_start_row[counter] = -1;
    	temp_start_column[counter] = -1;
	}

	//Noise elimination module used on holes, so this just turn the noise elimination module around, but keep the same principle.
    column1 = 0;
    column2 = 0;
    switch_6 = 1;
    hole_counter = 0;
    while(switch_6 != 0){
	    for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
			for(temp_column_1 = 0; temp_column_1 < width - 1; ++temp_column_1){
			    if(pixels[temp_row_1][temp_column_1] == 1){
			    	temp_start_row[hole_counter] = temp_row_1;
			    	temp_start_column[hole_counter] = temp_column_1;
	                hole_coordination[temp_row_1][temp_column_1] = hole_counter;
			    	break;
			    }
			}
			if(pixels[temp_row_1][temp_column_1] == 1){
				break;
			}
		}
	    
	    temp_row_1 = temp_start_row[hole_counter];
	    for(temp_column_1 = temp_start_column[hole_counter]; pixels[temp_row_1][temp_column_1] != 0; ++temp_column_1){
	    	line_finish = temp_column_1;
	    }

	    column1 = temp_start_column[hole_counter];
	    line_start = 1;
	    for(temp_row_1 = temp_start_row[hole_counter]; line_start != width; ++temp_row_1){
	    	switch_1 = 0;
	    	line_start = width;
	    	for(temp_column_1 = column1; switch_1 == 0 || pixels[temp_row_1][temp_column_1] != 0; ++temp_column_1){
	    		column1 = 0;	   		
	    		if(pixels[temp_row_1][temp_column_1] == 1){
		    		hole_area[hole_counter] += 1.0;
		    		hole_coordination[temp_row_1][temp_column_1] = hole_counter;
		    		if(pixels[temp_row_1 + 1][temp_column_1] == 1 && line_start >= temp_column_1){
		    			line_start = temp_column_1;
		    		}
		    		if(pixels[temp_row_1 + 1][temp_column_1] == 1 && line_finish <= temp_column_1){
		    			line_finish = temp_column_1;
		    		}
		    		pixels[temp_row_1][temp_column_1] = 0;
		    	}
		    	if(line_finish == temp_column_1){
		    		switch_1 = 1;
		    	}
	    	}
	    	column1 = line_start;
	    	if(column1 < width){
	    	   for(temp_column_1 = column1; pixels[temp_row_1 + 1][temp_column_1] != 0; --temp_column_1){
			    	if(pixels[temp_row_1 + 1][column1 - 1] == 1){
				    	column1 -= 1; 
				    }
			    }
	    	}
	    	
			line_start2 = 0;
			for(temp_column_1 = line_start; temp_column_1 <= line_finish; ++temp_column_1){
				if(pixels[temp_row_1 - 1][temp_column_1] == 1){
					line_start2 = 1;
					break;
				}
			}

			if(line_start2 == 1){
				temp_column_2 = temp_column_1;
				temp_row_2 = temp_row_1 - 1;
				column2 = 0;
				while(pixels[temp_row_2][temp_column_2] != 0){
					column2 = temp_column_2;
					temp_column_2 -= 1;
			    }
				for(temp_row_2 = temp_row_1 - 1; line_start2 != width; --temp_row_2){
					line_start2 = width;
					for(temp_column_2 = column2; pixels[temp_row_2][temp_column_2] != 0; ++temp_column_2){
						if(pixels[temp_row_2][temp_column_2] == 1){

							hole_area[hole_counter] += 1;
	    		            hole_coordination[temp_row_2][temp_column_2] = hole_counter;
	    		            if(pixels[temp_row_2 - 1][temp_column_2] == 1 && line_start2 >= temp_column_2){
				    			line_start2 = temp_column_2;
				    		}
				    		if(pixels[temp_row_2 - 1][temp_column_2] == 1 && line_finish2 <= temp_column_2){
				    			line_finish2 = temp_column_2;
				    		}
				    		column2 = line_start2;
	    		            pixels[temp_row_2][temp_column_2] = 0;
						}
						while(pixels[temp_row_2][temp_column_2] != 0){
							column2 = temp_column_2;
							temp_column_2 -= 1;
					    }
					}
				}
			}
	    }
	    for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
	    	for(temp_column_1 = 0; temp_column_1 < width - 1; ++temp_column_1){
	    		if(pixels[temp_row_1][temp_column_1] == 1){
	    			switch_6 = 1;
	    			break;
	    		}
	    		else{
	    			switch_6 = 0;
	    		}
	    	}
	    	if(pixels[temp_row_1][temp_column_1] == 1){
    			switch_6 = 1;
    			break;
	    	}
	    }
	    hole_counter += 1;
	} 

    //Eliminate holes that are too small
	for(counter = 0; counter < hole_counter; ++counter){
		if(hole_area[counter] / (72 * 200) < 0.0023){ //To get this ratio is a pain... You just have to test it with weird numbers
			hole_area[counter] = 0;
			for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
				for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
			        if(hole_coordination[temp_row_1][temp_column_1] == counter){
			           hole_coordination[temp_row_1][temp_column_1] = -1;
			           if((entity_coordination[temp_row_1 - 1][temp_column_1] != -1 && entity_coordination[temp_row_1 + 1][temp_column_1] != -1)
			           	  || (entity_coordination[temp_row_1][temp_column_1 - 1] != -1 && entity_coordination[temp_row_1][temp_column_1 + 1] != -1)){
				           pixels[temp_row_1][temp_column_1] = 1;
				       	   hole_coordination[temp_row_1][temp_column_1] = -1;
			           }		       	   
			        }		
		        }
		    }
        }
    }

    /*-----------------------------------------------------------------------------------------------------------------------------*/ 

    //REFILL OUTPUT IMAGE
    
    //Reform the image into pure form, there should be only up to four entities in this image.
    for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
		for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
			if(entity_coordination[temp_row_1][temp_column_1] >= 0){
			   pixels[temp_row_1][temp_column_1] = 1;
			}
			if(hole_coordination[temp_row_1][temp_column_1] >= 0){
			   pixels[temp_row_1][temp_column_1] = 2;
			}
		}
	}

    /*-----------------------------------------------------------------------------------------------------------------------------*/

    //CHECK ORDER
    
    //Rearrange order of true entity. As if I just use the scan from left to right, I may capture numbers in the wrong order, so this is necessary. I start from mid pic and scan through the entities.
    temp_row_1 = height/2;
    switch_1 = 0;
    counter = 0;
    entity_order1[0] = -1;
    for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
    	if(switch_1 == 0 && pixels[temp_row_1][temp_column_1] == 1){
    		switch_1 = 1;
    		if(counter > 0){
	    		if(entity_order1[counter - 1] != entity_coordination[temp_row_1][temp_column_1]){
	    			entity_order1[counter] = entity_coordination[temp_row_1][temp_column_1];
	    		}
	    	}
	    	else{
	    		entity_order1[counter] = entity_coordination[temp_row_1][temp_column_1];
	    	}
    		counter += 1;
    		for(temp_row_2 = 0; temp_row_2 < height - 1; ++temp_row_2){
				for(temp_column_2 = 0; temp_column_2 < width - 1; ++temp_column_2){
					if(counter < 4){
					   if(entity_coordination[temp_row_2][temp_column_2] == entity_order1[counter]){
						  pixels[temp_row_2][temp_column_2] = 0;
					    }
					}
					
				}
	        } 
	    }
	    if(switch_1 == 1 && pixels[temp_row_1][temp_column_1] == 0){
	    	switch_1 = 0;
	    } 
    }
    
    //Refill after scan
    for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
		for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
			if(entity_coordination[temp_row_1][temp_column_1] >= 0){
			   pixels[temp_row_1][temp_column_1] = 1;
			}
			if(hole_coordination[temp_row_1][temp_column_1] >= 0){
			   pixels[temp_row_1][temp_column_1] = 2;
			}
			
		}
	}	

    //Eliminate potential faulty holes. This is essential for proper operation. If a 'hole' have 0 around it, then it would be terminated.
    counter = 0;
    for(counter = 0; counter < 200; ++counter){
		for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
			for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
				if(hole_coordination[temp_row_1][temp_column_1] >= 0){
				   if(pixels[temp_row_1][temp_column_1] == 2 && pixels[temp_row_1 - 1][temp_column_1] == 0){
						pixels[temp_row_1][temp_column_1] = 0;
					}
					if(pixels[temp_row_1][temp_column_1] == 2 && pixels[temp_row_1 + 1][temp_column_1] == 0){
						pixels[temp_row_1][temp_column_1] = 0;
					}
					if(pixels[temp_row_1][temp_column_1] == 2 && pixels[temp_row_1][temp_column_1 - 1] == 0){
						pixels[temp_row_1][temp_column_1] = 0;
					}
					if(pixels[temp_row_1][temp_column_1] == 2 && pixels[temp_row_1][temp_column_1 + 1] == 0){
						pixels[temp_row_1][temp_column_1] = 0;
					}
				}	
			}
		}
    } 
    
    //If I happen to screw up and unable to get every digits in the picture, I'll arrange them into a nice order rather than leave a empty space.
    for(counter = 0; counter < 4; ++counter){
    	if(entity_order1[counter] < 0){
    	   if(counter < 3){
	           if(entity_order1[counter + 1] >= 0 && counter < 3){
	           	  entity_order1[counter] = entity_order1[counter + 1];
	           	  entity_order1[counter + 1] = -2;
	           	}
	        }
    	}
    }

    //This is the 'pixels' that would be output to number recognition
	for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
		for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
			final_image[temp_row_1][temp_column_1] = pixels[temp_row_1][temp_column_1];
		}
	}
    
    //This is for figuring out how many digits do I have.
    entity_counter3 = 0;
    for(counter = 0; counter < 4; ++counter){
    	if(entity_order1[counter] >= 0 && entity_order1[counter] >= 0){
           entity_counter3 += 1;
    	}
    }

    entity_amount[0] = entity_counter3;
   
    /*-----------------------------------------------------------------------------------------------------------------------------*/

    //CALCUALTE PARAMETERS
    
    //Initilaize again to be sure
    for(counter = 0; counter < 14400 / 4; ++counter){
		temp_start_row[counter] = -1;
    	temp_start_column[counter] = width;
	}

    entity_counter1 = 0;
    while(entity_counter1 < entity_counter3){

		//Get the smallest roll of the entity.
		for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
			for(temp_column_1 = 0; temp_column_1 < width - 1; ++temp_column_1){
			    if(pixels[temp_row_1][temp_column_1] == 1){
			    	temp_start_row[entity_counter1] = temp_row_1;
			    	break;
			    }
			}
			if(pixels[temp_row_1][temp_column_1] == 1){
				break;
			}
		}      
        
        //Arrange the attributes into their proper 'in picture' order
        entity_order2[entity_counter1] = entity_coordination[temp_row_1][temp_column_1];

        //Find the right index
		for(counter = 0; counter < 4; ++counter){
			if(entity_order1[counter] == entity_order2[entity_counter1]){
				break;
			}
        }
        entity_counter2 = counter;
        start_row[entity_counter2] = temp_start_row[entity_counter1];

        //Clear all other entities temporarily
		for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
			for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
				if(entity_coordination[temp_row_1][temp_column_1] != entity_order2[entity_counter1] || hole_coordination[temp_row_1][temp_column_1] != -1){
				   pixels[temp_row_1][temp_column_1] = 0;
				}
			}
        }

        //Get the smallest, largest column and the maximum row of the entity.
        max_column = 0;
        max_row = 0;
        box_area[entity_counter2] = 0;
		for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
			for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
				if(final_image[temp_row_1][temp_column_1] == 1 || final_image[temp_row_1][temp_column_1] == 2){
					box_area[entity_counter2] += 1;
			    }
			    if(pixels[temp_row_1][temp_column_1] == 1){
			    	if(temp_start_column[entity_counter1] >= temp_column_1){
			    		temp_start_column[entity_counter1] = temp_column_1;
			    	}
			    	if(max_column <= temp_column_1){
			    		max_column = temp_column_1;
			    	}
			    	if(max_row <= temp_row_1){
				        max_row = temp_row_1;
				    }
			    }
			}
		}
		
		start_column[entity_counter2] = temp_start_column[entity_counter1];

        //Calculate entity parameters
		box_height[entity_counter2] = max_row - start_row[entity_counter2] + 1;
		box_width[entity_counter2] = max_column - start_column[entity_counter2] + 1;

        //Refill for next calculation, but leave out the ones that are already done.
        for(temp_row_1 = 0; temp_row_1 < height - 1; ++temp_row_1){
			for(temp_column_1 = 0; temp_column_1 < width; ++temp_column_1){
				if(entity_coordination[temp_row_1][temp_column_1] == entity_order2[0] && hole_coordination[temp_row_1][temp_column_1] == -1){
				   pixels[temp_row_1][temp_column_1] = 0;
				}
				else if(entity_coordination[temp_row_1][temp_column_1] == entity_order2[1] && hole_coordination[temp_row_1][temp_column_1] == -1){
				   pixels[temp_row_1][temp_column_1] = 0;
				}
				else if(entity_coordination[temp_row_1][temp_column_1] == entity_order2[2] && hole_coordination[temp_row_1][temp_column_1] == -1){
				   pixels[temp_row_1][temp_column_1] = 0;
				}
				else if(entity_coordination[temp_row_1][temp_column_1] == entity_order2[3] && hole_coordination[temp_row_1][temp_column_1] == -1){
				   pixels[temp_row_1][temp_column_1] = 0;
				}
				else if(entity_coordination[temp_row_1][temp_column_1] != -1 && hole_coordination[temp_row_1][temp_column_1] == -1){
				   pixels[temp_row_1][temp_column_1] = 1;
				}
			}
        }
        entity_counter1 += 1;
    }

}
    

    
	
 

    










