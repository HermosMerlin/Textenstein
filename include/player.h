#ifndef PLAYER_H
#define PLAYER_H

#include "motion.h"
#include "map.h"
typedef struct {
    double x;
    double y;
    double angle;
    Motion* motion;
} Player;

void player_init(Player* player, Motion* motion, double x, double y);
void player_update(Player* player, const Map* map);
void player_printf(Player* player);
#endif