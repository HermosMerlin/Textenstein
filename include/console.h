#ifndef CONSOLE_H
#define CONSOLE_H

#include "config.h"
#include "frame_buffer.h"

void console_init(int cols, int rows);
void console_clear(void);
void console_show_cursor(void);
void console_write_frame(const FrameBuffer* framebuffer);

#endif
