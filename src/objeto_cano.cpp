#include "../include/objeto_cano.hpp"
#include <cstdlib>
#include <iostream>

Cano::Cano(float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura, const std::string& sprite_path)
    : RetanguloHitbox({ x, 0 }, largura, altura_tela),  // Hitbox configurada depois
      m_x(x),
      m_largura(largura),
      m_espaco(abertura),
      m_color(cor),
      m_espessura(espessura),
      sprite(sprite_path, 1, 52, 320, 1.0f)
{
    m_y_topo = rand() % int(altura_tela - abertura - 100);
    foiContado = false;

    // Define a hitbox para o cano inferior
    ponto_inferior_esquerdo = { m_x, m_y_topo + m_espaco };
    altura = altura_tela - ponto_inferior_esquerdo.y;
}

void Cano::draw() const {
    float yBase = m_y_topo + m_espaco;

    // Desenha cano superior com flip vertical
    sprite.draw({ m_x, m_y_topo - sprite.getHeight() }, true);

    // Desenha cano inferior
    sprite.draw({ m_x, yBase });
}

void Cano::move(float dx) {
    m_x += dx;
    ponto_inferior_esquerdo.x += dx; // move a hitbox junto
}

void Cano::reset_if_out_of_screen(float limite_esquerdo, float posicao_ultimo_cano, float espacamento, float altura_tela) {
    if (m_x + m_largura < limite_esquerdo) {
        m_x = posicao_ultimo_cano + espacamento;

        int min_topo = 50;
        int max_topo = int(altura_tela - m_espaco - 50);
        m_y_topo = min_topo + rand() % (max_topo - min_topo + 1);

        ponto_inferior_esquerdo = { m_x, m_y_topo + m_espaco };
        altura = altura_tela - ponto_inferior_esquerdo.y;

        foiContado = false;
    }
}

void Cano::onCollision() {
    std::cout << "Cano colidido!\n";
}

bool Cano::colisao(coordenadas p) {
    return noInterior(p) || noPerimetro(p);
}
