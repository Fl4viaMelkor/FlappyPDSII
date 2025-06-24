#include "../include/objeto_cano.hpp"
#include "../include/config.hpp"

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h> // Para al_map_rgb
#include <allegro5/allegro_primitives.h>

using namespace std;

#include <stdexcept>

Cano::Cano(float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura, const std::string& sprite_path)
  : m_x(x)
  , m_largura(largura)
  , m_espaco(abertura)
  , m_color(cor)
  , m_espessura(espessura)
  , sprite(sprite_path, 1, 52, 320, 1.0f) // 1 frame, 52 largura, 320 altura, FPS não importa
{
    m_y_topo = rand() % int(altura_tela - abertura - 100);
    foiContado = false;
}



// Desenha o par de canos
// (superior e inferior)

void Cano::draw() const {
    float yBase = m_y_topo + m_espaco;

    // Desenha cano superior: desenha com flip vertical e desloca para cima pela altura do sprite
    sprite.draw({m_x, m_y_topo - sprite.getHeight()}, true);

    // Desenha cano inferior normal, começando em yBase
    sprite.draw({m_x, yBase});
}




// Move o cano
// horizontalmente (dx
// negativo move para
// esquerda)
void Cano::move(float dx) { m_x += dx; }


/**
 * @brief Reposiciona o cano se ele sair completamente da tela pela esquerda.
 *
 * Este método verifica se o cano ultrapassou o limite esquerdo da tela
 * (ou seja, saiu completamente da área visível). Se isso acontecer,
 * ele reposiciona o cano para a direita da tela com base na posição
 * de um cano anterior e aplica uma nova altura aleatória.
 */

void Cano::reset_if_out_of_screen(float limite_esquerdo, float posicao_ultimo_cano, float espacamento,
                                  float altura_tela)
{

    // Verifica se a
    // borda direita do
    // cano está à
    // esquerda do limite
    // (cano saiu da
    // tela)
    if (m_x + m_largura < limite_esquerdo) {

        // Reposiciona o
        // cano para a
        // direita do
        // último cano +
        // espaçamento
        m_x = posicao_ultimo_cano + espacamento;

        // Define limites
        // para altura
        // aleatória do
        // cano superior,
        // evitando
        // extremos da
        // tela
        int min_topo = 50;
        int max_topo = int(altura_tela - m_espaco - 50);

        // Gera altura
        // aleatória para
        // o cano
        // superior
        // dentro do
        // intervalo
        m_y_topo = min_topo + rand() % (max_topo - min_topo + 1);
        foiContado = false;
    }
}


