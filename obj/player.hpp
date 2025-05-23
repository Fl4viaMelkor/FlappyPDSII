// player.hpp
#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>

class Player {
private:
    float x, y;
    float speed;
    ALLEGRO_COLOR color;

public:
    Player(float startX, float startY);

    void move(float dx, float dy);
    void draw();
};

#endif
