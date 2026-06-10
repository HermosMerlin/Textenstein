#include <stdio.h>
#include <windows.h>
#include "config.h"
#include "map.h"
#include "input.h"
#include "player.h"
#include "motion.h"
#include "ray.h"
#include "renderer.h"
#include "console.h"

int main(void) {
    console_init(SCREEN_WIDTH, SCREEN_HEIGHT);
    printf("Textenstein Loading...\n");

    Map map;

    if (map_load(&map, "../assets/complex_map.txt")) {
        printf("Loaded map\n");
        map_print(&map);
    }
    else {
        printf("Failed to load map\n");
        return 1;
    }

    // input_test();
    InputState input;
    Motion motion;
    Player player;
    motion_init(&motion, &input);
    player_init(&player, &motion, map.player_init_x, map.player_init_y);
    printf("Player created\n");

    Sleep(5000);
    console_clear();

    while (1) {
        input_poll(&input);

        player_update(&player, &map);

        renderer_render_frame(&map, player.x, player.y, player.angle);

        if (input.quit) {
            break;
        }
        Sleep((DWORD)(FIXED_DELTA_TIME * 1000.0));
    }

    console_show_cursor();
    return 0;
}
