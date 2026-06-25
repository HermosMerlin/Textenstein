#include "player.h"
#include "vector.h"
#include "config.h"
#include "collision.h"
#include <math.h>
#include <stdio.h>

int player_init(Player* player, Motion* motion, const Map* map, const EntityManager* em, double x, double y) {
    player->x = x;
    player->y = y;
    player->angle = 0;
    player->motion = motion;
    player->radius = PLAYER_RADIUS;
    player->velocity = (Vec2d){0, 0};
    if (collision_is_safe(map, em, x, y, player->radius) == 0) {
        fprintf(stderr, "出生点 (%.2f, %.2f) 压墙,半径 %.2f\n", x, y, player->radius);
        return 0;
    }
    return 1;
}

//update player velocity
void player_velocity_update(Player* player) {
    Vec2d forward = {cos(player->angle), sin(player->angle)};
    Vec2d right = {-sin(player->angle), cos(player->angle)};

    Vec2d accel_local = motion_get_accel(player->motion);

    Vec2d accel_world = (Vec2d){
      accel_local.y * forward.x + accel_local.x * right.x,
      accel_local.y * forward.y + accel_local.x * right.y};

    double speed_now = vec2d_length(player->velocity);

    if (speed_now <= PLAYER_FRICTION * DELTA_TIME)
        player->velocity = (Vec2d){0, 0};
    else {
        Vec2d friction_accel = vec2d_scale(vec2d_normalize(player->velocity), -1.0 * PLAYER_FRICTION);
        player->velocity = vec2d_add(vec2d_scale(friction_accel, DELTA_TIME), player->velocity);
    }

    player->velocity = vec2d_add(player->velocity, vec2d_scale(accel_world, DELTA_TIME));

    speed_now = vec2d_length(player->velocity);
    if (speed_now > PLAYER_MAX_SPEED) {
        player->velocity = vec2d_scale(player->velocity, PLAYER_MAX_SPEED / speed_now);
    }
}

//update position after calculating collision
void player_update(Player* player, const Map* map, const EntityManager* em) {
    player_velocity_update(player);

    double delta_x = player->velocity.x * DELTA_TIME;
    double delta_y = player->velocity.y * DELTA_TIME;

    double next_x = player->x + delta_x;
    double next_y = player->y + delta_y;

    //先尝试组合位置移动
    if (collision_is_safe(map, em, next_x, next_y, player->radius)) {
        player->x = next_x;
        player->y = next_y;
    }
    //失败后分轴判断移动
    else if (collision_can_move_x(map, em, next_x, player->y, player->radius)) {
        player->x = next_x;
        player->velocity.y = 0;
    }
    else if (collision_can_move_y(map, em, player->x, next_y, player->radius)) {
        player->y = next_y;
        player->velocity.x = 0;
    }
    else {
        player->velocity.x = 0;
        player->velocity.y = 0;
    }

    //update direction
    player->angle += player->motion->turn_speed * player->motion->turn_dir * DELTA_TIME;
    if (player->angle > TAU)
        player->angle -= TAU;
    if (player->angle < 0)
        player->angle += TAU;
}

void player_printf(Player* player) {
    printf("position:(%lf,%lf)\nangle:%lf\n", player->x, player->y, player->angle);
}
