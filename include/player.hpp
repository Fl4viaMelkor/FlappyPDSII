#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <allegro5/allegro5.h>
#include "util.hpp"
#include "sprite.hpp"

class Player {
private:
    Sprite sprite;  //Ver se roda um sprite.
    bool dead;
    float velY;    // Velocidade vertical
    float gravidade; //
    bool noChao;   //


public:
    float x, y;
    float speed;
    int width, height;

    Player(const std::string& filename, float x, float y, float speed, int width, int height);

    void update(const ALLEGRO_KEYBOARD_STATE& key_state);
    void draw() const;

    void setDead(bool d);
    bool isDead() const;

};

#endif
