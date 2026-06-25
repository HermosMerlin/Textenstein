#ifndef PLAYER_H
#define PLAYER_H

#include "motion.h"
#include "map.h"

typedef struct EntityManager EntityManager;
typedef struct {
    double x;
    double y;
    double angle;
    double radius;
    Vec2d velocity;
    Motion* motion;
} Player;

int player_init(Player* player, Motion* motion, const Map* map, const EntityManager* em, double x, double y);
void player_update(Player* player, const Map* map, const EntityManager* em);
void player_printf(Player* player);
#endif