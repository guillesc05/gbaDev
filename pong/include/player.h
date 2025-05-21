#ifndef GAME_H
#define GAME_H

struct Entity
{
    int x;
    int y;
    int radius;
    int color;

    int velX;
    int velY;

    int lastX;
    int lastY;
    int lastRad;

};
#endif