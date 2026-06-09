#ifndef CONFIG_H
#define CONFIG_H

enum {
    MAP_MAX_HEIGHT = 128,
    MAP_MAX_WIDTH = 128,

    SCREEN_WIDTH = 174,
    SCREEN_HEIGHT = 42
};

#define WALL_SHADE_COUNT 8
static const char WALL_SHADE_CHARS[WALL_SHADE_COUNT] = {
  '.', ':', 'c', 'o', 'O', 'P', '#', '@'};

#define PI 3.14159265358979323846
#define TAU 6.28318530717958647692

#define FOV (PI / 2.0)
#define MAX_VIEW_DISTANCE 6
#define WALL_FADE_START_DISTANCE 4

#define GAME_FPS 60.0
#define FIXED_DELTA_TIME (1.0 / GAME_FPS)

#define PLAYER_MOVE_SPEED 3.0
#define PLAYER_TURN_SPEED 4.0

#endif
