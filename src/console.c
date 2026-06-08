#include "console.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static int vt_enabled = 0;

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
        printf("\x1b[2J\x1b[H");
        fflush(stdout);
    }
    else {
        system("cls");
    }
}

void console_move_home(void) {
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

void console_hide_cursor(void) {
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
