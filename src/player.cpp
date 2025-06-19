#include "../include/player.hpp"
#include "../include/sprite.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

Player::Player(const std::string &filename, float x, float y, float speed, int width, int height)
  : dead(false)
  , velY(0)
  , gravidade(0.5f)
  , x(x)
  , y(y)
  , speed(speed)
  , width(width)
  , height(height)
  , sprite(filename, this->x, this->y)
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
        velY = -5.0f; // força do pulo

    // Gravidade
    velY += gravidade;
    y += velY;
    cout << "Player: " << x << ", " << y << "\n";
    // Pensar no cooldown do pulo **
}

void Player::draw() { sprite.draw(); }

void Player::setDead(bool d) { dead = d; }

bool Player::isDead() const { return dead; }