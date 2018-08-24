#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
	char array[20][20] = {{"-1"}};
	char*a = "miao";

	strcpy(array[0],a);

	printf("%s\n",array[0]);

	return 0;
}