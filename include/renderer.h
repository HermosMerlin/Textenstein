#ifndef RENDERER_H
#define RENDERER_H

#include "map.h"
#include "wall_texture.h"

void renderer_render_frame(const Map* map, const Texture* texture, double x, double y, double angle);

#endif