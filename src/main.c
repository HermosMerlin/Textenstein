#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "config.h"
#include "map.h"
#include "input.h"
#include "player.h"
#include "motion.h"
#include "renderer.h"
#include "console.h"
#include "game_time.h"
#include "wall_texture.h"
#include "shoot.h"
#include "frame_buffer.h"
#include "entity.h"
#include "log.h"

int main(void) {
    log_init("../log.txt");

    console_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    Map map;
    if (map_load(&map, "../assets/complex_map_e.txt")) {
        log_save("Loaded map\n");
        map_log(&map);
    }
    else {
        log_save("Failed to load map\n");
        return 1;
    }

    EntityManager entity_manager;
    entity_manager_init(&entity_manager);
    if (entity_load(&map, &entity_manager)) {
        log_save("Loaded entities: %d\n", entity_manager.count);
    }
    else {
        log_save("Failed to load entities\n");
        return 1;
    }

    Texture texture;
    if (wall_texture_load(&texture, "../assets/wall_brick_16.txt")) {
        log_save("Loaded wall texture\n");
    }
    else {
        log_save("Failed to load wall texture\n");
        return 1;
    }

    InputState input = {0};
    Motion motion;
    Player player;
    motion_init(&motion, &input);
    if (player_init(&player, &motion, &map, &entity_manager, map.player_init_x, map.player_init_y) == 1)
        log_save("Player created\n");
    else {
        log_save("Player failed to create\n");
        exit(1);
    }

    FrameBuffer frame_buffer;
    frame_buffer_init(&frame_buffer);

    ShootState shoot_state;
    shoot_state_init(&shoot_state);

    printf("Game will start...");
    Sleep(3000);
    console_clear();

    while (1) {
        double start_time = time_now_seconds();

        input_poll(&input);
        shoot_update(&map, &shoot_state, &input, &entity_manager, &player);
        player_update(&player, &map, &entity_manager);
        renderer_render_background(&map, &texture, &player, &frame_buffer);
        entity_render(&entity_manager, &player, &frame_buffer);
        console_write_frame(&frame_buffer);

        printf("\nhit:%d entity_id:%d pos:(%lf,%lf)", shoot_state.hit, shoot_state.entity_id, shoot_state.hit_pos.x, shoot_state.hit_pos.y);

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
    log_close();
    return 0;
}
