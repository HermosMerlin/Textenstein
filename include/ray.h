#ifndef RAY_H
#define RAY_H

#include "map.h"
#include "vector.h"
#include <math.h>

typedef struct {
    int hit;
    double distance;
    double hit_x;
    double hit_y;
    int map_x;
    int map_y;
    double hit_angle;
    int hit_side;      // 0 -> 竖直墙 ; 1 -> 水平墙
    double hit_wall_x; //0.0 ~ 1.0
} RayHit;

RayHit ray_check_dda(
  const Map* map,
  double x, double y,
  double angle,
  double max_distance);

double point_to_segment_dis(Vec2d a, Vec2d b, Vec2d point);

#endif