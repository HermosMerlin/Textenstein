#ifndef INPUT_H
#define INPUT_H

typedef struct {
    int forward;
    int backward;
    int strafe_left;
    int strafe_right;
    int turn_left;
    int turn_right;
    int quit;
} InputState;

void input_poll(InputState* input);
void input_test();

#endif