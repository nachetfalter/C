// Author: z5054637 Victor Wang
// Title: pagerank.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "graph.h"

// This is a cmp for qsort to sort the vertices based on their page rank
int urlCmp(const void* a, const void* b){
	Vertice *x = *(Vertice **)a;
    Vertice *y = *(Vertice **)b;
	if(x->pr > y->pr){
        return -1;
    } 
    else if(x->pr < y->pr){
        return 1;
    } 
    else{
        return 0;
    }
}

// This is the part where I calculate the W(in)
double wIn(Vertice*i, Vertice*j){
	double numerator = j->nI;
	double denominator = 0;
	int k = 0;
	while(i->out[k] != NULL){
		denominator += i->out[k]->nI;
		k++;
	}
	return numerator/denominator;
}

// This is the part where I calculate the W(out)
double wOut(Vertice*i, Vertice*j){
	double numerator = j->nO;
	double denominator = 0;
	int k = 0;
	while(i->out[k] != NULL){
		if(i->out[k]->nO == 0){
			denominator += 0.5;
		}
		else{
			denominator += i->out[k]->nO;
		}
		k++;
	}
	// Just in case the nO is 0.
	if(numerator == 0) numerator = 0.5;
	if(denominator == 0) denominator = 0.5;
	return numerator/denominator;
}

int main(int argc, char* argv[]){
	double d = atof(argv[1]);
	double diffPR = atof(argv[2]);
	int maxIterations = atoi(argv[3]);
	// Establish the graph first.
	ADL*adRep = newADL();
	newGraph(adRep);
	double n = adRep->nV;

	// The initial page rank.
	int i = 0;
	for(i = 0; adRep->entry[i] != NULL; ++i){
		updatePr(adRep->entry[i], 1/n);
	}

	int iteration = 0;
	double diff = diffPR;
	double prev = 0;
	double next = 0;
	while(iteration < maxIterations && diff >= diffPR){	
		iteration++;
		for(i = 0; adRep->entry[i] != NULL; ++i){
			prev = adRep->entry[i]->pr;
			int k = 0;
			double equition_second_part = 0;
			while(adRep->entry[i]->in[k] != NULL){
				equition_second_part += (adRep->entry[i]->in[k]->pr)*wIn(adRep->entry[i]->in[k], adRep->entry[i])*
										wOut(adRep->entry[i]->in[k], adRep->entry[i]);
				k++;
			}
			updatePr(adRep->entry[i], ((1-d)/n) + d*equition_second_part);
			next = adRep->entry[i]->pr;
			diff += abs(next - prev);
			prev = next;
		}
	}
	// Sort the result based on the page rank weight in descending order.
	qsort(adRep->entry, adRep->nV, sizeof(Vertice*), urlCmp);
	// Out put the result.
	FILE*output;
	output = fopen("pagerankList.txt", "w"); 
	for(i = 0;  i < adRep->nV; ++i){
		fprintf(output, "%s, %d, %.7f\n", adRep->entry[i]->ID,adRep->entry[i]->nO,adRep->entry[i]->pr);
	}
	freeADL(adRep);
	return EXIT_SUCCESS;
}