#include <stdio.h>
#include <map.h>
int main(void) {
    printf("Textenstein started\n");
    Map map;
    load_map(&map, "../assets/default_map.txt");
    map_print(&map);
    return 0;
}