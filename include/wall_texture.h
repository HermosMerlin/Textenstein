#ifndef WALL_TEXTURE_H
#define WALL_TEXTURE_H

#include "config.h"

typedef struct {
    int height;
    int width;
    int pixels[TEXTURE_MAX_SIZE][TEXTURE_MAX_SIZE];
} Texture;

int wall_texture_load(Texture* texture, const char* filename);
double wall_texture_get_factor(const Texture* texture, double wall_x, double wall_y);

#endif