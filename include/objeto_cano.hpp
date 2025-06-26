#ifndef CANO_H
#define CANO_H

//
#include "sprite.hpp"

// biblioteca padrão
#include "./fisica/hitbox.hpp"

#include <allegro5/allegro.h>
#include <string>

/**
 * @class Cano
 * @brief Representa um par de canos (superior e inferior) como obstáculo no jogo.
 *
 * A classe Cano herda de RetanguloHitbox e possui duas hitboxes internas: uma para o cano superior e outra para o
 * inferior. Ela também gerencia a renderização, movimentação e reposicionamento dos canos na tela.
 */
class Cano : public RetanguloHitbox {
  public:
    /**
     * @brief Construtor do cano.
     * @param x Posição X inicial do cano.
     * @param largura Largura de cada cano (superior e inferior).
     * @param abertura Espaço vertical entre os dois canos.
     * @param altura_tela Altura total da tela, usada para cálculo da posição.
     * @param cor Cor do cano (usada em debug ou renderização alternativa).
     * @param espessura Espessura da borda (caso desenhado com primitivas).
     * @param sprite_path Caminho para o sprite do cano.
     */
    Cano(float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura,
         const std::string &sprite_path);

    /**
     * @brief Desenha os dois canos na tela.
     */
    void draw() const;

    /**
     * @brief Move os canos horizontalmente.
     * @param dx Deslocamento em X.
     */
    void move(float dx);

    /**
     * @brief Reposiciona o cano quando sai da tela, criando o efeito de loop.
     * @param limite_esquerdo Lado esquerdo da tela (quando passar disso, reseta).
     * @param posicao_ultimo_cano Posição X do último cano da fila.
     * @param espacamento Espaçamento horizontal entre canos.
     * @param altura_tela Altura total da tela (para recalcular a abertura).
     */
    void reset_if_out_of_screen(float limite_esquerdo, float posicao_ultimo_cano, float espacamento, float altura_tela);

    /**
     * @brief Ação a ser executada quando ocorre uma colisão com o cano.
     */
    void onCollision() override;

    /**
     * @brief Verifica colisão com um ponto específico.
     * @param p Ponto a ser testado.
     * @return true se o ponto colide com qualquer parte do cano.
     */
    bool colisao(coordenadas p) override;

    /**
     * @brief Retorna a posição X atual do cano.
     * @return Valor de X.
     */

    float getX() const { return m_x; }

    /**
     * @brief Retorna a largura do cano.
     * @return Largura do cano.
     */
    float getLargura() const { return m_largura; }

    /// Indica se esse cano já contou ponto para o jogador.
    bool foiContado;

  private:
    /// Posição X do cano.
    float m_x;

    /// Largura de ambos os canos.
    float m_largura;

    /// Espaço vertical entre o cano superior e o inferior.
    float m_espaco;

    /// Posição Y do topo da abertura entre os canos.
    float m_y_topo;

    /// Altura total da tela.
    float altura_tela;

    /// Espessura da borda do cano (se usado modo primitivo).
    float m_espessura;

    /// Cor do cano.
    ALLEGRO_COLOR m_color;

    /// Sprite animado usado para desenhar o cano.
    SpriteAnimado sprite;

    /// Hitbox do cano superior (virado).
    RetanguloHitbox hitboxTopo;

    /// Hitbox do cano inferior (normal).
    RetanguloHitbox hitboxBaixo;

    /**
     * @brief Atualiza as posições das hitboxes do cano superior e inferior.
     */
    void atualizarHitboxes();

    // Métodos herdados da classe RetanguloHitbox

    /**
     * @brief Verifica se um ponto está no interior do cano.
     */
    bool noInterior(const coordenadas &p) const override;

    /**
     * @brief Verifica se um ponto está no perímetro do cano.
     */
    bool noPerimetro(const coordenadas &p) const override;

    /**
     * @brief Retorna os pontos que definem as duas hitboxes do cano.
     */
    std::vector<coordenadas> get_pontos() const override;
};

#endif