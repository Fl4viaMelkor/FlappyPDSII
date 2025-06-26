#ifndef CANO_H
#define CANO_H

//
#include "sprite.hpp"

// biblioteca padrão
#include <allegro5/allegro.h>

// modulo para desenhar quadrado
#include <allegro5/allegro_primitives.h>

using namespace std;

#include <vector>

// para detecção de colisão
#include "fisica/hitbox.hpp"

// herdar RetanguloHitbox
// class Cano : public RetanguloHitbox {

class Cano : public RetanguloHitboxAbstract {
  public:
    /**
     * @brief Construtor do cano.
     * @param x Posição horizontal do cano.
     * @param largura Largura dos blocos dos canos.
     * @param abertura Espaço vertical entre os canos.
     * @param altura_tela Altura total da janela/tela.
     * @param cor Cor dos blocos dos canos.
     * @param espessura Espessura do contorno (linha) do cano.
     * @param sprite_path Caminho para o arquivo de imagem do sprite.
     */

    Cano(float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura,
         const std::string &sprite_path);

    void draw() const;   // Desenha os canos (superior e inferior)
    void move(float dx); // Move os canos na horizontal (dx pode ser negativo para mover para esquerda)

    /**
     * @brief Verifica se o cano saiu totalmente da tela pela esquerda e o reposiciona.
     * @param limite_esquerdo Posição do limite esquerdo da tela.
     * @param posicao_ultimo_cano Posição x do último cano (para reposicionar após ele).
     * @param espacamento Espaço entre os canos.
     * @param altura_tela Altura da janela do jogo.
     */

    void reset_if_out_of_screen(float limite_esquerdo, float posicao_ultimo_cano, float espacamento, float altura_tela);

    // Getters para pegar posição e largura (úteis para cálculo de reposicionamento)
    float getX() const { return m_x; }
    float getLargura() const { return m_largura; }
    void onCollision() override;
    bool colisao(coordenadas p) override;

    bool foiContado;

    /// implementação da hitbox
    // bool colisao_com_jogador(float jogador_x, float jogador_y, float jogador_largura, float jogador_altura) const;

  private:
    float m_x;             // Posição horizontal do cano (para ambos os blocos)
    float m_largura;       // Largura dos blocos (constante)
    float m_espaco;        // Espaço vertical entre os blocos (abertura)
    float m_y_topo;        // Altura do bloco superior (variável para fazer o espaço aleatório)
    ALLEGRO_COLOR m_color; // Cor dos blocos
    float m_espessura;     // Espessura da linha (não usada para preenchidos, mas mantida)

    SpriteAnimado sprite; // NOVO

    // contagem de pontuação no caso em que o player passa pelo cano

    // para contar a pontuação
};

#endif