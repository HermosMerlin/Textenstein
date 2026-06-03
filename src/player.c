#include "player.h"
#include "vector.h"
#include "config.h"
#include <math.h>
#include <stdio.h>

void player_init(Player* player, Motion* motion, double x, double y) {
    player->x = x;
    player->y = y;
    player->angle = 0;
    player->motion = motion;
}

void player_update(Player* player, Motion* motion) {
    //update position
    motion_update_dir(motion);

    Vec2d forward = {cos(player->angle), sin(player->angle)};
    Vec2d right = {-sin(player->angle), cos(player->angle)};

    Vec2d local = motion_get_move(motion);

    Vec2d move = {
      local.y * forward.x + local.x * right.x,
      local.y * forward.y + local.x * right.y};

    player->x += move.x;
    player->y += move.y;

    //update direction
    player->angle += motion->turn_speed * motion->turn_dir;
    if (player->angle > TAU)
        player->angle -= TAU;
    if (player->angle < 0)
        player->angle += TAU;
}

void player_printf(Player* player) {
    printf("position:(%lf,%lf)\nangle:%lf\n", player->x, player->y, player->angle);
}