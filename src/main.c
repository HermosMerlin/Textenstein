#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "config.h"
#include "map.h"
#include "input.h"
#include "player.h"
#include "motion.h"
#include "ray.h"
#include "renderer.h"
#include "console.h"
#include "game_time.h"
#include "wall_texture.h"
#include "shoot.h"
#include "frame_buffer.h"
#include "entity.h"

int main(void) {
    console_init(SCREEN_WIDTH, SCREEN_HEIGHT);
    printf("Textenstein Loading...\n");

    Map map;

    if (map_load(&map, "../assets/complex_map_e.txt")) {
        printf("Loaded map\n");
        map_print(&map);
    }
    else {
        printf("Failed to load map\n");
        return 1;
    }

    EntityManager entity_manager;
    entity_manager_init(&entity_manager);
    if (entity_load(&map, &entity_manager)) {
        printf("Loaded entities: %d\n", entity_manager.count);
    }
    else {
        printf("Failed to load entities\n");
        return 1;
    }

    Texture texture;
    if (wall_texture_load(&texture, "../assets/wall_brick_16.txt")) {
        printf("Loaded wall texture\n");
    }
    else {
        printf("Failed to load wall texture\n");
        return 1;
    }

    InputState input = {0};
    Motion motion;
    Player player;
    motion_init(&motion, &input);
    if (player_init(&player, &motion, &map, &entity_manager, map.player_init_x, map.player_init_y) == 1)
        printf("Player created\n");
    else {
        printf("Player failed to create\n");
        exit(1);
    }

    FrameBuffer frame_buffer;
    frame_buffer_init(&frame_buffer);

    Sleep(3000);
    console_clear();

    while (1) {
        double start_time = time_now_seconds();

        input_poll(&input);
        player_update(&player, &map,&entity_manager);
        renderer_render_background(&map, &texture, &player, &frame_buffer);
        entity_render(&entity_manager, &player, &frame_buffer);
        console_write_frame(&frame_buffer);

        double end_time = time_now_seconds();

        if (input.quit) {
            break;
        }

        double cost_time = end_time - start_time;
        double sleep_time = DELTA_TIME - cost_time;
        if (sleep_time > 0.0)
            Sleep((DWORD)(sleep_time * 1000.0));
    }

    console_show_cursor();
    return 0;
}
