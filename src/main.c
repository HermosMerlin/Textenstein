#include <stdio.h>
#include "map.h"
int main(void) {
    printf("Textenstein started\n");
    Map map;

    if (map_load(&map, "../assets/default_map.txt")) {
        printf("Loaded map\n");
        map_print(&map);
    }
    else
        printf("Failed to load map\n");

    return 0;
}