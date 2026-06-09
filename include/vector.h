#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>
typedef struct {
    int x;
    int y;
} Vec2i;

typedef struct {
    double x;
    double y;
} Vec2d;

static inline Vec2d vec2d_add(Vec2d a, Vec2d b) {
    return (Vec2d){a.x + b.x, a.y + b.y};
}

static inline Vec2i vec2i_add(Vec2i a, Vec2i b) {
    return (Vec2i){a.x + b.x, a.y + b.y};
}

static inline Vec2d vec2d_scale(Vec2d v, double s) {
    return (Vec2d){v.x * s, v.y * s};
}

static inline double vec2d_length(Vec2d v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

static inline Vec2d vec2d_normalize(Vec2d v) {
    if (v.x == 0 && v.y == 0)
        return (Vec2d){0, 0};
    return (Vec2d){v.x / vec2d_length(v), v.y / vec2d_length(v)};
}

static inline double vec2d_dot(Vec2d a, Vec2d b) {
    return ((a.x * b.x) + (a.y * b.y));
}

static inline Vec2d vec2d_angle_to_vector(double angle) {
    double dir_x = cos(angle);
    double dir_y = sin(angle);
    return ((Vec2d){dir_x,dir_y});
}

#endif