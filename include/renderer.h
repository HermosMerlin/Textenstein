#ifndef RENDERER_H
#define RENDERER_H

#include "map.h"

void renderer_windowcolumn_debug(const Map* map,double x,double y,double angle);
void renderer_render_frame(const Map* map,double x, double y, double angle);

#endif