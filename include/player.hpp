#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <allegro5/allegro5.h>
#include "util.hpp"

class Player {
private:
    bool dead;
    float velY;    // Velocidade vertical
    float gravidade; //
    bool noChao;   //


public:
    float x, y;
    float speed;
    int width, height;

    Player(float x, float y, float speed, int width, int height);

    void update(const ALLEGRO_KEYBOARD_STATE& key_state);
    void draw() const;

    void setDead(bool d);
    bool isDead() const;

};

#endif
