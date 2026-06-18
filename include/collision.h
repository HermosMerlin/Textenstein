#ifndef COLLISION_H
#define COLLISION_H

#include "map.h"

int collision_is_safe(const Map* map, double x, double y, double r);
int collision_can_move_x(const Map* map, double next_x, double y, double r);
int collision_can_move_y(const Map* map, double x, double next_y, double r);

#endif