#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "config.h"

typedef enum Color {
    COLOR_DEFAULT = 0,
    COLOR_FLOOR,
    COLOR_WALL,
    COLOR_ENEMY
} ConsoleColor;

typedef struct {
    char chars[SCREEN_HEIGHT][SCREEN_WIDTH];
    ConsoleColor colors[SCREEN_HEIGHT][SCREEN_WIDTH];
    double depth[SCREEN_WIDTH];
} FrameBuffer;

void frame_buffer_init(FrameBuffer* fb);

#endif