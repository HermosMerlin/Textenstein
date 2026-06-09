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
        // printf("Loaded map\n");
        // map_print(&map);
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

    Sleep(5000);
    console_clear();

    while (1) {
        input_poll(&input);

        player_update(&player, &motion, &map);
        // player_printf(&player);

        // RayHit ray_t;
        // ray_t = ray_check_dda(&map, player.x, player.y, player.angle, MAX_VIEW_DISTANCE);
        // if (ray_t.hit)
        //     printf("hit:(%lf,%lf)  (%d,%d)\n\n", ray_t.hit_x, ray_t.hit_y, ray_t.map_x, ray_t.map_y);
        // else
        //     printf("not hit\n");

        // renderer_windowcolumn_debug(&map, player.x, player.y, player.angle);

        renderer_render_frame(&map, player.x, player.y, player.angle);

        if (input.quit) {
            break;
        }
        Sleep((DWORD)(FIXED_DELTA_TIME * 1000.0));
    }

    console_show_cursor();
    return 0;
}
