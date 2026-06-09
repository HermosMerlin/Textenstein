#ifndef CONSOLE_H
#define CONSOLE_H

#include "config.h"
typedef enum ConsoleColor {
    CONSOLE_COLOR_DEFAULT = 0,
    CONSOLE_COLOR_FLOOR,
    CONSOLE_COLOR_WALL
} ConsoleColor;

void console_init(int cols, int rows);
void console_clear(void);
void console_show_cursor(void);
void console_write_frame(
    const char buffer[SCREEN_HEIGHT][SCREEN_WIDTH + 1],
    const ConsoleColor color_buffer[SCREEN_HEIGHT][SCREEN_WIDTH]
);

#endif
