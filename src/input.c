#include "input.h"
#include <windows.h>
#include<stdio.h>

static int key_down(int key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}
void input_poll(InputState* input) {
    input->forward = key_down('W');
    input->backward = key_down('S');
    input->strafe_left = key_down('A');
    input->strafe_right = key_down('D');

    input->turn_left = key_down(VK_LEFT) || key_down('J');
    input->turn_right = key_down(VK_RIGHT) || key_down('L');

    input->quit = key_down(VK_ESCAPE);
}

void input_test() {
    InputState input;
    while (1) {
        input_poll(&input);

        printf("W:%d S:%d A:%d D:%d L:%d R:%d Q:%d\n",
          input.forward,
          input.backward,
          input.strafe_left,
          input.strafe_right,
          input.turn_left,
          input.turn_right,
          input.quit);

        if (input.quit) {
            break;
        }

        Sleep(100);
    }
}