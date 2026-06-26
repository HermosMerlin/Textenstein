#include "shoot.h"
#include "ray.h"
#include "config.h"

int shoot_state_init(ShootState* shoot_state) {
    shoot_state->entity_id = -1;
    shoot_state->hit = -1;
    shoot_state->hit_pos = (Vec2d){0, 0};
    return 1;
}
void shoot_update(const Map* map, ShootState* shoot_state, InputState* input, EntityManager* em, Player* player) {
    RayHit result = {0};
    EntityManager tmp;
    entity_manager_init(&tmp);

    if (input->fire_pressed)
        result = ray_check_dda(map, player->x, player->y, player->angle, MAX_VIEW_DISTANCE);
    else
        return;

    if (result.hit == 0)
        shoot_state->hit = 0;

    else if (result.hit == 1) {
        for (int i = 0; i < ENTITY_MAX; i++) {
            double dis = point_to_segment_dis((Vec2d){player->x, player->y}, (Vec2d){result.hit_x, result.hit_y}, em->entitylist[i].pos);
            if (em->entitylist[i].alive && dis < PLAYER_RADIUS + ENTITY_RADIUS) {
                entity_manager_push(&tmp, &em->entitylist[i]);
            }
        }

        if (tmp.count == 0) {
            shoot_state->hit = 1;
            shoot_state->hit_pos = (Vec2d){result.hit_x, result.hit_y};
            return;
        }

        double min_dis = pow(pow(tmp.entitylist[0].pos.x - player->x, 2) + pow(tmp.entitylist[0].pos.y - player->y, 2), 0.5);
        int pop_id = tmp.entitylist[0].id;
        for (int i = 1; i < tmp.count; i++) {
            if (tmp.entitylist[i].alive == 0)
                break;
            double dis = pow(pow(tmp.entitylist[i].pos.x - player->x, 2) + pow(tmp.entitylist[i].pos.y - player->y, 2), 0.5);
            if (dis < min_dis) {
                min_dis = dis;
                pop_id = tmp.entitylist[i].id;
            }
        }
        shoot_state->hit = 2;
        shoot_state->entity_id = pop_id;
        for (int i = 0; i < ENTITY_MAX; i++) {
            if (em->entitylist[i].id == pop_id) {
                shoot_state->hit_pos = em->entitylist[i].pos;
                break;
            }
        }
        entity_manager_pop(em, pop_id);
    }
}