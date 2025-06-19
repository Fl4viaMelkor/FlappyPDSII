#include "../include/player.hpp"
#include "../include/sprite.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

Player::Player(const std::string &filename, float x, float y, float speed, float width, float height)
  : RetanguloHitbox({ x, y }, width, height)
  , velY(0)
  , speed(speed)
  , sprite(filename, this->ponto_inferior_esquerdo)
  , gravidade(GRAVIDADE)

{
}

void Player::update(const ALLEGRO_KEYBOARD_STATE &key_state)
{
    if (al_key_down(&key_state, ALLEGRO_KEY_UP))
        ponto_inferior_esquerdo.y -= speed;
    if (al_key_down(&key_state, ALLEGRO_KEY_DOWN))
        ponto_inferior_esquerdo.y += speed;

    // Pulo
    if (al_key_down(&key_state, ALLEGRO_KEY_SPACE))
        velY = -10 * gravidade; // força do pulo

    // Gravidade
    velY += gravidade;
    ponto_inferior_esquerdo.y += velY;

    // Jogo gradativamente mais rápido e difícil
    gravidade += 0.0001;
    // Pensar no cooldown do pulo **
}

void Player::draw() { sprite.draw(); }
bool Player::colisao(coordenadas p)
{
    // cout << "Ponto p:" << p.x << "," << p.y << endl;
    // cout << "no perimitro" << noPerimetro(p) << endl;
    // cout << "no interior" << noInterior(p) << endl;
    return noPerimetro(p) || noInterior(p);
}
void Player::onCollision() { cout << "Aconteceu uma colisão no jogador"; }
