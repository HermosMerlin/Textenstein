#include <stdio.h>
#include <windows.h>
#include "map.h"
#include "input.h"
int main(void) {
    printf("Textenstein started\n");
    Map map;

    if (map_load(&map, "../assets/default_map.txt")) {
        printf("Loaded map\n");
        map_print(&map);
    }
    else
        printf("Failed to load map\n");

    input_test();

    return 0;
}