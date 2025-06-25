#include "../include/player.hpp"
#include "../include/sprite.hpp"
#include "player_exception.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include <iostream>

// Created By Flávia

Player::Player(const std::string &filename, float x, float y, float speed, float width, float height)
  : RetanguloHitbox({ x, y }, width, height)
  , sprite(nullptr)
  , velY(0)
  , speed(speed)
  , gravidade(GRAVIDADE)
  , IsAlive(true)
  , lastJumpTime(0.0f)
  , carregouSprite(false)
{
    try {
        sprite = new SpriteAnimado(filename, 3, 24, 24, 8.0f);
        carregouSprite = true;
    }
    catch (const std::runtime_error &e) {
        std::cerr << "Aviso: " << e.what() << std::endl;
        carregouSprite = false;
    }
}

Player::~Player() { delete sprite; }

void Player::update(const ALLEGRO_KEYBOARD_STATE &key_state)
{
    if (!IsAlive)
        return;

    float currentTime = al_get_time();
    float deltaTime = currentTime - ultimoTempo;
    ultimoTempo = currentTime;

    if (al_key_down(&key_state, ALLEGRO_KEY_SPACE)) {
        if (currentTime - lastJumpTime >= jumpCooldown) {
            velY = -15 * gravidade;
            lastJumpTime = currentTime;
        }
    }

    velY += gravidade;
    ponto_inferior_esquerdo.y += velY;
    gravidade += 0.0001;
    jumpCooldown -= 0.0001;

    if (carregouSprite && sprite)
        sprite->update(deltaTime);
}

void Player::draw()
{
    if (carregouSprite && sprite) {
        sprite->draw(ponto_inferior_esquerdo, false);
    }
    else {
        // Desenha retângulo vermelho se sprite não carregou
        al_draw_filled_rectangle(ponto_inferior_esquerdo.x, ponto_inferior_esquerdo.y, ponto_inferior_esquerdo.x + base,
                                 ponto_inferior_esquerdo.y + altura, al_map_rgb(255, 0, 0));
    }
}

bool Player::colisao(coordenadas p) { return noPerimetro(p) || noInterior(p); }

void Player::Kill()
{
    IsAlive = false;
    velY = 0;
    speed = 0;
}

bool Player::getIsAlive() const { return IsAlive; }

void Player::onCollision() { Kill(); }
