#ifndef MAP_H
#define MAP_H
#include "config.h"
typedef struct {
    int height;
    int width;
    char tiles[MAX_HEIGHT][MAX_WIDTH + 1];
} Map;

void load_map(Map* map, const char* filename);
void map_print(const Map* map);
int is_wall(const Map* map, int x, int y);

#endif