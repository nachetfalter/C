// vmsim.c ... Virtual memory simulation
// COMP1521 17s2 Assignment 2
// Written by John Shepherd, September 2017
//
// Sets up memory, page table, replacement policy, etc.
// Reads sequence of memory references to simulate program execution
// Prints running statistics and summary statistics after execution ends

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "Memory.h"
#include "PageTable.h"
#include "Stats.h"

int processArgs(int, char **, int *, int *, int *);
int getNextReference(int *, char *);

// main: setup and run the simulation

int main(int argc, char *argv[])
{
   int policy;  // page replacement policy
   int nFrames; // size of memory (in # frames)
   int nPages;  // size of process address space

   if (!processArgs(argc, argv, &policy, &nPages, &nFrames))
      return EXIT_FAILURE;

   // Set up data structures
   initMemory(nFrames);
   initPageTable(policy, nPages);
   initStats();

#ifdef DBUG
   showPageTableStatus();
#endif

   int  pageNo;    // which page is accessed
   char mode;      // accessed for read or write
   int  time = 0;  // current time on clock
   while (getNextReference(&pageNo, &mode)) {
#ifdef DBUG
      printf("\n=== Request %c%d @ t=%d ===\n",mode,pageNo,time);
#endif
      if (mode == 'r')
         countPeekRequest();
      else
         countPokeRequest();
      requestPage(pageNo, mode, time);
      time++;
#ifdef DBUG
      showPageTableStatus();
#endif
   }

   showSummaryStats();

   return EXIT_SUCCESS;
}

// processArgs: process command-line arguments

int processArgs(int argc, char **argv, int *pol, int *np, int *nf)
{
   // Process command-line args
   if (argc < 4) {
      fprintf(stderr, "Usage: %s Policy #Pages #Frames\n", argv[0]);
      return 0;
   }
   if (strcasecmp(argv[1],"LRU") == 0)
      *pol = REPL_LRU;
   else if (strcasecmp(argv[1],"FIFO") == 0)
      *pol = REPL_FIFO;
   else if (strcasecmp(argv[1],"Clock") == 0)
      *pol = REPL_CLOCK;
   else {
      fprintf(stderr, "%s: invalid Policy %s\n", argv[0], argv[1]);
      fprintf(stderr, "Policy must be one of LRU|FIFO|Clock\n");
      return 0;
   }
   if (sscanf(argv[2], "%d", np) < 1) {
      fprintf(stderr, "%s: invalid #Pages %s\n", argv[0], argv[2]);
      return 0;
   }
   if (*np < MIN_PAGES) {
      fprintf(stderr, "%s: #Pages must be >= %d\n", argv[0], MIN_PAGES);
      return 0;
   }
   if (sscanf(argv[3], "%d", nf) < 1) {
      fprintf(stderr, "%s: invalid #Frames\n", argv[0]);
      return 0;
   }
   if (*nf < MIN_FRAMES) {
      fprintf(stderr, "%s: #Frames must be >= %d\n", argv[0], MIN_FRAMES);
      return 0;
   }
   return 1;
}

// getNextReference: get next page reference from stdin

int getNextReference(int *page, char *mode)
{
   int ch;
   // skip white space
   ch = getchar();
   while (ch != EOF && isspace(ch))
      ch = getchar();
   if (ch == 'r' || ch == 'w')
      *mode = ch;
   else
      return 0;
   if (scanf("%d", page) == 1)
      return 1;
   else
      return 0;
}
