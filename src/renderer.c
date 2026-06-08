#include "renderer.h"
#include "config.h"
#include "ray.h"
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