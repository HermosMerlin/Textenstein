#include <stdio.h>
#include <windows.h>
#include "map.h"
#include "input.h"
#include "player.h"
#include "motion.h"
int main(void) {
    printf("Textenstein started\n");
    Map map;

    if (map_load(&map, "../assets/default_map.txt")) {
        printf("Loaded map\n");
        map_print(&map);
    }
    else
        printf("Failed to load map\n");

    // input_test();
    InputState input;
    Motion motion;
    Player player;
     motion_init(&motion, &input);
    player_init(&player,&motion, 0, 0);
   

    while (1) {
        input_poll(&input);

        player_update(&player, &motion);
        player_printf(&player);

        if (input.quit) {
            break;
        }
        Sleep(100);
    }

    return 0;
}