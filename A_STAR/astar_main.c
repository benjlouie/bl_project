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

struct jps_node {
    struct coord cur;
    struct jps_node *prev;
    int direction; // -1 means from no direction (start node)
    int weightFromStart;
    int weightTotal;
};

struct field *init_map(int xLen, int yLen);
struct field *init_map_file(FILE *file, struct coord *start, struct coord *end);
struct field *init_map_rand(struct coord *start, struct coord *end);
struct astar_node *astar_path(struct field *map, struct coord start, struct coord end);
int astar_priority(const void *a, const void *b);
int grid_heuristic(struct coord cur, struct coord end);
void print_map(struct field *map);

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
        printf("path found\n");
    } else {
        printf("no path found\n");
    }

    while(endNode) {
        map->grid[endNode->cur.x][endNode->cur.y].terrain = 2;
        endNode= endNode->prev;
    }
    
    print_map(map);
    printf("\nstart = {%d,%d}\nend = {%d,%d}\n", start.x, start.y, end.x, end.y);
    
    return 0;
}


struct astar_node *astar_path(struct field *map, struct coord start, struct coord end)
{
    struct coord adjOffset[8] = {{-1,-1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};
    int adjWeight[8] = {1414, 1000, 1414, 1000, 1414, 1000, 1414, 1000};
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
        // go through every adjacent node
        for(int i = 0; i < 8; i++) {
            int chX = node->cur.x + adjOffset[i].x;
            int chY = node->cur.y + adjOffset[i].y;
            if((chX >= 0 && chX < map->xLen) && (chY >= 0 && chY < map->yLen)) {
                // coordinates are valid
                struct coord newCoord = {chX, chY};
                struct astar_node *tmpNode = bl_hashtable_get(nodeList, &newCoord, sizeof(struct coord));
                if(tmpNode) { // node already expanded
                    if(map->grid[chX][chY].terrain != 1) { // not a wall
                        int tmpWeight = node->weightFromStart + grid_heuristic(tmpNode->cur, end) + adjWeight[i];
                        if(tmpNode->weightTotal > tmpWeight) { // check if a better path to tmpNode exists
                            tmpNode->prev = node;
                            tmpNode->weightFromStart = node->weightFromStart + adjWeight[i];
                            tmpNode->weightTotal = tmpWeight;
                            bl_heap_push(openList, tmpNode); // node has been added more than once (this is ok)
                        }
                    }
                    continue; // already expanded
                } else {
                    struct astar_node *adjNode = malloc(sizeof(struct astar_node));
                    adjNode->cur = newCoord;
                    adjNode->prev = node;
                    adjNode->weightFromStart = node->weightFromStart + adjWeight[i];
                    adjNode->weightTotal = adjNode->weightFromStart + grid_heuristic(adjNode->cur, end);
                    if(map->grid[chX][chY].terrain != 1) {// not a "wall"
                        bl_heap_push(openList, adjNode);
                        map->grid[chX][chY].terrain = 3; //TODO: this is debug feature
                    }
                    bl_hashtable_insert(nodeList, &adjNode->cur, sizeof(struct coord), adjNode);
                }
            }
        }
    }
    return NULL;
}

struct jps_node *jps_path(struct field *map, struct coord start, struct coord end)
{
    struct coord adjOffset[8] = {{-1,-1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};
    bl_heap *openList = bl_heap_new(10, astar_priority);  // priority queue of expanded nodes
    bl_hashtable *nodeList = bl_hashtable_new(100);       // table of all expanded nodes
    
    // init start node and have it go in all directions
    struct jps_node *node = malloc(sizeof(struct jps_node));
    node->cur = start;
    node->prev = NULL;
    node->direction = -1;
    node->weightFromStart = 0;
    node->weightTotal = grid_heuristic(node->cur, end);
    bl_heap_push(openList, node);
    bl_hashtable_insert(nodeList, &node->cur, sizeof(struct coord), node);
    // init every adjacent node to the start node? (probably easier)
    //TODO: deal with edge case of how to init the start node and the nodes around it
    
    // go through each node in the openList
    while((node = bl_heap_pop(openList))) {
        if(node->cur.x == end.x && node->cur.y == end.y) {
            return node;
        }
        int chX = node->cur.x + adjOffset[node->direction].x;
        int chY = node->cur.y + adjOffset[node->direction].y;
        if(node->direction % 2 == 0) { // diagonal
            ;
        } else { // horizontal
            // go until there is wall on left of right
            struct coord left = adjOffset[(node->direction - 2) % 8];
            struct coord right = adjOffset[(node->direction + 2) % 8];
            left.x += node->cur.x;
            left.y += node->cur.y;
            right.x += node->cur.x;
            right.y += node->cur.y;
            //TODO: do it right
            //TODO: case where there's wall and it has to go diagonal or something (look online)
        }
        // use JPS to jump in that direction and continually search diagonally for jump points
        // horiz -- when there is wall to left or right
        // diag -- when there is wall to left or right of horizontally expanded node
            // add that to the openList
        
    }
    return NULL;
}

int grid_heuristic(struct coord cur, struct coord end)
{
    // TODO: put weight and diagWeight in with terrain so the map can have different weighted tiles
    int dx = abs(cur.x - end.x);
    int dy = abs(cur.y - end.y);
    int weight = 1000;
    int diagWeight = 1414;
    return weight * (dx + dy) + (diagWeight - (2 * weight)) * MIN(dx, dy);
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
    return map;
}

struct field *init_map_rand(struct coord *start, struct coord *end)
{
    int xLen = (rand() % 15) + 11; // 5 - 25
    int yLen = (rand() % 15) + 5; // 5 - 25
    struct field *map = init_map(xLen, yLen);
    // random start/end
    *start = (struct coord){rand() % xLen, rand() % yLen};
    *end = (struct coord){rand() % xLen, rand() % yLen};
    // fill grid with random walls
    for(int x = 0; x < xLen; x++) {
        for(int y = 0; y < yLen; y++) {
            if(rand() % 100 < 40){ // 40% chance of wall
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

void print_map(struct field *map)
{
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
                case 3:
                    printf("%c%c%c", 176, 176, 176);
                    break;
            }
        }
        printf("\n");
    }
    printf("  ");
    for(int x = 0; x < map->xLen; x++) {
        printf("%3d", x);
    }
}
