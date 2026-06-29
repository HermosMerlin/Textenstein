#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

void log_init(const char* filename);
void log_close(void);
void log_save(const char* fmt, ...);

#endif