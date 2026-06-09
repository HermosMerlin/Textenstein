#include "ray.h"
#include "vector.h"

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
    if (ray.hit == 0) {
        ray.distance = max_distance;
    }
    return ray;
}

RayHit ray_check_dda(const Map* map, double x, double y, double angle, double max_distance) {
    RayHit ray;
    ray.hit = 0;
    ray.hit_x = x;
    ray.hit_y = y;
    ray.map_x = (int)floor(x);
    ray.map_y = (int)floor(y);

    Vec2d dir = vec2d_angle_to_vector(angle);
    // double dir.x = cos(angle);
    // double dir.y = sin(angle);

    const double eps = 1e-6;
    const double inf = 1e30;

    double delta_dis_x = (fabs(dir.x) < eps) ? inf : fabs(1.0 / dir.x);
    double delta_dis_y = (fabs(dir.y) < eps) ? inf : fabs(1.0 / dir.y);

    int step_x = (dir.x >= 0) ? 1 : -1;
    int step_y = (dir.y >= 0) ? 1 : -1;

    double side_dis_x = (dir.x >= 0)
                        ? (ray.map_x + 1.0 - x) * delta_dis_x
                        : (x - ray.map_x) * delta_dis_x;
    double side_dis_y = (dir.y >= 0)
                        ? (ray.map_y + 1.0 - y) * delta_dis_y
                        : (y - ray.map_y) * delta_dis_y;

    double dis = 0;
    while (dis < max_distance) {
        if (side_dis_x <= side_dis_y) {
            dis = side_dis_x;
            ray.map_x += step_x;

            side_dis_x += delta_dis_x;
            //命中竖直方向边线
            ray.hit_side = 0;
        }
        else if (side_dis_y < side_dis_x) {
            dis = side_dis_y;
            ray.map_y += step_y;

            side_dis_y += delta_dis_y;
            //命中水平方向边线
            ray.hit_side = 1;
        }
        if (dis > max_distance) {
            break;
        }
        if (map_is_wall(map, ray.map_x, ray.map_y)) {
            ray.hit = 1;
            ray.hit_x = x + dir.x * dis;
            ray.hit_y = y + dir.y * dis;
            ray.distance = dis;

            //计算法线和射线-平面夹角
            Vec2d normal;
            if (ray.hit_side == 0) {
                normal = (dir.x > 0) ? (Vec2d){-1, 0} : (Vec2d){1, 0};
            }
            else if (ray.hit_side == 1) {
                normal = (dir.y > 0) ? (Vec2d){0, -1} : (Vec2d){0, 1};
            }
            else {
                normal = (Vec2d){0, 0};
            }

            //cos α = (vecA * vecB) / (|vecA| * |vecB|);
            ray.hit_angle = acos(vec2d_dot(normal, dir) * -1);

            return ray;
        }
    }

    ray.distance = max_distance;
    return ray;
}
