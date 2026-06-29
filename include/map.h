#ifndef MAP_H
#define MAP_H
#include "config.h"
typedef struct {
    int height;
    int width;
    char tiles[MAP_MAX_HEIGHT][MAP_MAX_WIDTH + 1];
    double player_init_x;
    double player_init_y;
} Map;

int map_load(Map* map, const char* filename);
void map_print(const Map* map);
void map_log(const Map* map);
int map_is_overbound(const Map* map, int x, int y);
int map_is_wall(const Map* map, int x, int y);

#endif