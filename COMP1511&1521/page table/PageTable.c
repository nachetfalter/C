// PageTable.c ... implementation of Page Table operations
// COMP1521 17s2 Assignment 2
// Written by John Shepherd, September 2017

#include <stdlib.h>
#include <stdio.h>
#include "Memory.h"
#include "Stats.h"
#include "PageTable.h"

// Symbolic constants

#define NOT_USED 0
#define IN_MEMORY 1
#define ON_DISK 2

// PTE = Page Table Entry

typedef struct {
   char status;      // NOT_USED, IN_MEMORY, ON_DISK
   char modified;    // boolean: changed since loaded
   int  frame;       // memory frame holding this page
   int  accessTime;  // clock tick for last access
   int  loadTime;    // clock tick for last time loaded
   int  nPeeks;      // total number times this page read
   int  nPokes;      // total number times this page modified
   // TODO: add more fields here, if needed ...
   int  np;          // number of pages
   
} PTE;

// Define the DLL node
typedef struct
{
	int page;
	Node *prev;
	Node *next;
} Node;

// Define the queue
typedef struct
{
	Node *head;
	Node *tail;
} Queue;

// Define the hash table
typedef struct
{
	int size;
   Node* *table;
} Hash;

// Malloc and initialize a new node
Node *newNode(int page)
{
   Node *new = malloc(sizeof(Node));
   new->page = page;
   new->prev = new->next = NULL;
   return new;
}

// Make a new queue
Queue newQueue()
{
	Queue new = malloc(sizeof(Queue));
	new->head = NULL;
	new->tail = NULL;
	return new;
}

// Make a new hash table
Hash newHash(int size)
{
   Hash new = malloc(sizeof(Hash));
   new->size = size; 
   new->table = malloc(size*sizeof(Node*));
   for(int i = 0; i < size; ++i){
      new->table[i] = NULL;
   }
   return new;
}

// free the queue
void freequeue(Queue q)
{
	Node *curr, *next;
	curr = q->head;
	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	free(q);
}

// enqueue
int enqueue(Queue q, int page)
{
	if (queueFull(q))
   {
      page = dequeue(q);
   }
	Node *new = newNode(page);
	if (q->head == NULL) q->head = new;		
	if (q->tail != NULL) q->tail->next = new;	
	q->tail = new;
   hash->table[page] = new;
   return page;
}

// dequeue
int dequeue(Queue q, Hash h)
{
	if (queueEmpty(q)) return -1;
	int page = q->head->page;
	Node *target = q->head;
	q->head = target->next;
	if (q->head == NULL) q->tail = NULL;
	free(target);
	q->curr -= 1;
	h->table[q->head->page] = NULL;
	return page;
}

// see if the queue is empty
int queueEmpty(Queue q)
{
	if (q->head == NULL) 
	{
		return 1;
	}
	return 0; 
}

// see if the queue is full
int queueFull(Queue q)
{
	if (q->curr == q->max) 
	{
		return 1;
	}
	return 0; 
}

// The virtual address space of the process is managed
//  by an array of Page Table Entries (PTEs)
// The Page Table is not directly accessible outside
//  this file (hence the static declaration)

static PTE *PageTable;      // array of page table entries
static int  nPages;         // # entries in page table
static int  replacePolicy;  // how to do page replacement
static int  fifoList;       // index of first PTE in FIFO list
static int  fifoLast;       // index of last PTE in FIFO list
// Forward refs for private functions

static int findVictim(int);

// initPageTable: create/initialise Page Table data structures

void initPageTable(int policy, int np)
{
   PageTable = malloc(np * sizeof(PTE));
   if (PageTable == NULL) {
      fprintf(stderr, "Can't initialise Memory\n");
      exit(EXIT_FAILURE);
   }
   replacePolicy = policy;
   nPages = np;
   fifoList = 0;
   fifoLast = nPages-1;
   for (int i = 0; i < nPages; i++) {
      PTE *p = &PageTable[i];
      p->status = NOT_USED;
      p->modified = 0;
      p->frame = NONE;
      p->accessTime = NONE;
      p->loadTime = NONE;
      p->nPeeks = p->nPokes = 0;
      if (i == 0)
      {
   		p->np = np;
      } 
   }
}

// Declare the global data structure
static Hash h = newHash(&PageTable[0]->np);
static Queue q = newQueue(&PageTable[0]->np);


// requestPage: request access to page pno in mode
// returns memory frame holding this page
// page may have to be loaded
// PTE(status,modified,frame,accessTime,nextPage,nPeeks,nWrites)

