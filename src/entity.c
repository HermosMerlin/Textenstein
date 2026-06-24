#include "entity.h"
#include "config.h"
#include <math.h>

void entity_init(Entity* entity, int x, int y) {
    entity->alive = 1;
    entity->pos = (Vec2d){x + 0.5, y + 0.5};
    entity->radius = ENTITY_RADIUS;
    entity->id = -1;
}

void entity_manager_init(EntityManager* em) {
    for (int i = 0; i < ENTITY_MAX; i++) {
        em->entitylist[i].alive = 0;
        em->entitylist[i].id = -1;
    }
    em->count = 0;
    em->id_cursor = 0;
}

int entity_manager_push(EntityManager* em, int x, int y) {
    if (em->count >= ENTITY_MAX)
        return 0;

    for (int i = 0; i < ENTITY_MAX; i++) {
        if (em->entitylist[i].alive == 0) {
            entity_init(&em->entitylist[i], x, y);
            em->entitylist[i].id = em->id_cursor;
            em->id_cursor++;
            em->count++;
            return 1;
        }
    }
    return 0;
}

int entity_manager_pop(EntityManager* em, int id) {
    for (int i = 0; i < ENTITY_MAX; i++) {
        if (em->entitylist[i].id == id) {
            em->entitylist[i].alive = 0;
            em->entitylist[i].id = -1;
            em->count--;
            return 1;
        }
    }
    return 0;
}

int entity_load(const Map* map, EntityManager* enemies) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if (map->tiles[i][j] == 'E') {
                if (entity_manager_push(enemies, j, i) == 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}