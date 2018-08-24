//Topic:Farnarkle
//Author:Victor Wang
//Date:29/3/17

#define N_TILES 4
#define MAX_TILE 8
#define MAX_TURNS 100

int read_tiles(int tiles[N_TILES]);
void print_tiles(int tiles[N_TILES]);
int count_farnarkles(int sequence1[N_TILES], int sequence2[N_TILES]);
int count_arkles(int sequence1[N_TILES], int sequence2[N_TILES]);
void create_random_tiles(int tiles[N_TILES]);
void farnarkle_player(int turn, int prev[N_TILES], int far, int ark, int ans[N_TILES]);