#include "../include/player.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include "../include/sprite.hpp"

Player::Player(const std::string& filename,float x, float y, float speed, int width, int height)
    : sprite(filename),dead(false), velY(0), gravidade(0.5f), noChao(false), x(x), y(y), speed(speed), width(width), height(height) {}

void Player::update(const ALLEGRO_KEYBOARD_STATE& key_state) {
    if (al_key_down(&key_state, ALLEGRO_KEY_UP))    y -= speed;
    if (al_key_down(&key_state, ALLEGRO_KEY_DOWN))  y += speed;


    // Pulo
    if (al_key_down(&key_state, ALLEGRO_KEY_SPACE)) {
        velY = -5.0f;  // força do pulo

    }

    // Gravidade
    velY += gravidade;
    y += velY;

    //Pensar no cooldown do pulo **
}

void Player::draw() const {
    sprite.draw(x,y);
    //al_draw_filled_rectangle(x, y, x + width, y + height, al_map_rgb(255, 0, 0));
}


void Player::setDead(bool d) {
    dead = d;
}

bool Player::isDead() const {
    return dead;
}