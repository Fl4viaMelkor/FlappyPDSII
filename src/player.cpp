#include <iostream>
#include "../include/player.hpp"
#include "../include/sprite.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include "player_exception.hpp"

//Created By Flávia

Player::Player(const std::string &filename, float x, float y, float speed, float width, float height)
  : RetanguloHitbox({ x, y }, width, height)
  , sprite(filename, 3, 34, 24, 8.0f) // 3 frames, cada uma 34x24, 8FPS.
  , velY(0)
  , speed(speed)
  , gravidade(GRAVIDADE)
  , IsAlive(true)
  , lastJumpTime(0.0f)
{}




void Player::update(const ALLEGRO_KEYBOARD_STATE &key_state)
{
    if (!IsAlive) return;

    float currentTime = al_get_time();
    float deltaTime = currentTime - ultimoTempo;
    ultimoTempo = currentTime;

    // Controle do cooldown no pulo
    if (al_key_down(&key_state, ALLEGRO_KEY_SPACE)) {
        if (currentTime - lastJumpTime >= jumpCooldown) {
            velY = -15 * gravidade;
            lastJumpTime = currentTime;
        }
    }

    velY += gravidade;
    ponto_inferior_esquerdo.y += velY;
    gravidade += 0.0001;

    sprite.update(deltaTime);
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
    //std::cout << "Player foi morto! IsAlive = " << IsAlive << "\n";
    velY = 0;       // Para paralisar o player como no jogo original.
    speed = 0;      // 
}


bool Player::getIsAlive() const {
    return IsAlive;
}

void Player::onCollision(){ 
        Kill();
        throw PlayerException("O jogador morreu.");


}

