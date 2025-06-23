#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "./fisica/hitbox.hpp"
#include "sprite.hpp"
#include "util.hpp"
#include <allegro5/allegro5.h>

/**
 * @brief Classe que representa o jogador no jogo.
 * 
 * Herda RetanguloHitbox para permitir detecção de colisão.
 */
class Player : public RetanguloHitbox {
private:
    SpriteAnimado* sprite;     /**< Ponteiro para o sprite animado do jogador */
    float velY;                /**< Velocidade vertical do jogador */
    float speed;               /**< Velocidade horizontal do jogador */
    float gravidade;           /**< Gravidade aplicada ao jogador */
    bool IsAlive;              /**< Estado de vida do jogador */
    float lastJumpTime;        /**< Tempo do último pulo */
    const float jumpCooldown = 0.3f; /**< Cooldown entre pulos (segundos) */
    float ultimoTempo = 0.0f;  /**< Tempo da última atualização */
    bool carregouSprite;       /**< Indica se o sprite foi carregado com sucesso */

public:
    /**
     * @brief Construtor do jogador.
     * 
     * @param filename Caminho da sprite sheet.
     * @param x Posição inicial X.
     * @param y Posição inicial Y.
     * @param speed Velocidade horizontal.
     * @param width Largura do hitbox.
     * @param height Altura do hitbox.
     */
    Player(const std::string &filename, float x, float y, float speed, float width, float height);

    /**
     * @brief Destrutor do jogador, libera recursos.
     */
    ~Player();

    /**
     * @brief Atualiza o estado do jogador conforme input e física.
     * 
     * @param key_state Estado atual do teclado.
     */
    void update(const ALLEGRO_KEYBOARD_STATE &key_state);

    /**
     * @brief Desenha o jogador na tela.
     */
    void draw();

    /**
     * @brief Verifica colisão com ponto.
     * 
     * @param p Coordenadas do ponto.
     * @return true Se colidir com a hitbox.
     * @return false Caso contrário.
     */
    bool colisao(coordenadas p) override;

    /**
     * @brief Mata o jogador.
     */
    void Kill();

    /**
     * @brief Retorna se o jogador está vivo.
     * 
     * @return true Se vivo.
     * @return false Se morto.
     */
    bool getIsAlive() const;

    /**
     * @brief Função chamada quando ocorre colisão.
     * 
     * Lança exceção PlayerException para indicar morte.
     */
    void onCollision() override;
};

#endif
