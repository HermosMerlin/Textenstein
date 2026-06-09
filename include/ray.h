#ifndef RAY_H
#define RAY_H

#include "map.h"
#include <math.h>

typedef struct {
    int hit;
    double distance;
    double hit_x;
    double hit_y;
    int map_x;
    int map_y;
    double hit_angle;
    int hit_side;
} RayHit;

RayHit ray_check(
  const Map* map,
  double x, double y,
  double angle,
  double max_distance);

RayHit ray_check_dda(
  const Map* map,
  double x, double y,
  double angle,
  double max_distance);

#endif