#include "entity.h"
#include "config.h"
#include <math.h>

void entity_init(Entity* entity, int x, int y) {
    entity->alive = 1;
    entity->pos = (Vec2d){x + 0.5, y + 0.5};
    entity->radius = ENTITY_RADIUS;
    entity->id = -1;
}

void entity_manager_init(EntityManager* em) {
    for (int i = 0; i < ENTITY_MAX; i++) {
        em->entitylist[i].alive = 0;
        em->entitylist[i].id = -1;
    }
    em->count = 0;
    em->id_cursor = 0;
}

int entity_manager_push(EntityManager* em, Entity* entity) {
    if (em->count >= ENTITY_MAX)
        return 0;

    for (int i = 0; i < ENTITY_MAX; i++) {
        if (em->entitylist[i].alive == 0) {
            em->entitylist[i] = *entity;
            em->count++;
            em->id_cursor = fmax(em->entitylist[i].id + 1, em->id_cursor);
            return 1;
        }
    }
    return 0;
}

int entity_manager_create(EntityManager* em, int x, int y) {
    if (em->count >= ENTITY_MAX)
        return 0;

    for (int i = 0; i < ENTITY_MAX; i++) {
        if (em->entitylist[i].alive == 0) {
            entity_init(&em->entitylist[i], x, y);
            em->entitylist[i].id = em->id_cursor;
            em->id_cursor++;
            em->count++;
            return 1;
        }
    }
    return 0;
}

int entity_manager_pop(EntityManager* em, int id) {
    for (int i = 0; i < ENTITY_MAX; i++) {
        if (em->entitylist[i].id == id) {
            em->entitylist[i].alive = 0;
            em->entitylist[i].id = -1;
            em->count--;
            return 1;
        }
    }
    return 0;
}

int entity_load(const Map* map, EntityManager* enemies) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if (map->tiles[i][j] == 'E') {
                if (entity_manager_create(enemies, j, i) == 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void entity_render(EntityManager* em, Player* player, FrameBuffer* fb) {
    for (int i = 0; i < ENTITY_MAX; i++) {
        if (em->entitylist[i].alive) {
            //将enemy坐标转化到角色镜头坐标系
            Vec2d enemy_cpos, camera_i, camera_j, delta_pos;
            camera_i = (Vec2d){-sin(player->angle), cos(player->angle)};
            camera_j = (Vec2d){cos(player->angle), sin(player->angle)};
            delta_pos = (Vec2d){em->entitylist[i].pos.x - player->x, em->entitylist[i].pos.y - player->y};
            //cpos -> camera_pos
            enemy_cpos = (Vec2d){vec2d_dot(delta_pos, camera_i), vec2d_dot(delta_pos, camera_j)};
            double enemy_dis = vec2d_length(enemy_cpos);

            if (enemy_cpos.y <= 0)
                continue;

            int enemy_height = (int)round(SCREEN_HEIGHT / enemy_dis * ENEMY_VISUAL_HEIGHT);
            int enemy_width = (int)round(ENTITY_RADIUS * SCREEN_WIDTH / (enemy_cpos.y * tan(FOV / 2.0)));

            //必须严格对应renderer模块中算法
            double camera_x = enemy_cpos.x / enemy_cpos.y / tan(FOV / 2.0);
            double center_col = (camera_x + 1.0) * SCREEN_WIDTH * 0.5 - 0.5;
            int start_col = (int)round(center_col - (enemy_width / 2.0));
            int end_col = start_col + enemy_width - 1;

            start_col = fmax(0, start_col);
            end_col = fmin(SCREEN_WIDTH - 1, end_col);

            for (int col = start_col; col <= end_col; col++) {
                if (fb->depth[col] <= enemy_cpos.y)
                    continue;

                int row_bottom = (int)round(SCREEN_HEIGHT * (1.0 / enemy_cpos.y + 1.0) / 2.0);
                int row_top = row_bottom - enemy_height;

                row_top = fmax(0, row_top);
                row_bottom = fmin(SCREEN_HEIGHT - 1, row_bottom);

                for (int row = row_top; row <= row_bottom; row++) {
                    fb->chars[row][col] = '$';
                    fb->colors[row][col] = COLOR_ENEMY;
                }
            }
        }
    }
}