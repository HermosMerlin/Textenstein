#include "input.h"
#include <windows.h>

static int key_down(int key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}
void input_poll(InputState* input) {
    // 当前状态
    int forward_now = key_down('W');
    int backward_now = key_down('S');
    int strafe_left_now = key_down('A');
    int strafe_right_now = key_down('D');
    int turn_left_now = key_down(VK_LEFT) || key_down('J');
    int turn_right_now = key_down(VK_RIGHT) || key_down('L');
    int fire_now = key_down(VK_SPACE);
    int dash_now = key_down(VK_SHIFT);
    int quit_now = key_down(VK_ESCAPE);

    // 边沿检测:当前按 + 上帧没按 = 刚按下
    input->forward_pressed = forward_now && !input->_prev_forward;
    input->backward_pressed = backward_now && !input->_prev_backward;
    input->strafe_left_pressed = strafe_left_now && !input->_prev_strafe_left;
    input->strafe_right_pressed = strafe_right_now && !input->_prev_strafe_right;
    input->turn_left_pressed = turn_left_now && !input->_prev_turn_left;
    input->turn_right_pressed = turn_right_now && !input->_prev_turn_right;
    input->fire_pressed = fire_now && !input->_prev_fire;
    input->dash_pressed = dash_now && !input->_prev_dash;
    input->quit_pressed = quit_now && !input->_prev_quit;

    // 更新当前状态
    input->forward = forward_now;
    input->backward = backward_now;
    input->strafe_left = strafe_left_now;
    input->strafe_right = strafe_right_now;
    input->turn_left = turn_left_now;
    input->turn_right = turn_right_now;
    input->fire = fire_now;
    input->dash = dash_now;
    input->quit = quit_now;

    // 保存本帧状态供下帧用
    input->_prev_forward = forward_now;
    input->_prev_backward = backward_now;
    input->_prev_strafe_left = strafe_left_now;
    input->_prev_strafe_right = strafe_right_now;
    input->_prev_turn_left = turn_left_now;
    input->_prev_turn_right = turn_right_now;
    input->_prev_fire = fire_now;
    input->_prev_dash = dash_now;
    input->_prev_quit = quit_now;
}