#ifndef MOTION_H
#define MOTION_H

#include "vector.h"
#include "input.h"

typedef struct {
    double accel;
    double turn_speed;
    Vec2d move_dir;
    double turn_dir;
    InputState* input;
} Motion;

void motion_init(Motion* motion, InputState* input);

void motion_update_dir(Motion* motion);
Vec2d motion_get_accel(Motion* motion);
#endif