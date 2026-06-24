#include "frame_buffer.h"
#include "config.h"

void frame_buffer_init(FrameBuffer* fb) {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            fb->chars[i][j] = ' ';
            fb->colors[i][j] = COLOR_DEFAULT;
        }
    }
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        fb->depth[i] = 0;
    }
}