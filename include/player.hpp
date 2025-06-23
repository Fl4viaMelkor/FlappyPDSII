#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "./fisica/hitbox.hpp"
#include "sprite.hpp"
#include "util.hpp"
#include <allegro5/allegro5.h>

// Created by Flávia

// Classe que representa o jogador no jogo
class Player : public RetanguloHitbox {
private:
    SpriteAnimado sprite;      // Sprite animado do jogador
    float velY;                // Velocidade vertical do jogador
    float speed;               // Velocidade horizontal (se usada)
    float gravidade;           // Gravidade aplicada ao jogador
    bool IsAlive;              // Estado de vida do jogador
    float lastJumpTime = 0.0f; // Tempo do último pulo
    const float jumpCooldown = 0.3f; // Cooldown de pulo: 0.3 segundos
    float ultimoTempo = 0.0f;  // Armazena tempo de atualização anterior

public:
    // Construtor: inicializa o jogador com sprite e atributos físicos
    Player(const std::string &filename, float x, float y, float speed, float width, float height);

    // Atualiza o estado do jogador baseado no input do teclado
    void update(const ALLEGRO_KEYBOARD_STATE &key_state);

    // Desenha o jogador na tela
    void draw();

    // Verifica colisão com um ponto e retorna true se colidir
    bool colisao(coordenadas p) override;

    // Reação do jogador a uma colisão (pode ser matar o jogador, rebater, etc.)
    void onCollision() override;

    // Mata o jogador
    void Kill();

    // Retorna se o jogador ainda está vivo
    bool getIsAlive() const;

    // Comentados: métodos herdados da hitbox que podem ser implementados depois
    // Hitbox *getHitbox() const override;
    // CollisionInfo getInfo() override;
    // void onCollision() override;
    // ~Player() override;
};

#endif
