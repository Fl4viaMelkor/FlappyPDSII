#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <allegro5/allegro5.h>

class Player {
public:
    float x, y;
    float speed;
    int width, height;

    Player(float x, float y, float speed, int width, int height);

    void update(const ALLEGRO_KEYBOARD_STATE& key_state);
    void draw() const;
};

#endif
