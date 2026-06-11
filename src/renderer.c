#include "renderer.h"
#include "config.h"
#include "ray.h"
#include "console.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

char get_wall_char(const RayHit* ray, const Texture* texture, double corrected_distance, double wall_y, int screen_x, int screen_y) {
    int bayer[4][4] = {
      {0, 8, 2, 10},
      {12, 4, 14, 6},
      {3, 11, 1, 9},
      {15, 7, 13, 5}};
    double threshold = bayer[screen_y % 4][screen_x % 4] / 16.0;

    //远距离模糊墙面
    if (ray->distance > WALL_FADE_START_DISTANCE) {
        double visibility = (ray->distance - WALL_FADE_START_DISTANCE) / (MAX_VIEW_DISTANCE - WALL_FADE_START_DISTANCE);

        if (visibility > threshold)
            return ' ';
    }

    //综合角度和距离计算光照强度
    double angle_factor = 0.2 + 0.8 * pow((1.0 - fabs(ray->hit_angle / (PI / 2.0))), 2.5);
    double distance_factor = 1.0 / (pow(((corrected_distance / MAX_VIEW_DISTANCE) + 1), 2.0));

    //计算光照强度
    double bright = 1.0 * distance_factor * angle_factor;

    //计算墙面纹理参数
    double wall_x = ray->hit_wall_x;

    double value = 0.5 + bright * 6.0;
    double texture_delta = wall_texture_get_factor(texture, wall_x, wall_y);

    double texture_strength;
    if (texture_delta < 0.0) {
        texture_strength = 0.30 + 0.35 * angle_factor;
    }
    else {
        texture_strength = 0.20;
    }

    value *= 1.0 + texture_delta * texture_strength;

    //抖动 dithering
    int index = (int)value;
    double frac = value - index;

    if (frac > threshold)
        index++;

    if (index < 0)
        index = 0;
    if (index > 7)
        index = 7;

    const char WALL_SHADE_CHARS[WALL_SHADE_COUNT] = {
      '.', ':', 'c', 'o', 'O', 'P', '#', '@'};

    return WALL_SHADE_CHARS[index];
}

void renderer_render_frame(const Map* map, const Texture* texture, double x, double y, double player_angle) {
    char buffer[SCREEN_HEIGHT][SCREEN_WIDTH + 1];
    ConsoleColor color_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

    //考虑最大视距，计算地面消失点
    double max_wall_height = (double)SCREEN_HEIGHT / MAX_VIEW_DISTANCE;
    int floor_top = (int)((SCREEN_HEIGHT / 2.0) + (max_wall_height / 2.0));

    //打印天空
    for (int i = 0; i < floor_top; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            buffer[i][j] = ' ';
            color_buffer[i][j] = CONSOLE_COLOR_DEFAULT;
        }
        buffer[i][SCREEN_WIDTH] = '\0';
    }

    //打印地面

    for (int i = floor_top; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            buffer[i][j] = '`';
            color_buffer[i][j] = CONSOLE_COLOR_FLOOR;
        }
        buffer[i][SCREEN_WIDTH] = '\0';
    }

    //打印墙面
    RayHit result = {0};
    double half_fov = FOV / 2.0;
    for (int screen_col = 0; screen_col < SCREEN_WIDTH; screen_col++) {
        //window column归一化到 -1 ~ 1 范围
        double camera_x = 2.0 * (screen_col + 0.5) / SCREEN_WIDTH - 1.0;
        double angle_offset = atan(camera_x * tan(half_fov));
        double angle = player_angle + angle_offset;

        if (angle < 0)
            angle += PI * 2.0;
        else if (angle > PI * 2.0)
            angle -= PI * 2.0;

        result = ray_check_dda(map, x, y, angle, MAX_VIEW_DISTANCE);

        if (result.hit == 1) {
            //鱼眼矫正
            double corrected_distance = result.distance * cos(angle_offset);
            double wall_height = SCREEN_HEIGHT / corrected_distance;
            double wall_top = SCREEN_HEIGHT / 2.0 - wall_height / 2.0;
            double wall_bottom = SCREEN_HEIGHT / 2.0 + wall_height / 2.0;

            //处理墙面是否造成屏幕越界
            int wall_top_i = (int)ceil(wall_top - 0.5);
            int wall_bottom_i = (int)floor(wall_bottom - 0.5);

            if (wall_top_i < 0)
                wall_top_i = 0;
            if (wall_bottom_i >= SCREEN_HEIGHT)
                wall_bottom_i = SCREEN_HEIGHT - 1;

            //将墙面填充入frame
            for (int screen_y = wall_top_i; screen_y <= wall_bottom_i; screen_y++) {
                double wall_y = ((screen_y + 0.5) - wall_top) / wall_height;
                buffer[screen_y][screen_col] = get_wall_char(&result, texture, corrected_distance, wall_y, screen_col, screen_y);
                color_buffer[screen_y][screen_col] = CONSOLE_COLOR_WALL;
            }
        }
    }

    //输出到屏幕
    console_write_frame(buffer, color_buffer);

    fflush(stdout);
}
