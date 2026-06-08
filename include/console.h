#ifndef CONSOLE_H
#define CONSOLE_H

void console_init(int cols, int rows);
void console_clear(void);
void console_move_home(void);
void console_hide_cursor(void);
void console_show_cursor(void);

#endif
