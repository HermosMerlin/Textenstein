#include "console.h"

#include <stdio.h>
#include <windows.h>

#define ANSI_MAX_LEN 32
#define GLYPH_MAX_LEN 4

#define FRAME_MAX \
    (SCREEN_HEIGHT * SCREEN_WIDTH * (ANSI_MAX_LEN + GLYPH_MAX_LEN) + SCREEN_HEIGHT + ANSI_MAX_LEN)

static const char* console_vt_color_code(ConsoleColor color) {
    switch (color) {
    case COLOR_FLOOR:
        return "\x1b[38;2;156;128;76m";
    case COLOR_WALL:
        return "\x1b[38;2;132;138;124m";
    case COLOR_ENEMY:
        return "\x1b[38;2;255;64;64m";
    case COLOR_DEFAULT:
    default:
        return "\x1b[0m";
    }
}

static void console_move_home(void) {
    printf("\x1b[H");
    fflush(stdout);
}

static void console_hide_cursor(void) {
    printf("\x1b[?25l");
    fflush(stdout);
}

void console_init(int cols, int rows) {
    if (cols < 1) {
        cols = 1;
    }
    if (rows < 1) {
        rows = 1;
    }

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output == INVALID_HANDLE_VALUE) {
        return;
    }

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    DWORD mode = 0;
    if (GetConsoleMode(output, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(output, mode);
    }

    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(output, &info)) {
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
    printf("\x1b[0m\x1b[2J\x1b[H");
    fflush(stdout);
}

void console_show_cursor(void) {
    printf("\x1b[?25h");
    fflush(stdout);
}

static void append_string(char* a, const char* b, int* pos, int max_size) {
    for (int i = 0; b[i] != '\0' && (*pos) < max_size; i++) {
        a[*pos] = b[i];
        (*pos)++;
    }
}

void console_write_frame(const FrameBuffer* fb) {
    char frame[FRAME_MAX];
    int pos = 0;
    ConsoleColor pre_color = COLOR_DEFAULT;

    console_move_home();
    for (int row = 0; row < SCREEN_HEIGHT; row++) {
        for (int col = 0; col < SCREEN_WIDTH; col++) {
            if (pre_color != fb->colors[row][col]) {
                append_string(frame, console_vt_color_code(fb->colors[row][col]), &pos, FRAME_MAX);
                pre_color = fb->colors[row][col];
            }
            frame[pos] = fb->chars[row][col];
            pos++;
        }
        if (row < SCREEN_HEIGHT - 1) {
            frame[pos] = '\n';
            pos++;
        }
    }
    append_string(frame, console_vt_color_code(COLOR_DEFAULT), &pos, FRAME_MAX);
    frame[pos] = '\0';
    printf("%s", frame);
    fflush(stdout);
}
