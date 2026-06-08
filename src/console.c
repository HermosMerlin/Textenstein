#include "console.h"

#include <windows.h>

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
    SetConsoleCursorPosition(output, (COORD){0, 0});
}

void console_clear(void) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output == INVALID_HANDLE_VALUE) {
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(output, &info)) {
        return;
    }

    DWORD cell_count = (DWORD)info.dwSize.X * (DWORD)info.dwSize.Y;
    DWORD written = 0;
    COORD home = {0, 0};

    FillConsoleOutputCharacterA(output, ' ', cell_count, home, &written);
    FillConsoleOutputAttribute(output, info.wAttributes, cell_count, home, &written);
    SetConsoleCursorPosition(output, home);
}

void console_move_home(void) {
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output == INVALID_HANDLE_VALUE) {
        return;
    }

    COORD home = {0, 0};
    SetConsoleCursorPosition(output, home);
}
