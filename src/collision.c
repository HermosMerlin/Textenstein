#include "collision.h"
#include <math.h>
int collision_is_safe(const Map* map, const EntityManager* em, double x, double y, double r) {
    int x_min = (int)floor(x - r);
    int x_max = (int)floor(x + r);
    int y_min = (int)floor(y - r);
    int y_max = (int)floor(y + r);

    for (int cx = x_min; cx <= x_max; cx++) {
        for (int cy = y_min; cy <= y_max; cy++) {
            if (map_is_wall(map, cx, cy) == 1)
                return 0;
        }
    }

    for (int i = 0; i < ENTITY_MAX; i++) {
        if (em->entitylist[i].alive) {
            double distance = PLAYER_RADIUS + ENTITY_RADIUS;
            if (fabs(x - em->entitylist[i].pos.x) < distance && fabs(y - em->entitylist[i].pos.y) < distance) {
                return 0;
            }
        }
    }

    return 1;
}

int collision_can_move_x(const Map* map, const EntityManager* em, double next_x, double y, double r) {
    int x_min = (int)floor(next_x - r);
    int x_max = (int)floor(next_x + r);
    int y_min = (int)floor(y - r);
    int y_max = (int)floor(y + r);

    for (int cy = y_min; cy <= y_max; cy++) {
        if (map_is_wall(map, x_min, cy))
            return 0;
        if (map_is_wall(map, x_max, cy))
            return 0;
    }

     for (int i = 0; i < ENTITY_MAX; i++) {
        if (em->entitylist[i].alive) {
            double distance = PLAYER_RADIUS + ENTITY_RADIUS;
            if (fabs(next_x - em->entitylist[i].pos.x) < distance && fabs(y - em->entitylist[i].pos.y) < distance) {
                return 0;
            }
        }
    }

    return 1;
}

int collision_can_move_y(const Map* map, const EntityManager* em, double x, double next_y, double r) {
    int x_min = (int)floor(x - r);
    int x_max = (int)floor(x + r);
    int y_min = (int)floor(next_y - r);
    int y_max = (int)floor(next_y + r);

    for (int cx = x_min; cx <= x_max; cx++) {
        if (map_is_wall(map, cx, y_min))
            return 0;
        if (map_is_wall(map, cx, y_max))
            return 0;
    }

     for (int i = 0; i < ENTITY_MAX; i++) {
        if (em->entitylist[i].alive) {
            double distance = PLAYER_RADIUS + ENTITY_RADIUS;
            if (fabs(x - em->entitylist[i].pos.x) < distance && fabs(next_y - em->entitylist[i].pos.y) < distance) {
                return 0;
            }
        }
    }

    return 1;
}
