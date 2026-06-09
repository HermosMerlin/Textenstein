#include "renderer.h"
#include "config.h"
#include "ray.h"
#include "console.h"
#include <math.h>
#include <stdio.h>

void renderer_windowcolumn_debug(const Map* map, double x, double y, double player_angle) {
    RayHit result;
    double half_fov = FOV / 2.0;
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        //window column归一化到 -1 ~ 1 范围
        double camera_x = 2.0 * (i + 0.5) / SCREEN_WIDTH - 1.0;
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
            printf("col:%d angle:%lf dis:%lf map:(%d,%d) wall:%lf~%lf\n", i, angle, result.distance, result.map_x, result.map_y, wall_bottom, wall_top);
        }
        else
            printf("col:%d angle:%lf not hit\n", i, angle);
    }
}

char get_wall_char_by_distance(double distance, double hit_angle) {
    //综合角度和距离计算光照强度
    double angle_factor = 0.2 + 0.8 * pow((1.0 - fabs(hit_angle / (PI / 2.0))), 2.5);
    double bright = 1.0 / (pow(distance, 1.2)) * angle_factor;

    if (bright > 0.8)
        return '@';
    if (bright > 0.5)
        return '#';
    if (bright > 0.1)
        return 'o';
    if (bright > 0.05)
        return '+';
    return '*';
}

char get_floor_char_by_row(int row) {
    if (SCREEN_HEIGHT - row < (SCREEN_HEIGHT / 2.0) * 0.3)
        return '=';
    if (SCREEN_HEIGHT - row < (SCREEN_HEIGHT / 2.0) * 0.6)
        return '"';
    if (SCREEN_HEIGHT - row < (SCREEN_HEIGHT / 2.0) * 0.9)
        return '.';
    return ' ';
}

void renderer_render_frame(const Map* map, double x, double y, double player_angle) {
    char buffer[SCREEN_HEIGHT][SCREEN_WIDTH + 1];

    //打印天空
    for (int i = 0; i < SCREEN_HEIGHT / 2; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            buffer[i][j] = ' ';
        }
        buffer[i][SCREEN_WIDTH] = '\0';
    }

    //打印地面
    for (int i = SCREEN_HEIGHT / 2; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            buffer[i][j] = '.';
        }
        buffer[i][SCREEN_WIDTH] = '\0';
    }

    //打印墙面
    RayHit result;
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
            if (wall_top < 0)
                wall_top = 0;
            if (wall_bottom >= SCREEN_HEIGHT)
                wall_bottom = SCREEN_HEIGHT - 1;

            //将墙面填充入frame
            for (int screen_y = wall_top; screen_y <= wall_bottom; screen_y++) {
                buffer[screen_y][screen_col] = get_wall_char_by_distance(corrected_distance, result.hit_angle);
            }
        }
    }

    //输出到屏幕
    console_move_home();
    for (int row = 0; row < SCREEN_HEIGHT - 1; row++) {
        printf("%s\n", buffer[row]);
    }
    printf("%s", buffer[SCREEN_HEIGHT - 1]);
    fflush(stdout);
}
