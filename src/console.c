#include "console.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ANSI_MAX_LEN 32
#define GLYPH_MAX_LEN 4

#define FRAME_MAX \
    (SCREEN_HEIGHT * SCREEN_WIDTH * (ANSI_MAX_LEN + GLYPH_MAX_LEN) + SCREEN_HEIGHT + ANSI_MAX_LEN)

static int vt_enabled = 0;
static WORD default_attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

static void console_move_home(void);
static void console_hide_cursor(void);
static void console_set_color(ConsoleColor color);
static void console_reset_color(void);

static const char* console_vt_color_code(ConsoleColor color) {
    switch (color) {
    case CONSOLE_COLOR_FLOOR:
        return "\x1b[38;2;156;128;76m";
    case CONSOLE_COLOR_WALL:
        return "\x1b[38;2;132;138;124m";
    case CONSOLE_COLOR_DEFAULT:
    default:
        return "\x1b[0m";
    }
}

static WORD console_win32_color_attribute(ConsoleColor color) {
    switch (color) {
    case CONSOLE_COLOR_FLOOR:
        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    case CONSOLE_COLOR_WALL:
        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    case CONSOLE_COLOR_DEFAULT:
    default:
        return default_attributes;
    }
}

void console_init(int cols, int rows) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output == INVALID_HANDLE_VALUE) {
        return;
    }

    if (cols < 1) {
        cols = 1;
    }
    if (rows < 1) {
        rows = 1;
    }

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    DWORD mode = 0;
    if (GetConsoleMode(output, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        vt_enabled = SetConsoleMode(output, mode) != 0;
    }

    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(output, &info)) {
        default_attributes = info.wAttributes;

        int current_cols = info.srWindow.Right - info.srWindow.Left + 1;
        int current_rows = info.srWindow.Bottom - info.srWindow.Top + 1;

        if (cols < current_cols || rows < current_rows) {
            SMALL_RECT shrink_rect;
            shrink_rect.Left = 0;
            shrink_rect.Top = 0;
            shrink_rect.Right = (SHORT)(cols - 1);
            shrink_rect.Bottom = (SHORT)(rows - 1);
            SetConsoleWindowInfo(output, TRUE, &shrink_rect);
        }
    }

    COORD buffer_size;
    buffer_size.X = (SHORT)cols;
    buffer_size.Y = (SHORT)rows;

    SMALL_RECT window_rect;
    window_rect.Left = 0;
    window_rect.Top = 0;
    window_rect.Right = (SHORT)(cols - 1);
    window_rect.Bottom = (SHORT)(rows - 1);

    SetConsoleScreenBufferSize(output, buffer_size);
    SetConsoleWindowInfo(output, TRUE, &window_rect);
    console_move_home();
    console_hide_cursor();
}

void console_clear(void) {
    if (vt_enabled) {
        printf("\x1b[0m\x1b[2J\x1b[H");
        fflush(stdout);
    }
    else {
        console_reset_color();
        system("cls");
    }
}

static void console_move_home(void) {
    if (vt_enabled) {
        printf("\x1b[H");
        fflush(stdout);
    }
    else {
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        if (output == INVALID_HANDLE_VALUE) {
            return;
        }
        SetConsoleCursorPosition(output, (COORD){0, 0});
    }
}

static void console_hide_cursor(void) {
    if (vt_enabled) {
        printf("\x1b[?25l");
        fflush(stdout);
    }
    else {
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursor_info;
        if (output == INVALID_HANDLE_VALUE || !GetConsoleCursorInfo(output, &cursor_info)) {
            return;
        }
        cursor_info.bVisible = FALSE;
        SetConsoleCursorInfo(output, &cursor_info);
    }
}

void console_show_cursor(void) {
    if (vt_enabled) {
        printf("\x1b[?25h");
        fflush(stdout);
    }
    else {
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursor_info;
        if (output == INVALID_HANDLE_VALUE || !GetConsoleCursorInfo(output, &cursor_info)) {
            return;
        }
        cursor_info.bVisible = TRUE;
        SetConsoleCursorInfo(output, &cursor_info);
    }
}

static void console_set_color(ConsoleColor color) {
    if (vt_enabled) {
        printf("%s", console_vt_color_code(color));
    }
    else {
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        if (output == INVALID_HANDLE_VALUE) {
            return;
        }
        SetConsoleTextAttribute(output, console_win32_color_attribute(color));
    }
}

static void console_reset_color(void) {
    console_set_color(CONSOLE_COLOR_DEFAULT);
}

static void append_string(char* a, const char* b, int* pos, int max_size) {
    for (int i = 0; b[i] != '\0' && (*pos) < max_size; i++) {
        a[*pos] = b[i];
        (*pos)++;
    }
}

void console_write_frame(
    const char buffer[SCREEN_HEIGHT][SCREEN_WIDTH + 1],
    const ConsoleColor color_buffer[SCREEN_HEIGHT][SCREEN_WIDTH]
) {
    char frame[FRAME_MAX];
    int pos = 0;
    ConsoleColor pre_color = CONSOLE_COLOR_DEFAULT;

    console_move_home();
    for (int row = 0; row < SCREEN_HEIGHT; row++) {
        for (int col = 0; col < SCREEN_WIDTH; col++) {
            if (pre_color != color_buffer[row][col]) {
                append_string(frame, console_vt_color_code(color_buffer[row][col]), &pos, FRAME_MAX);
                pre_color = color_buffer[row][col];
            }
            frame[pos] = buffer[row][col];
            pos++;
        }
        if (row < SCREEN_HEIGHT - 1) {
            frame[pos] = '\n';
            pos++;
        }
    }
    append_string(frame, console_vt_color_code(CONSOLE_COLOR_DEFAULT), &pos, FRAME_MAX);
    frame[pos] = '\0';
    printf("%s", frame);
    fflush(stdout);
}
