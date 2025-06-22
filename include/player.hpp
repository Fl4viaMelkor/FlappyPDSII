#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "./fisica/hitbox.hpp"

#include "sprite.hpp"
#include "util.hpp"
#include <allegro5/allegro5.h>

//Created by Flávia

class Player : public RetanguloHitbox {
  private:
    SpriteAnimado sprite;; // Sprite animado.
    float velY;    // Velocidade vertical
    float speed;
    float gravidade;
    bool IsAlive;
    float lastJumpTime = 0.0f;   // Tempo do último pulo
    const float jumpCooldown = 0.3f;  // Meio segundo de cooldown no pulo
    float ultimoTempo = 0.0f;


  public:
    Player(const std::string &filename, float x, float y, float speed, float width, float height);

    void update(const ALLEGRO_KEYBOARD_STATE &key_state);
    void draw();
    bool colisao(coordenadas p) override;
    void onCollision() override;
    void Kill();
    bool getIsAlive() const;

    // Hitbox *getHitbox() const override;
    // CollisionInfo getInfo() override;
    // void onCollision() override;
    // ~Player() override;
};

#endif
