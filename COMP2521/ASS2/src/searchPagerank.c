// Author: z5054637 Victor Wang
// Title: searchPagerank.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define ENTRY_SIZE 603
#define WORD_SIZE 50

typedef struct Page{ // Page is an URL
	char* name; // Name of the URL
	int priority; // The priority is the amount of target words contained in this page
	double pr; // This is the page rank
} Page;

typedef struct Set{ // The set contains all pages that we found with target words
	int size;
	struct Page **page;
} Set;

// This is a cmp function used for qsort
// this function sort strings in ascending order.
// DO NOT USE THIS FUNCTION ON ITS OWN
int charCmp(const void* a, const void* b){
	char *x = *(char **)a;
	char *y = *(char **)b;
	return strcmp(x, y);
}

// This is a cmp function used for qsort
// this function sort page in descending order.
// This is done by combine the two major comparison critiria
// The requirement is:
// first order the pages by the number of terms appeared in them
// then within each group with the same number of terms
// the pages are then sorted again by the page rank of each page.
// So what happens here is
// (number of terms appeared in a page) * 100 * (1 + page rank of the page)
// In this way the priority (number of terms) have absolute priority over the page rank
// Thus make it sorted first
// then within the same ranks, small difference is done by * (1 + page rank of the page)
// DO NOT USE THIS FUNCTION ON ITS OWN
int pageCmp(const void* a, const void* b){
	Page *x = *(Page **)a;
	Page *y = *(Page **)b;
	if(x->priority * 100 * (1 + x->pr) > y->priority* 100 * (1 + y->pr)){
		return -1;
	}
	else if(x->priority * 100 * (1 + x->pr) < y->priority* 100 * (1 + y->pr)){
		return 1;
	}
	else{
		return 0;
	}
}

// This is a cmp function used for qsort
// this function sort name of page in ascending order.
// DO NOT USE THIS FUNCTION ON ITS OWN
int pageNameCmp(const void* a, const void* b){
	Page *x = *(Page **)a;
	Page *y = *(Page **)b;
	return strcmp(x->name, y->name);
}

// This BS is used to search a term in the argv
// c = the term you need to look for
// argv = the argv where you conduct the search
// low = 0, high = the size of the pool - 1
// if the term is found, return the index of the term
// else it returns -1
// BS MUST BE SORTED IN ASCENDING ORDER BEFORE BEING USED
int argvBS(char* c, char* argv[], int low, int high){
	if(high < low){
		return -1;
	}
	int mid = (low + high) / 2;
	if(strcmp(argv[mid],c) < 0){
		return argvBS(c, argv, mid+1, high);
	}
	else if(strcmp(argv[mid],c) > 0){
		return argvBS(c, argv, low, mid-1);
	}
	else if(strcmp(argv[mid],c) == 0){
		return mid;
	}
	return -1;
}

// This BS is used to search a term in the set
// c = the name of the page you need to look for
// s = the set where you will conduct the search
// low = 0, high = the amount of the pages in the set - 1
// if the term is found, return the index of the term
// else it returns -1
// BS MUST BE SORTED IN ASCENDING ORDER BEFORE BEING USED
int pageBS(char* c, Set* s, int low, int high){
	if(high < low){
		return -1;
	}
	int mid = (low + high) / 2;
	if(strcmp(s->page[mid]->name, c) < 0){
		return pageBS(c, s, mid+1, high);
	}
	else if(strcmp(s->page[mid]->name, c) > 0){
		return pageBS(c, s, low, mid-1);
	}
	else if(strcmp(s->page[mid]->name, c) == 0){
		return mid;
	}
	return -1;
}

// This is the function for creating a new page, the argument is the name of the page
Page *newPage(char* page){
	Page* new = malloc(sizeof(Page));
	new->name = malloc(strlen(page)+1);
	strcpy(&new->name[0], &page[0]);
	new->priority = 1;
	new->pr = 0;
	return new;
}

// This is the function for creating a new set
Set *newSet(){
	Set* new = malloc(sizeof(Set));
	new->size = 0;
	new->page = malloc(1*sizeof(Page));
	new->page[0] = NULL;
	return new;
}

// Update the set, insert/update the page.
// it takes two argument, the s = set and the p = page
// if the target page is found in the set, then the page info is updated
// if the target page is not found, add it to the set. 
void updateSet(Set* s, Page* p){
	int flag = -1;
	flag = pageBS(p->name, s, 0, (s->size - 1));				
	if(flag != -1){
		s->page[flag]->priority += 1;
	}
	else{
		s->size += 1;
		s->page = realloc(s->page, s->size*sizeof(Page));
		s->page[s->size - 1] = p;
		s->page[s->size] = NULL;
	}	
}

// Normalization function just like the one from inverted
void normalize(char* word){
	if(word[strlen(word) - 1] == '.' || word[strlen(word) - 1] == '?' || word[strlen(word) - 1] == ','){
		word[strlen(word) - 1] = '\0';
	}
	int j = 0;
	char string_buffer[WORD_SIZE]; 
	while(word[j]){
		string_buffer[j] = tolower(word[j]);
		++j;
	}
	string_buffer[j] = '\0';
	strcpy(word, string_buffer);
}


