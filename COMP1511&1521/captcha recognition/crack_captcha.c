#include <stdio.h>
#include <stdlib.h>
#include "captcha.h"

int main(int argc, char *argv[]) {

    //Premade scanning parts from the "Test Horizontal Balance.c" from week 7 lab
    //The only thing I used that is not my own is the formula which calculate the roundness of a object, I got it from a official mathlab tutorial in its written form(not code):
    //https://au.mathworks.com/help/images/examples/identifying-round-objects.html?s_tid=answers_rc2-2_p5_Topic
    int height, width;
   
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image-file>\n", argv[0]);
        return 1;
    }

    if (get_pbm_dimensions(argv[1], &height, &width) != 1) {
        return 1;
    }
    
    //Attributes:
    int pixels[height][width];
    int start_row[4], start_column[4], final_image1[height][width], final_image2[height][width], box_area[4], final_answer[4];
    double box_width[4], box_height[4];
    int entity_amount = 0;

    //Utility:
    int temp_column1 = 0, temp_row1 = 0;
    int counter = 0;
    
    //Initialize 
    for(temp_row1 = 0; temp_row1 < height; ++temp_row1){
        for(temp_column1 = 0; temp_column1 < width; ++temp_column1){
            final_image1[temp_row1][temp_column1] = -1;
            final_image2[temp_row1][temp_column1] = -1;
        }
        if(temp_row1 < 4){
            start_row[temp_row1] = -1;
            start_column[temp_row1] = -1;
            box_width[temp_row1] = -1;
            box_height[temp_row1] = -1;
            box_area[temp_row1] = -1;
            final_answer[temp_row1] = -1; 
        }
    }
    
    //Once succesfully scaned, move on with operations
    if(read_pbm(argv[1], height, width, pixels)){
        //Capture the number and their parameters and attributes.
        capture(height, width, pixels, start_row, start_column, box_height, box_width, box_area, final_image1, &entity_amount);   

        //Analyse and recognize the numbers and output the answer
        pattern_recognition(height, width, final_image1, start_row, start_column, box_height, box_width, box_area, entity_amount, final_answer);

        //Final output
        for(counter = 0; counter < 4; ++counter){
            if(final_answer[counter] >= 0 && final_answer[counter] <= 9){
                printf("%d",final_answer[counter]);
            }
        }
        printf("\n");
    }
    return 0;
}
