#ifndef SHOOT_H
#define SHOOT_H

#include "entity.h"
#include "player.h"
#include "input.h"
#include "vector.h"

typedef struct ShootState {
    int hit;
    int entity_id;
    Vec2d hit_pos;
} ShootState;

int shoot_state_init(ShootState* shoot_state);
void shoot_update(const Map* map, ShootState* shoot_state, InputState* input, EntityManager* em, Player* player);

#endif