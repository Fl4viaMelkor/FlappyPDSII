#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "./fisica/hitbox.hpp"

#include "sprite.hpp"
#include "util.hpp"
#include <allegro5/allegro5.h>

class Player {
  private:
    Sprite sprite; // Ver se roda um sprite.
    bool dead;
    float velY;      // Velocidade vertical
    float gravidade; //

  public:
    float x, y;
    float speed;
    int width, height;

    Player(const std::string &filename, float x, float y, float speed, int width, int height);

    void update(const ALLEGRO_KEYBOARD_STATE &key_state);
    void draw();

    void setDead(bool d);
    bool isDead() const;
    // Hitbox *getHitbox() const override;
    // CollisionInfo getInfo() override;
    // void onCollision() override;
    // ~Player() override;
};

#endif
