// Author: z5054637 Victor Wang
// Title: scaledFootrule.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

// Maximum length of a url name
#define ENTRY_SIZE 603

typedef struct List{
	int size; // The size of the list
	struct File** file; // An array of file structs
} List;

typedef struct File{
	int size; // how many urls this file have
	char** page; // An array of url names
} File;

// This is a compare function for qsort to compare the names of different pages.
int pageCmp(const void* a, const void* b){
	char *x = *(char **)a;
	char *y = *(char **)b;
	return strcmp(x, y);
}

// This function is a binary search for searching for special File struct.
int commonBS(char* c, File* cl, int low, int high){
	if(high < low){
		return -1;
	}
	int mid = (low + high) / 2;
	if(strcmp(cl->page[mid], c) < 0){
		return commonBS(c, cl, mid+1, high);
	}
	else if(strcmp(cl->page[mid], c) > 0){
		return commonBS(c, cl, low, mid-1);
	}
	else if(strcmp(cl->page[mid], c) == 0){
		return mid;
	}
	return -1;
}

// ..No fancy tricks here... 
// This is a simple linear search for files.
// As the original list cannot be ordered due to their order being important later...
int fileLS(char* c, File* f){
	int i = 0;
	for(i = 0; i < f->size; i++){
		if(strcmp(f->page[i], c) == 0){
			return i;
		}
	}
	return -1;
}

// Create a new list which contains all files in the argv.
List *newList(){
	List* new = malloc(sizeof(List));
	new->size = 0;
	new->file = malloc(1*sizeof(File));
	new->file[0] = NULL;
	return new;
}

// Create a file which contains all entries of URLs
File *newFile(){
	File* new = malloc(sizeof(File));
	new->size = 0;
	new->page = malloc(1*ENTRY_SIZE*sizeof(char));
	new->page[0] = NULL;
	return new;
}

// Update a list, add new file to the list.
void updateList(List* l, File* f){
	l->size += 1;
	l->file = realloc(l->file, l->size*sizeof(File));
	l->file[l->size - 1] = f;
	l->file[l->size] = NULL;
}

// This is a special way of updatng a file struct object, qsort is needed to make BS working.
// Common is the union of all URL.
void updateCommon(File* c, char* url){
	qsort(c->page, c->size, sizeof(char*), pageCmp);		
	char* buffer = malloc(strlen(url)+1);
	// The reason why I strcpy all the time is to avoid values changes with pointers.
	strcpy(buffer, url);
	// If there is not yet one
	if(c->size < 1){
		c->size += 1;
		c->page[c->size - 1] = buffer;
		c->page[c->size] = NULL;
	}
	// For the opposite
	else if(commonBS(buffer, c, 0, (c->size - 1)) == -1){
		c->size += 1;
		c->page = realloc(c->page, c->size*ENTRY_SIZE*sizeof(char));
		c->page[c->size - 1] = buffer;
		c->page[c->size] = NULL;
	}
}

// Add an URL entry to a file object
void updateFile(File* f, char* url){
	f->size += 1;
	f->page = realloc(f->page, f->size*ENTRY_SIZE*sizeof(char));
	char* buffer = malloc(strlen(url)+1);
	strcpy(buffer, url);
	f->page[f->size - 1] = buffer;
	f->page[f->size] = NULL;
}

// Free the list
void freeList(List* l){
	for(int i = 0; i < l->size; ++i){
		free(l->file[i]);
	}
	free(l);
}

// I did a modified Dijkstra algo for this before, however it didn't manage to work out
// My original plan is to use horizontal sum of a same url as the weight, thus this function
// is called weight.
// It still does the same thing now as before
double weight(int n, char* page, List* l, int p){
	double w = 0;
	// For each file in l
	for(int i = 0; i < l->size; ++i){
		int index = -1;
		// For each page in the file i
		// Find the current location of the page from common in this file, which is Tf(x)
		index = fileLS(page, l->file[i]);
		// If the page is found
		if(index != -1){
			// +1 for the index starts from 1
			double f1 = index + 1;
			// Not +1 here for already plused
			double f2 = p;
			// w should be the sum of this page in all files at this position
			w += fabs((f1 / (l->file[i]->size)) - f2 / n);
		}
	}	
	return w;
}

// A minor function used by brute permunate
void swap(int* array, int a, int b){
	int temp = array[a];
	array[a] = array[b];
	array[b] = temp;
}

// I have no way to keep this number without using a global :(
double final = 10000;
// The lowest of lowest denominator, however it's my only resort on the night before due 
// after I failed the former algo :(
void brute(File*c, List* l, int* array, int* final_order, int start, int n){
	double temp = 0;
	// when we try to swap n with n, this means we have 
	// reached the end case of this branch wher everything is fixed
	if(start == n){	
		// This is the lame part where you calculate the W(c,p)
		temp = 0;
		for(int j = 0; j < c->size; j++){		
			temp += weight(c->size, c->page[j], l, array[j]);	
		}
		// If smaller, substitute
		if(temp < final){
			final = temp;
			for(int k = 0; k < c->size; ++k){
				final_order[array[k] - 1] = k;
			}
		}	
	}
	else{
		for(int i = 0; i < n; i++){
			// swap a with a
			swap(array, start, i);
			// swap b with b of the next level
			brute(c, l, array, final_order, start+1, c->size);
			// restore the last level of swap
			swap(array, start, i);
		}
	}
}

// Here is our main course
int main(int argc, char* argv[]){
	List* l = newList();
	File* c = newFile();
	FILE* file;
	// Open the file in the argv one by one.
	for(int i = 1; i < argc; ++i){
		if(fopen(argv[i],"r") != NULL){
			file = fopen(argv[i],"r");
			char string[ENTRY_SIZE];
			File* new = newFile();
			while(fscanf(file, "%s", string) != EOF){
				updateFile(new, &string[0]);
				updateCommon(c, &string[0]);
			}
			updateList(l, new);
		}
	}
	fclose(file);	
	// If at this point c is still 0, then there is something wrong
	// terminate	
	if(c->size == 0) return EXIT_FAILURE;
	// Array is used for sorting
	int array[c->size]; 
	// The final order is the final sequence
	int final_order[c->size];
	for(int j = 1; j <= c->size; j++){
		array[j - 1] = j;
	}
	// Involke the brute calculation which change the value of final.
	brute(c, l, &array[0], &final_order[0], 0, c->size);
	// Output everything
	printf("%f\n", final);
	for(int j = 1; j <= c->size; j++){
		printf("%s\n", c->page[final_order[j - 1]]);
	}
	freeList(l);
	return EXIT_SUCCESS;
}