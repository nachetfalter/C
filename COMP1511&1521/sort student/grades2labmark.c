//Topic: Converting COMP1511 Lab Grade Into a Mark
//Author: Victor Wang
//Date: 20/05/2017
//Description: Converting COMP1511 Lab Grade Into a Mark

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
double grades2labmark(char grades[]);

void main(int argc, char*argv[]){
    printf("%.1lf\n",grades2labmark(argv[1]));
}

double grades2labmark(char grades[]){
	int index = 0; //Elements in the string
	double mark = 0;

	while(grades[index] != '\0'){
		if(grades[index + 1] == '+'){
            mark += 1.2;
		}
		else if(grades[index] == 'A'){
			mark += 1;
		}
		else if(grades[index] == 'B'){
			mark += 0.8;
		}
		else if(grades[index] == 'C'){
			mark += 0.5;
		}
		else if(grades[index] == '.'){
			mark += 0;
		}
		index += 1;
	}
    //Make sure no mark exceed the limit.
    if(mark > 10.0) mark = 10.0;
    return mark;
	
}