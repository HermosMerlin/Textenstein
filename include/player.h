#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    double x;
    double y;
    double angle;
    double move_speed;
    double turn_speed;
} Player;

void player_init(Player* player, double x, double y, double angle);

#endif