#include "../include/player.hpp"
#include "../include/sprite.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

Player::Player(const std::string &filename, float x, float y, float speed, int width, int height)
  : velY(0)
  , x(x)
  , y(y)
  , speed(speed)
  , width(width)
  , height(height)
  , sprite(filename, this->x, this->y)
  , gravidade(GRAVIDADE)
{
}

void Player::update(const ALLEGRO_KEYBOARD_STATE &key_state)
{
    if (al_key_down(&key_state, ALLEGRO_KEY_UP))
        y -= speed;
    if (al_key_down(&key_state, ALLEGRO_KEY_DOWN))
        y += speed;

    // Pulo
    if (al_key_down(&key_state, ALLEGRO_KEY_SPACE))
        velY = -10 * gravidade; // força do pulo

    // Gravidade
    velY += gravidade;
    y += velY;

    // Jogo gradativamente mais rápido e difícil
    gravidade += 0.0001;
    // Pensar no cooldown do pulo **
}

void Player::draw() { sprite.draw(); }
