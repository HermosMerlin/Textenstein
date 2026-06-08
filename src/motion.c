#include "motion.h"
#include "config.h"

void motion_init(Motion* motion, InputState* input) {
    motion->input = input;
    motion->move_speed = PLAYER_MOVE_SPEED;
    motion->turn_speed = PLAYER_TURN_SPEED;
}

void motion_update_dir(Motion* motion) {
    motion->move_dir = vec2d_normalize((Vec2d){
      motion->input->strafe_right - motion->input->strafe_left,
      motion->input->forward - motion->input->backward});

    motion->turn_dir = motion->input->turn_right - motion->input->turn_left;
}

Vec2d motion_get_move(Motion* motion) {
    return (vec2d_scale(motion->move_dir, motion->move_speed));
}