// Free the set
void freeSet(Set* s){
	for(int i = 0; i < s->size; ++i){
		free(s->page[i]);
	}
	free(s);
}

int main(int argc, char* argv[]){
	// Convert all the words into lower case without any punctuations.
	for(int i = 1; i < argc; ++i){
		normalize(argv[i]);
	}
	// Set up the set we are going to use
	Set* s = newSet();
	// Part one: (search and insert all relevent pages)
	FILE*file;
	// Open the inverted index txt first
	file = fopen("invertedIndex.txt","r");
	char string[WORD_SIZE];
	// This is done to avoid there are words 
	// which is the name of the function in the command line
	strcpy(argv[0], " ");
	// Sort the command line in ascending alphabetical order
	qsort(argv, argc, sizeof(char*), charCmp);
	int flag = -1;
	int continue_flag = 0;
	while(fscanf(file, "%s", string) != EOF){
		// Every word is loaded into the string buffer.	
		flag = -1;	
		// Try to search the word in the txt file in the command line.
		flag = argvBS(&string[0], argv, 0, argc - 1);
		// If the current word in buffer is found in the command line
		// go wih the following operations, else nothing happens
		// This is because the arbitrary nature of the page amount following the key word.
		if(flag != -1){
			// We find the word in the commandline, so we know the words that follows it are the
			// urls where this word is contained
			continue_flag = 1;
			// skip to the next loop so the flag doesn't take effect immediately.
			continue;
		}
		// We know now we are in between the key word and the end of the line
		if(continue_flag == 1){
			// As we know, after the key word, there are several urls
			// and the structure of urlxxx is always url+number
			// until we reach the new line ( the new word), 
			// the words would always be urlxxx, the xxx part after the third char of the word is always a number
			// so if the char after the third letter, which is the first digit of xxx, becomes a none number
			// which means it has to be over 9 or under 0
			// we know we have reached the new line and new word key, thus we turn off the flag, so we are
			// back to the state where we haven't yet found the key word.
			if(string[3] > '9' || string[3] < '0'){
				continue_flag = 0;
				continue;
			}
			// You have to sort the pages with every iteration to make the BS search working properly
			qsort(s->page, s->size, sizeof(Page*), pageNameCmp);
			// So if none of the above situation happens, we make a new page out of the urlx, and try to update
			// it in the set.
			Page* p = newPage(&string[0]);
			updateSet(s, p);		
		}			
	}
	fclose(file);
	// At this point the set should contains all the pages where any of the keywords ever appears in.
	// if the size of the set is still 0 at this point, it means there aren't any matching results.
	// thus if that happens, we terminate.
	if(s->size == 0) return EXIT_FAILURE;
	// Part two: inserting page ranks into the pages we found in part one.
	// remember we need both the page priority and the page rank for sorting.
	// thus it is crucial we obtain both.
	// so now we will be reading from the page rank file.
	file = fopen("pagerankList.txt","r");
	// Sort the pages by their names to ensure the BS works
	qsort(s->page, s->size, sizeof(Page*), pageNameCmp);
	continue_flag = 1;
	int marker = -1;
	while(fscanf(file, "%s", string) != EOF){
		// This here is to eliminate the comma after each entry by move the NULL pointer forward 1
		string[strlen(string) - 1] = '\0';
		flag = -1;
		// If we are in the first word on the line, which is the url
		if(continue_flag == 1){
			// Trye to look for the page in the set.
			flag = pageBS(&string[0], s, 0, (s->size - 1));
			// If the page is found, we change marker to the index of the page
			// which means marker will no longer be -1; (the reason for not picking 0 is because
			// there can be index 0)
			if(flag != -1){
				marker = flag;
			}
		}
		// We are not concerned about the actual out number of the page, which is the second component
		// of a line in pageranklist.txt, we need the third, which is the page rank weight
		if(continue_flag == 3){	
			// So here, if the pagename of the same line is found before, we know
			// this page rank is the page rank of the url we found, so we get the page by
			// using the marker, which is now the index of the page we found
			// then change its pr weight.
			if(marker != -1){
				// All this is done to make the string to double transition work properly.
				char*buffer = malloc(strlen(string) + 1);
				strcpy(buffer, string);
				buffer[strlen(string)] = '\0';
				s->page[marker]->pr = atof(buffer);
			}
			// once we are finished with the above operation, (found or not)
			// we know it's time for the next line, so we reset the continue flag and the marker.
			continue_flag = 0;
			marker = -1;
		}
		// the flag is used as a symbol of advancemen in the same line
		// when on the first word it is 1, then it is 2, then it is 3, then it is reset
		// back to 0 and get incremented into 1 with the next entry again.
		continue_flag++;
	}
	fclose(file);
	// We do the final sorting based on the complex requirement, this qsort use my page cmp
	// which sort the urls based on complex requirement.
	qsort(s->page, s->size, sizeof(Page*), pageCmp);
	// Then we output the urls we find into stdout.
	// we print all pages we find in order, but if there are over 30, we only output 30 of them

	for(int k = 0; k < s->size; k++){
		printf("%s\n",s->page[k]->name);
		if(k >= 30) break;
	}
	freeSet(s);
	return EXIT_SUCCESS;
}