#include "log.h"
#include <time.h>
#include <stdio.h>

static FILE* logFile = NULL;

void log_init(const char* filename) {
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        // 文件不存在，创建
        fp = fopen(filename, "w");
        if (fp == NULL)
            return;
        fclose(fp);
    }
    else {
        fclose(fp);
    }

    // 以追加方式打开
    logFile = fopen(filename, "a");

    
    time_t now = time(NULL);
    log_save("\n==================================================\n");
    log_save("Game Log Session %s",ctime(&now));
    log_save("==================================================\n");

}

void log_close(void) {
    if (logFile) {
        fclose(logFile);
        logFile = NULL;
    }
}

void log_save(const char* fmt, ...) {
    if (!logFile)
        return;

    va_list args;
    va_start(args, fmt);
    vfprintf(logFile, fmt, args);
    va_end(args);
    fflush(logFile);
}