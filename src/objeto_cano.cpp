#include "../include/objeto_cano.hpp"

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h> // Para al_map_rgb
#include <allegro5/allegro_primitives.h>

using namespace std;

Cano::Cano(float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura)
  : m_x(x)
  , m_largura(largura)
  , m_espaco(abertura)
  , m_color(cor)
  , m_espessura(espessura)
{

    // Inicializa a
    // posição vertical
    // do cano superior
    // aleatoriamente no
    // construtor
    m_y_topo = rand() % int(altura_tela - abertura - 100);
}

// Desenha o par de canos
// (superior e inferior)
void Cano::draw() const
{
    float m_y_base = m_y_topo + m_espaco; // posição
                                          // inicial
                                          // do
                                          // cano
                                          // inferior

    // Desenha bloco
    // superior (do topo
    // da tela até
    // m_y_topo)
    al_draw_filled_rectangle(m_x, 0, m_x + m_largura, m_y_topo, m_color);

    // Desenha bloco
    // inferior (de
    // m_y_base até o fim
    // da tela)
    al_draw_filled_rectangle(m_x, m_y_base, m_x + m_largura, 600, m_color);
}

// Move o cano
// horizontalmente (dx
// negativo move para
// esquerda)
void Cano::move(float dx) { m_x += dx; }

// Se o cano saiu
// completamente da tela
// à esquerda,
// reposiciona ele
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
    }
}

bool Cano::colisao_com_jogador(float jogador_x, float jogador_y, float jogador_largura, float jogador_altura) const
{

    // Define retângulos
    // dos dois blocos do
    // cano
    float cano_sup_x1 = m_x;
    float cano_sup_y1 = 0;
    float cano_sup_x2 = m_x + m_largura;
    float cano_sup_y2 = m_y_topo;

    float cano_inf_x1 = m_x;
    float cano_inf_y1 = m_y_topo + m_espaco;
    float cano_inf_x2 = m_x + m_largura;
    float cano_inf_y2 = 600; // altura da
                             // tela, pode
                             // ser passado
                             // como
                             // parâmetro

    // Define retângulo do jogador
    float jog_x1 = jogador_x;
    float jog_y1 = jogador_y;
    float jog_x2 = jogador_x + jogador_largura;
    float jog_y2 = jogador_y + jogador_altura;

    // Função auxiliar
    // para detectar    // interseção entre
    // retângulos
    auto retangulos_intersectam = [](float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2,
                                     float by2) { return (ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1); };

    // Verifica colisão
    // com bloco superior
    if (retangulos_intersectam(cano_sup_x1, cano_sup_y1, cano_sup_x2, cano_sup_y2, jog_x1, jog_y1, jog_x2, jog_y2))
        return true;

    // Verifica colisão
    // com bloco inferior
    if (retangulos_intersectam(cano_inf_x1, cano_inf_y1, cano_inf_x2, cano_inf_y2, jog_x1, jog_y1, jog_x2, jog_y2))

        return true;

    // Sem colisão
    return false;
}
