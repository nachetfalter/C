// Author: z5054637 Victor Wang
// Header for graph.c

// The entry size is the size of url names
#define ENTRY_SIZE 603

// The word size is the size of each word.
#define WORD_SIZE 50

typedef struct Vertice{
   char* ID; // The url ID
   int nI; // Number of edges that comes in
   int nO; // Number of edges that comes out
   double pr; // The page rank weight
   struct Vertice **out; // The vertices this vertice links to, with no ordering
   struct Vertice **in; // The vertices this vertice links to, with no ordering
} Vertice;

typedef struct ADL{ // This is the adjacency list.
	int    nV; // This is the number of vertices in the ADL
	struct Vertice  **entry; // This is the entry list of all vertices in the ADL
} ADL;

void newGraph(ADL* adRep);
void updatePr(Vertice* v, double pr);
void printGraph(ADL* adRep);
void freeADL(ADL* adRep);
ADL *newADL();
