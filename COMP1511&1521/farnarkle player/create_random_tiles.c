//Topic:Farnarkle
//Author:Victor Wang
//Date:29/3/17

#include <stdlib.h>
#include <time.h>
#include "farnarkle.h"

void create_random_tiles(int tiles[N_TILES]){
    int i;
    // seed (initialize) pseudo-random number generate with current time in seconds
    srand(time(NULL));

    i = 0;
    while (i < N_TILES) {
        // rand() returns a pseudo-random integer in ranger 0 to RAND_MAX inclusive
        // convert to an integer in the range 1..MAX_TILE_template
        tiles[i] = rand() % MAX_TILE + 1;
        i = i + 1;
    }
}