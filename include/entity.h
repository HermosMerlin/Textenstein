#ifndef ENTITY_H
#define ENTITY_H

#include "vector.h"
#include "map.h"
#include "frame_buffer.h"
#include "player.h"
typedef struct {
    int id;
    Vec2d pos;
    int alive;
    double radius;
} Entity;

typedef struct EntityManager {
    Entity entitylist[ENTITY_MAX];
    int count;
    int id_cursor;
} EntityManager;

void entity_init(Entity* entity, int x, int y);
void entity_manager_init(EntityManager* em);

int entity_load(const Map* map, EntityManager* enemies);

int entity_manager_push(EntityManager* em, int x, int y);
int entity_manager_pop(EntityManager* em, int id);

void entity_render(EntityManager* em, Player* player, FrameBuffer* fb);

#endif