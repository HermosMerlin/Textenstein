#include "renderer.h"
#include "config.h"
#include "ray.h"
#include <math.h>
#include <stdio.h>

void renderer_windowcolumn_debug(const Map* map, double x, double y, double player_angle) {
    RayHit result;
    double half_fov = FOV / 2.0;
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        double camera_x = 2.0 * (i + 0.5) / SCREEN_WIDTH - 1.0;
        double angle_offset = atan(camera_x * tan(half_fov));
        double angle = player_angle + angle_offset;

        if (angle < 0)
            angle += PI * 2.0;
        else if (angle > PI * 2.0)
            angle -= PI * 2.0;

        result = ray_check_dda(map, x, y, angle, MAX_VIEW_DISTANCE);
        printf("col:%d angle:%lf dis:%lf map:(%d,%d)\n", i, angle, result.distance, result.map_x, result.map_y);
    }
}