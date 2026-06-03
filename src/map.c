#include "map.h"
#include <stdio.h>
#include <string.h>

void load_map(Map* map, const char* filename) {
    FILE* file = fopen(filename, "r");

    char line[MAX_WIDTH + 2];

    if (file == NULL) {
        perror(filename);
        return;
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
}
void map_print(const Map* map) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            printf("%c",map->tiles[i][j]);
        }
        printf("\n");
    }
}
int is_wall(const Map* map, int x, int y) {
    return (map->tiles[y][x] == '#');
}