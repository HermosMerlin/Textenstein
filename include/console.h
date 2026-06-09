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
void console_move_home(void);
void console_hide_cursor(void);
void console_show_cursor(void);
void console_set_color(ConsoleColor color);
void console_reset_color(void);
void console_write_colored_line(const char* text, const ConsoleColor* colors, int length, int newline);
void console_write_frame(
    const char buffer[SCREEN_HEIGHT][SCREEN_WIDTH + 1],
    const int color_buffer[SCREEN_HEIGHT][SCREEN_WIDTH]
);

#endif