int requestPage(int pno, char mode, int time)
{
   if (pno < 0 || pno >= nPages) {
      fprintf(stderr,"Invalid page reference\n");
      exit(EXIT_FAILURE);
   }
   PTE *p = &PageTable[pno];
   int fno; // frame number
   switch (p->status) {
   case NOT_USED:
      p->status = IN_MEMORY;
      p->loadTime = time;
   case ON_DISK:
      // TODO: add stats collection
      countPageFault();
      fno = findFreeFrame();
      if (fno == NONE) {
         int vno = findVictim(pno);
#ifdef DBUG
         printf("Evict page %d\n",vno);
#endif
         // TODO:
         // if victim page modified, save its frame
         if (&PageTable[vno]->modified)
         {
            saveFrame(fno);
         }
         // collect frame# (fno) for victim page
            fno = &PageTable[vno]->frame;
         // update PTE for victim page
         // - new status
            &PageTable[vno]->status = ON_DISK;
         // - no longer modified
            &PageTable[vno]->modified = 0;
         // - no frame mapping
            &PageTable[vno]->frame = NONE;
         // - not accessed, not loaded
            &PageTable[vno]->accessTime = NONE;
            &PageTable[vno]->loadTime = NONE;
      }
      printf("Page %d given frame %d\n",pno,fno);
      // TODO:
      // load page pno into frame fno
      loadFrame(fno, pno, time);
      // update PTE for page
      // - new status
      p->status = IN_MEMORY;
      // - not yet modified
      p->modified = 0;
      // - associated with frame fno
      p->frame = fno;
      // - just loaded
      p->loadTime = time;
      break;
   case IN_MEMORY:
      // TODO: add stats collection
      countPageHit();
      break;
   default:
      fprintf(stderr,"Invalid page status\n");
      exit(EXIT_FAILURE);
   }
   if (mode == 'r')
      p->nPeeks++;
   else if (mode == 'w') {
      p->nPokes++;
      p->modified = 1;
   }
   p->accessTime = time;
   switch (replacePolicy) 
   {
   case REPL_FIFO:
   	findVictim(pno)
   	break;
	}
   return p->frame;
}

// findVictim: find a page to be replaced
// uses the configured replacement policy

static int findVictim(int pno)
{
   int victim = 0;
   switch (replacePolicy) 
   {
   case REPL_LRU:
		Node* target = h->table[pno];
	   // If the target page is not in the hash table
	   if (target == NULL)
	   {
	   	victim = enqueue(q, h, pno);
	   } 
	   // page is in the queue yet it's not at the end (newest)
	   // update node position!
	   else if (target != q->tail)
	   {
	   	// Isolate the node
	   	target->next->prev = target->prev;
	      if (target != q->head)
	      {
	      	target->prev->next = target->next;
	      } 
	 		else
	 		{
				q->head = target->next;
	         q->head->prev = NULL;
	 		}
	      target->prev = q->tail;
	      target->next = NULL;
	      target->prev->next = target;
	      // Finishing updating the node position
	      q->tail = target;
	   }
      break;
   case REPL_FIFO:
   	victim = enqueue(q, pno);
      break;
   case REPL_CLOCK:
      return 0;
   }
   return victim;
}

// showPageTableStatus: dump page table
// PTE(status,modified,frame,accessTime,nextPage,nPeeks,nWrites)

void showPageTableStatus(void)
{
   char *s;
   printf("%4s %6s %4s %6s %7s %7s %7s %7s\n",
          "Page","Status","Mod?","Frame","Acc(t)","Load(t)","#Peeks","#Pokes");
   for (int i = 0; i < nPages; i++) {
      PTE *p = &PageTable[i];
      printf("[%02d]", i);
      switch (p->status) {
      case NOT_USED:  s = "-"; break;
      case IN_MEMORY: s = "mem"; break;
      case ON_DISK:   s = "disk"; break;
      }
      printf(" %6s", s);
      printf(" %4s", p->modified ? "yes" : "no");
      if (p->frame == NONE)
         printf(" %6s", "-");
      else
         printf(" %6d", p->frame);
      if (p->accessTime == NONE)
         printf(" %7s", "-");
      else
         printf(" %7d", p->accessTime);
      if (p->loadTime == NONE)
         printf(" %7s", "-");
      else
         printf(" %7d", p->loadTime);
      printf(" %7d", p->nPeeks);
      printf(" %7d", p->nPokes);
      printf("\n");
   }
}