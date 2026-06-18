#ifndef SHOOT_H
#define SHOOT_H

#include "ray.h"
#include "input.h"

RayHit shoot_update(const Map* map, InputState* input, double x, double y, double angle);

#endif