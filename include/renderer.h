#ifndef RENDERER_H
#define RENDERER_H

#include "map.h"
#include "wall_texture.h"
#include "player.h"
#include "frame_buffer.h"

void renderer_render_frame(const Map* map, const Texture* texture, const Player* player,FrameBuffer* fb);

#endif