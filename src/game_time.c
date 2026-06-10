#include "game_time.h"
#include <windows.h>

double time_now_seconds(void) {
    static LARGE_INTEGER frequency;
    static int initialized = 0;

    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = 1;
    }

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return ((double)counter.QuadPart / (double)frequency.QuadPart);
}