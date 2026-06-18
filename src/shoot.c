#include "shoot.h"

RayHit shoot_update(const Map* map, InputState* input, double x, double y, double angle) {
    RayHit result = {0};
    if (input->fire_pressed)
        result = ray_check_dda(map, x, y, angle, MAX_VIEW_DISTANCE);
    return result;
}