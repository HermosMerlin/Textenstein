#include "ray.h"

RayHit ray_check(const Map* map, double x, double y, double angle, double max_distance) {
    RayHit ray;
    ray.hit = 0;
    double dis = 0;
    double step = 0.001;
    while (dis < max_distance) {
        x += cos(angle) * step;
        y += sin(angle) * step;
        dis += step;

        if (map_in_wall(map, x, y)) {
            ray.hit = 1;
            ray.hit_x = x;
            ray.hit_y = y;
            ray.map_x = (int)x;
            ray.map_y = (int)y;
            ray.distance = dis;
            break;
        }
    }
    return ray;
}