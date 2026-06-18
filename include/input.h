#ifndef INPUT_H
#define INPUT_H

typedef struct {
    int forward;
    int backward;
    int strafe_left;
    int strafe_right;
    int turn_left;
    int turn_right;
    int fire;
    int dash;
    int quit;

    // 边沿状态
    int forward_pressed, backward_pressed, strafe_left_pressed, strafe_right_pressed;
    int turn_left_pressed, turn_right_pressed;
    int fire_pressed;
    int dash_pressed;
    int quit_pressed;

    int _prev_forward, _prev_backward, _prev_strafe_left, _prev_strafe_right;
    int _prev_turn_left, _prev_turn_right;
    int _prev_fire, _prev_dash, _prev_quit;
} InputState;

void input_poll(InputState* input);

#endif