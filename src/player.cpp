#include <iostream>
#include "../include/player.hpp"
#include "../include/sprite.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include "player_exception.hpp"

Player::Player(const std::string &filename, float x, float y, float speed, float width, float height)
  : RetanguloHitbox({ x, y }, width, height)
  , velY(0)
  , speed(speed)
  , sprite(filename) // <-- Agora sem a posição
  , gravidade(GRAVIDADE)
  , IsAlive(true)
{
}



void Player::update(const ALLEGRO_KEYBOARD_STATE &key_state)
{
        /* debbug
    if (!IsAlive) {
        std::cout << "Player morto: update bloqueado\n";
        return;
    }

    std::cout << "Player vivo: update acontecendo\n"; */

    if (!IsAlive) return;

    float currentTime = al_get_time();

    if (al_key_down(&key_state, ALLEGRO_KEY_UP))
        ponto_inferior_esquerdo.y -= speed;
    if (al_key_down(&key_state, ALLEGRO_KEY_DOWN))
        ponto_inferior_esquerdo.y += speed;

    // Controle do cooldown no pulo
    if (al_key_down(&key_state, ALLEGRO_KEY_SPACE)) {
        if (currentTime - lastJumpTime >= jumpCooldown) {
            velY = -15 * gravidade; // pulo
            lastJumpTime = currentTime;
        }
    }

    velY += gravidade;
    ponto_inferior_esquerdo.y += velY;

    gravidade += 0.0001;
}


void Player::draw() {
    sprite.draw(ponto_inferior_esquerdo); // passa a posição atual
}



bool Player::colisao(coordenadas p)
{
    // cout << "Ponto p:" << p.x << "," << p.y << endl;
    // cout << "no perimitro" << noPerimetro(p) << endl;
    // cout << "no interior" << noInterior(p) << endl;
    return noPerimetro(p) || noInterior(p);
}


void Player::Kill() {
    IsAlive = false;
    std::cout << "Player foi morto! IsAlive = " << IsAlive << "\n";
}


bool Player::getIsAlive() const {
    return IsAlive;
}

void Player::onCollision(){ 
        Kill();
        throw PlayerException("O jogador morreu.");


}

