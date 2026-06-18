#include "map.h"
#include <stdio.h>
#include <string.h>

int map_load(Map* map, const char* filename) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror(filename);
        return 0;
    }

    char line[MAP_MAX_WIDTH + 2];

    map->height = 0;
    map->width = 0;

    while (map->height < MAP_MAX_HEIGHT && fgets(line, sizeof(line), file) != NULL) {
        size_t len = strcspn(line, "\r\n");

        if ((int)len > MAP_MAX_WIDTH) {
            len = MAP_MAX_WIDTH;
        }

        memcpy(map->tiles[map->height], line, len);
        map->tiles[map->height][len] = '\0';

        if ((int)len > map->width) {
            map->width = (int)len;
        }

        map->height++;
    }

    fclose(file);

    map->player_init_x = -1;
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if (map->tiles[i][j] == 'P') {
                map->player_init_x = (double)j + 0.5;
                map->player_init_y = (double)i + 0.5;
                map->tiles[i][j] = '.';
            }
        }
    }

    if (map->player_init_x == -1)
        return 0;

    return 1;
}

void map_print(const Map* map) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            printf("%c", map->tiles[i][j]);
        }
        printf("\n");
    }
}

int map_is_overbound(const Map* map, int x, int y) {
    if (x < 0 || x >= map->width || y < 0 || y >= map->height)
        return 1;
    return 0;
}

int map_is_wall(const Map* map, int x, int y) {
    if (map_is_overbound(map, x, y))
        return 1;
    return (map->tiles[y][x] == '#');
}

int map_in_wall(const Map* map, double x, double y) {
    if (x < 0 || x > (double)(map->width - 1) || y < 0 || y > (double)(map->height - 1))
        return 1;
    if (map_is_wall(map, (int)x, (int)y))
        return 1;
    return 0;
}