#include "map.h"
#include <stdio.h>
#include <string.h>

int map_load(Map* map, const char* filename) {
    FILE* file = fopen(filename, "r");

    char line[MAX_WIDTH + 2];

    if (file == NULL) {
        perror(filename);
        return 0;
    }

    map->height = 0;
    map->width = 0;

    while (map->height < MAX_HEIGHT && fgets(line, sizeof(line), file) != NULL) {
        size_t len = strcspn(line, "\r\n");

        if ((int)len > MAX_WIDTH) {
            len = MAX_WIDTH;
        }

        memcpy(map->tiles[map->height], line, len);
        map->tiles[map->height][len] = '\0';

        if ((int)len > map->width) {
            map->width = (int)len;
        }

        map->height++;
    }

    fclose(file);
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