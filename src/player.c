#include "player.h"
#include "vector.h"
#include "config.h"
#include "collision.h"
#include <math.h>
#include <stdio.h>

int player_init(Player* player, Motion* motion, const Map* map, double x, double y) {
    player->x = x;
    player->y = y;
    player->angle = 0;
    player->motion = motion;
    player->radius = PLAYER_RADIUS;
    if (collision_is_safe(map, x, y, player->radius) == 0) {
        fprintf(stderr, "出生点 (%.2f, %.2f) 压墙,半径 %.2f\n", x, y, player->radius);
        return 0;
    }
    return 1;
}

void player_update(Player* player, const Map* map) {
    //update position
    motion_update_dir(player->motion);

    Vec2d forward = {cos(player->angle), sin(player->angle)};
    Vec2d right = {-sin(player->angle), cos(player->angle)};

    Vec2d local = motion_get_move(player->motion);

    Vec2d move = vec2d_scale((Vec2d){
                               local.y * forward.x + local.x * right.x,
                               local.y * forward.y + local.x * right.y},
      FIXED_DELTA_TIME);

    double next_x = player->x + move.x;
    double next_y = player->y + move.y;
    int x_access = 0, y_access = 0;

    if (collision_can_move_x(map, next_x, player->y, player->radius))
        x_access = 1;
    if (collision_can_move_y(map, player->x, next_y, player->radius))
        y_access = 1;

    if (x_access)
        player->x = next_x;
    if (y_access)
        player->y = next_y;

    //update direction
    player->angle += player->motion->turn_speed * player->motion->turn_dir * FIXED_DELTA_TIME;
    if (player->angle > TAU)
        player->angle -= TAU;
    if (player->angle < 0)
        player->angle += TAU;
}

void player_printf(Player* player) {
    printf("position:(%lf,%lf)\nangle:%lf\n", player->x, player->y, player->angle);
}
