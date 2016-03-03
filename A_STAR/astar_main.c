#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bl_hash.h"
#include "bl_heap.h"

#define MAX(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })
    
 #define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

struct map_square {
    int terrain;
};

struct field {
    int xLen;
    int yLen;
    struct map_square **grid;
};

struct coord {
    int x;
    int y;
};

struct astar_node {
    struct coord cur;
    struct astar_node *prev;
    int weightFromStart;
    int weightTotal;
};

struct field *init_map(int xLen, int yLen);
struct field *init_map_file(FILE *file, struct coord *start, struct coord *end);
struct field *init_map_rand(struct coord *start, struct coord *end);
struct astar_node *astar_path(struct field *map, struct coord start, struct coord end);
int astar_priority(const void *a, const void *b);
int grid_heuristic(struct coord cur, struct coord end);

int main(int argc, char **argv)
{
    srand(time(NULL));
    FILE *inFile = NULL;
    if(argc > 1) {
        inFile = fopen(argv[1], "r");
    }
    
    struct coord start = {0,0}, end = {0,0};
    struct field *map = NULL;
    if(inFile) {
        // read map from file
        map = init_map_file(inFile, &start, &end);
    } else {
        // start with random map
        map = init_map_rand(&start, &end);
    }
    
    struct astar_node *endNode = NULL;
    endNode = astar_path(map, start, end);
    
    if(endNode) {
        printf("found something\n");
    } else {
        printf("found nothing\n");
    }
    
    while(endNode) {
        map->grid[endNode->cur.x][endNode->cur.y].terrain = 2;
        endNode= endNode->prev;
    }
    
    for(int y = map->yLen - 1; y >= 0; y--) {
        printf("%2d ", y);
        for(int x = 0; x < map->xLen; x++) {
            switch(map->grid[x][y].terrain) {
                case 0:
                    printf("%c%c%c", 177, 177, 177);
                    break;
                case 1:
                    printf("%c%c%c", 178, 178, 178);
                    break;
                case 2:
                    printf(" X ");
                    break;
            }
        }
        printf("\n");
    }
    printf("  ");
    for(int x = 0; x < map->xLen; x++) {
        printf("%3d", x);
    }
    printf("\nstart = {%d,%d}\nend = {%d,%d}\n", start.x, start.y, end.x, end.y);
    
    return 0;
}


struct astar_node *astar_path(struct field *map, struct coord start, struct coord end)
{
    struct coord adjacentOffset[8] = {{-1,-1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};
    int adjacentWeight[8] = {14, 10, 14, 10, 14, 10, 14, 10};
    bl_heap *openList = bl_heap_new(10, astar_priority);    //priority queue of expanded nodes
    bl_hashtable *nodeList = bl_hashtable_new(100);         //table of all expanded nodes
    
    // init start node
    struct astar_node *node = malloc(sizeof(struct astar_node));
    node->cur = start;
    node->prev = NULL;
    node->weightFromStart = 0;
    node->weightTotal = grid_heuristic(node->cur, end);
    bl_heap_push(openList, node);
    bl_hashtable_insert(nodeList, &node->cur, sizeof(struct coord), node);
    
    while(bl_heap_count(openList) > 0) {
        node = bl_heap_pop(openList);
        if(node->cur.x == end.x && node->cur.y == end.y) {
            return node;
        }
        //TODO: go through every adjacent node
        for(int i = 0; i < 8; i++) {
            int chX = node->cur.x + adjacentOffset[i].x;
            int chY = node->cur.y + adjacentOffset[i].y;
            if((chX >= 0 && chX < map->xLen) && (chY >= 0 && chY < map->yLen)) {
                // coordinates are valid
                struct coord tmp = {chX, chY};
                if(bl_hashtable_get(nodeList, &tmp, sizeof(struct coord))) {
                    continue;
                }
                struct astar_node *adjNode = malloc(sizeof(struct astar_node));
                adjNode->cur = (struct coord){chX, chY};
                adjNode->prev = node;
                adjNode->weightFromStart = node->weightFromStart + adjacentWeight[i];
                adjNode->weightTotal = adjNode->weightFromStart + grid_heuristic(adjNode->cur, end);
                if(map->grid[chX][chY].terrain != 1) {// not a "wall"
                    bl_heap_push(openList, adjNode);
                }
                bl_hashtable_insert(nodeList, &adjNode->cur, sizeof(struct coord), adjNode);
            }
        }
    }
    return NULL;
}

int grid_heuristic(struct coord cur, struct coord end)
{
    // TODO: put weight and diagWeight in with terrain so the map can have different weighted tiles
    int dx = abs(cur.x - end.x);
    int dy = abs(cur.y - end.y);
    int weight = 10;
    int diagWeight = 14;
    return weight * (dx + dy) + (diagWeight - 2 * weight) * MIN(dx, dy);
}

struct field *init_map(int xLen, int yLen)
{
    struct field *map = malloc(sizeof(struct field));
    map->xLen = xLen;
    map->yLen = yLen;
    map->grid = calloc(xLen, sizeof(struct map_quare *));
    for(int i = 0; i < xLen; i++) {
        map->grid[i] = calloc(yLen, sizeof(struct map_square));
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
    // fill grid from file
    for(int y = yLen - 1; y >= 0; y--) {
        fgets(buf, 1024, file);
        for(int x = 0; x < xLen; x++) {
            if(buf[x] == 'W') { // "wall" or impassable
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
            if(rand() % 100 < 40){ // 20% chance of wall
                map->grid[x][y].terrain = 1;
            }
        }
    }
    map->grid[start->x][start->y].terrain = 0;
    map->grid[end->x][end->y].terrain = 0;
    return map;
}

int astar_priority(const void *a, const void *b)
{
    struct astar_node *node1 = (struct astar_node *)a;
    struct astar_node *node2 = (struct astar_node *)b;
    return node2->weightTotal - node1->weightTotal; // lower weight is better 
}
