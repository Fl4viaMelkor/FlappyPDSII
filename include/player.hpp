#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "./fisica/hitbox.hpp"

#include "sprite.hpp"
#include "util.hpp"
#include <allegro5/allegro5.h>

class Player {
  private:
    Sprite sprite; // Ver se roda um sprite.
    float velY;    // Velocidade vertical
    float x, y;
    float speed;
    int width, height;
    float gravidade;

  public:
    Player(const std::string &filename, float x, float y, float speed, int width, int height);

    void update(const ALLEGRO_KEYBOARD_STATE &key_state);
    void draw();
    // Hitbox *getHitbox() const override;
    // CollisionInfo getInfo() override;
    // void onCollision() override;
    // ~Player() override;
};

#endif
