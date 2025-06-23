#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "./fisica/hitbox.hpp"
#include "sprite.hpp"
#include "util.hpp"
#include <allegro5/allegro5.h>

/**
 * @brief Classe que representa o jogador no jogo.
 * 
 * Herda de RetanguloHitbox para ter propriedades de colisão.
 * Gerencia sprite animado, movimentação vertical, pulos e estado de vida.
 */
class Player : public RetanguloHitbox {
private:
    SpriteAnimado sprite;      ///< Sprite animado do jogador.
    float velY;                ///< Velocidade vertical do jogador.
    float speed;               ///< Velocidade horizontal do jogador (pode ser usada futuramente).
    float gravidade;           ///< Gravidade aplicada ao jogador.
    bool IsAlive;              ///< Indica se o jogador está vivo.
    float lastJumpTime = 0.0f; ///< Tempo do último pulo.
    const float jumpCooldown = 0.3f; ///< Tempo mínimo entre pulos, em segundos.
    float ultimoTempo = 0.0f;  ///< Marca o tempo da última atualização.

public:
    /**
     * @brief Construtor da classe Player.
     * 
     * @param filename Caminho do arquivo de sprite.
     * @param x Posição X inicial.
     * @param y Posição Y inicial.
     * @param speed Velocidade horizontal.
     * @param width Largura da hitbox.
     * @param height Altura da hitbox.
     */
    Player(const std::string &filename, float x, float y, float speed, float width, float height);

    /**
     * @brief Atualiza o estado do jogador com base nas teclas pressionadas.
     * 
     * @param key_state Estado atual do teclado.
     */
    void update(const ALLEGRO_KEYBOARD_STATE &key_state);

    /**
     * @brief Desenha o jogador na tela.
     */
    void draw();

    /**
     * @brief Verifica se há colisão entre o jogador e um ponto.
     * 
     * @param p Ponto a ser testado.
     * @return true Se houver colisão.
     * @return false Caso contrário.
     */
    bool colisao(coordenadas p) override;

    /**
     * @brief Reação do jogador a uma colisão.
     */
    void onCollision() override;

    /**
     * @brief Mata o jogador (altera o estado interno para "não vivo").
     */
    void Kill();

    /**
     * @brief Retorna se o jogador ainda está vivo.
     * 
     * @return true Se estiver vivo.
     * @return false Se estiver morto.
     */
    bool getIsAlive() const;

    // Métodos herdados de Hitbox podem ser implementados futuramente:
    // Hitbox *getHitbox() const override;
    // CollisionInfo getInfo() override;
    // void onCollision() override;
    // ~Player() override;
};

#endif
