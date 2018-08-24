// Author: z5054637 Victor Wang
// Title: graph.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "graph.h"

// Create a new vertices with the given ID
Vertice *newVertice(char* ID){
	Vertice*new = malloc(sizeof(Vertice));
	new->ID = malloc(strlen(ID)+1);
	strcpy(&(new->ID)[0], &ID[0]);
	new->nI = 0;
	new->nO = 0;
	new->pr = 0.0;
	new->out = malloc(1*sizeof(Vertice));
	new->out[0] = NULL;
	new->in = malloc(1*sizeof(Vertice));
	new->in[0] = NULL;
	new->ID[strlen(ID)]='\0';
	return new;  
}

// Create a new ADL
ADL *newADL(){
	ADL* new = malloc(sizeof(ADL));
	new->nV = 0;
	new->entry = malloc(1*sizeof(Vertice));
	new->entry[0] = NULL;
	return new;
}

// This is for modifing vertices, change their IO
void updateConn(Vertice* start, Vertice* finish){
	// Some urls just goes no where :(
	if(start != NULL && finish != NULL){
		start->nO += 1;
		finish->nI += 1;
		finish->in = realloc(finish->in, finish->nI*sizeof(Vertice));
		finish->in[finish->nI-1] = start;
		finish->in[finish->nI] = NULL;
		start->out = realloc(start->out, start->nO*sizeof(Vertice));
		start->out[start->nO-1] = finish;
		start->out[start->nO] = NULL;
	}
}

// Update the ADL by adding new Vertice
void updateADL(ADL *adRep, Vertice* new){
	adRep->nV += 1;
	adRep->entry = realloc(adRep->entry, adRep->nV*sizeof(Vertice));
	adRep->entry[adRep->nV-1] = new;
	adRep->entry[adRep->nV] = NULL;
}

// This function is made specifically to be called in 
// page rank to avoid memory issue
void updatePr(Vertice* v, double pr){
	v->pr = pr;
}

// Free the ADL
void freeADL(ADL *adRep){
	for (int i = 0; adRep->entry[i] != NULL; i++){
	   free(adRep->entry[i]);
	}
	free(adRep);
}

// Graph creation
void newGraph(ADL* adRep){
	FILE*file;
	file = fopen("collection.txt","r");
	char buffer[ENTRY_SIZE];	
	Vertice*entry;
	// Scan through the collection
	while(fscanf(file, "%s", buffer) != EOF){		
		// If an entry of URL is found
		// Create a new vertice (URL)
		entry = newVertice(buffer);
		// Add entry to the ADL
		updateADL(adRep, entry);
	}
	fclose(file);
	int section_flag = 0;	
	// Prepare to read content of each URL
	for(int i = 0; adRep->entry[i] != NULL; ++i){
		char txt[5] = ".txt";
		char*dummy;
		dummy = malloc(strlen(&adRep->entry[i]->ID[0])+1);
		strcpy(&dummy[0],&adRep->entry[i]->ID[0]);
		// Open each url file contained in the collection to read connections
		FILE*url;
		url = fopen(strcat(&dummy[0],&txt[0]),"r");
		// The section flag is used to detect where we are
		section_flag = 0;
		while(fscanf(url, "%s", buffer) != EOF){		
			if(strcmp(buffer, "Section-1") == 0 || strcmp(buffer, "Section-2") == 0){				
				section_flag += 1;
				continue;
			}
			// Once reached section 1, all the connected vertices to this entry is added to the LL
			if(section_flag >= 1 && section_flag < 2 && strcmp(buffer, "#end") != 0){
				int k = 0;
				for(k = 0; adRep->entry[k] != NULL; ++k){
					if(strcmp(buffer, adRep->entry[k]->ID) == 0 && strcmp(adRep->entry[k]->ID, adRep->entry[i]->ID) != 0){
						break;
					}
				}
				updateConn(adRep->entry[i], adRep->entry[k]);	
			}
		}
		fclose(url);
	}
}


