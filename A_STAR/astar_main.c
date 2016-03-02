#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bl_hash.h"
#include "bl_heap.h"

struct mapSquare {
    int terrain;
};

struct field {
    int xLen;
    int yLen;
    struct mapSquare **grid;
};

struct coord {
    unsigned x;
    unsigned y;
};

struct astar_node {
    struct coord cur;
    struct coord prev;
};

struct field *init_map(int xLen, int yLen);
struct field *init_map_file(FILE *file, struct coord *start, struct coord *end);
struct field *init_map_rand(struct coord *start, struct coord *end);

int main(int argc, char **argv)
{
    FILE *inFile = NULL;
    if(argc > 1) {
        inFile = fopen(argv[1], "r");
    }
    
    srand(time(NULL));
    int xLen = 0, yLen = 0;
    struct coord start = {0,0}, end = {0,0};
    struct field *map = NULL;
    if(inFile) {
        // read map from file
        map = init_map_file(inFile, &start, &end);
    } else {
        // start with random map
        map = init_map_rand(&start, &end);
    }
    
    return 0;
}

struct field *init_map(int xLen, int yLen)
{
    struct field *map = malloc(sizeof(struct field));
    map->xLen = xLen;
    map->yLen = yLen;
    map->grid = calloc(xLen, sizeof(struct mapSquare *));
    for(int i = 0; i < xLen; i++) {
        map->grid[i] = calloc(yLen, sizeof(struct mapSquare));
    }
    return map;
}

struct field *init_map_file(FILE *file, struct coord *start, struct coord *end)
{
    char buf[1024];
    int xLen = 0, yLen = 0;
    struct field *map = NULL;
    
    fgets(buf, 1024, file);
    xLen = atoi(buf);
    fgets(buf, 1024, file);
    yLen = atoi(buf);
    map = init_map(xLen, yLen);
    for(int y = yLen - 1; y >= 0; y--) {
        fgets(buf, 1024, file);
        for(int x = 0; x < xLen; x++) {
            if(buf[x] == 'W') {
                map->grid[x][y].terrain = 1;
            } else if(buf[x] == 'S') {
                start->x = x;
                start->y = y;
            } else if(buf[x] == 'E') {
                end->x = x;
                end->y = y;
            } else {
                map->grid[x][y].terrain = 0;
            }
        }
    }
}

struct field *init_map_rand(struct coord *start, struct coord *end)
{
    int xLen = (rand() % 15) + 5; // 5 - 20
    int yLen = (rand() % 15) + 5; // 5 - 20
    struct field *map = init_map(xLen, yLen);
    // random start/end
    *start = (struct coord){rand() % xLen, rand() % yLen};
    *end = (struct coord){rand() % xLen, rand() % yLen};
    // fill grid with random walls
    for(int x = 0; x < xLen; x++) {
        for(int y = 0; y < yLen; y++) {
            if(rand() % 100 < 20) { // 20% chance of wall
                map->grid[x][y].terrain = 1;
            }
        }
    }
    return map;
}
