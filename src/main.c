#include <stdio.h>
#include <windows.h>
#include "map.h"
#include "input.h"
#include "player.h"
#include "motion.h"
#include "ray.h"

int main(void) {
    printf("Textenstein started\n");
    Map map;

    if (map_load(&map, "../assets/default_map.txt")) {
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

    while (1) {
        input_poll(&input);

        player_update(&player, &motion, &map);
        player_printf(&player);

        RayHit ray_t;
        ray_t = ray_check(&map, player.x, player.y, player.angle, 20);
        if (ray_t.hit)
            printf("hit:(%lf,%lf)  (%d,%d)\n\n", ray_t.hit_x, ray_t.hit_y, ray_t.map_x, ray_t.map_y);
        else
            printf("not hit\n");

        if (input.quit) {
            break;
        }
        Sleep(delta_t * 1000);
    }

    return 0;
}