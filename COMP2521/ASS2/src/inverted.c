// Author: z5054637 Victor Wang
// Title: inverted.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define ENTRY_SIZE 603
#define WORD_SIZE 50

typedef struct Key{ // Key is a single word 
	char* key; // This is the word itself
	int url_num; // How many urls it has
	char** url; // What are these urls
} Key;

typedef struct Dic{ // This is the dictionary containing all the keys
	int key_num; // How many keys do we have?
	struct Key **key; // What are the keys?
} Dic;

// This is a binary search for dictionary
int dicBS(char* c, Dic* d, int low, int high){
	if(high < low){
		return -1;
	}
	int mid = (low + high) / 2;
	if(strcmp(d->key[mid]->key,c) < 0){
		return dicBS(c, d, mid+1, high);
	}
	else if(strcmp(d->key[mid]->key,c) > 0){
		return dicBS(c, d, low, mid-1);
	}
	else if(strcmp(d->key[mid]->key,c) == 0){
		return mid;
	}
	return -1;
}

// A cmp for qsort to compare dictionary key
int dicCmp(const void* a, const void* b){
	Key *x = *(Key **)a;
	Key *y = *(Key **)b;
	return strcmp(x->key, y->key);
}

// A cmp for qsort to compare string
int charCmp(const void* a, const void* b){
	char *x = *(char **)a;
	char *y = *(char **)b;
	return strcmp(x, y);
}

// Create a new key
Key *newKey(char* key){
	Key*new = malloc(sizeof(Key));
	new->key = malloc(strlen(key)+1);
	strcpy(&new->key[0], &key[0]);
	new->url_num = 0;
	new->url = malloc(1*WORD_SIZE*sizeof(char));
	new->url[0] = NULL;
	return new;
}

// Create a new dictionary
Dic *newDic(){
	Dic*new = malloc(sizeof(Dic));
	new->key = malloc(sizeof(Key));
	new->key_num = 0;
	return new;
}

// Add a new url to the key
void updateKey(Key* key, char* ch){
	char*temp = malloc(strlen(ch)+1);
	strcpy(&(temp)[0], &ch[0]);
	key->url_num += 1;
	key->url = realloc(key->url, (key->url_num)*WORD_SIZE*sizeof(char));
	key->url[key->url_num-1] = temp;
	key->url[key->url_num] = NULL;
}

// Add a new key to the dictionary
void updateDic(Dic* dic, Key* key){
	dic->key_num += 1;
	dic->key = realloc(dic->key, dic->key_num*sizeof(Key));
	dic->key[dic->key_num - 1] = key;
	dic->key[dic->key_num] = NULL;	
}

// Free the dictonary
void freeDic(Dic *dic){
	int i = 0;
	for(i = 0; dic->key[i] != NULL; ++i){
	   free(dic->key[i]->url);
		free(dic->key[i]);
	}	
	free(dic);
}

// Normalize a word to be without punctuaton and in lower case.
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

int main(void){
	FILE*file;
	file = fopen("collection.txt","r");
	char buffer[WORD_SIZE];
	Dic*dic = newDic();
	int section_flag = 0;
	char string[WORD_SIZE];	
	// Scan through the collection for url files
	while(fscanf(file, "%s", buffer) != EOF){		
		char txt[5] = ".txt";
		char*dummy;
		dummy = malloc(strlen(buffer)+1);
		strcpy(&dummy[0],&buffer[0]);
		FILE*url;	
		url = fopen(strcat(&dummy[0],&txt[0]),"r");
		section_flag = 0;
		while(fscanf(url, "%s", string) != EOF){
			if(strcmp(string, "Section-1") == 0 || strcmp(string, "Section-2") == 0){				
				section_flag += 1;
				continue;
			}							
			// Once reached section 2, all the words in the section 2 are added to the vertice
			// The reason why I have a lot of pointer array is to eliminate linear search by using qsort.
			// With qsort, I don't have to eliminate any duplicates. And binary searching make everything easier.
			// This same principle applied to all my files
			if(section_flag >= 3 && section_flag < 4 && strcmp(string, "#end") != 0){
				qsort(dic->key, dic->key_num, sizeof(Key*), dicCmp);				
				if(dic->key_num == 0){
					normalize(string);
					Key* new = newKey(&string[0]);
					updateDic(dic, new);
					updateKey(new, &buffer[0]);			
				}
				else{
					normalize(string);
					int k = -1;
					// Look for the new string
					k = dicBS(&string[0], dic, 0, (dic->key_num - 1));
					// If the key is found, return the index	
					if(k != -1){
						updateKey(dic->key[k], &buffer[0]);
					}
					else{
						Key* new = newKey(string);
						updateDic(dic, new);
						updateKey(new, &buffer[0]);
					}
				}			
			}
		}
		fclose(url);	
	}
	// Sort the keys to be in alphabatic order
	qsort(dic->key, dic->key_num, sizeof(Key*), dicCmp);
	FILE*output;
	output = fopen("invertedIndex.txt", "w"); 
	for(int m = 0; dic->key[m] != NULL; m++){
		fprintf(output, "%s  ", dic->key[m]->key);
		// This is made to avoid printing duplicates
		// And I thought this is a easy way to avoid have to 
		// delete and checking for all the duplicates with each insert.
		char* prev = "-1";
		if(dic->key[m]->url_num != 0){
			qsort(dic->key[m]->url, dic->key[m]->url_num, sizeof(char*), charCmp);
			for(int z = 0; z < dic->key[m]->url_num; z++){
				if(strcmp(dic->key[m]->url[z], prev) != 0){				
					fprintf(output, "%s ",dic->key[m]->url[z]);
				}
				prev = dic->key[m]->url[z];
			}
			fprintf(output, "\n");
		}
	}
	fclose(file);
	freeDic(dic);
	return EXIT_SUCCESS;
}