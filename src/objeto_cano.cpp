#include "../include/objeto_cano.hpp"
#include "../include/fisica/hitbox.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

Cano::Cano(float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura,
           const std::string &sprite_path)
  : RetanguloHitbox({ x, 0 }, largura, altura_tela)
  , foiContado(false)
  , m_x(x)
  , m_largura(largura)
  , m_espaco(abertura)
  , m_y_topo(0)
  , // será ajustado logo após
  altura_tela(altura_tela)
  , m_espessura(espessura)
  , m_color(cor)
  , sprite(sprite_path, 1, 52, 320, 1.0f)
  , hitboxTopo({ 0, 0 }, 0, 0)
  ,                           // inicia vazio
  hitboxBaixo({ 0, 0 }, 0, 0) // inicia vazio

{
    std::srand(static_cast<unsigned>(time(nullptr)));
    int min_topo = 50;
    int max_topo = static_cast<int>(altura_tela - abertura - 50);
    m_y_topo = min_topo + rand() % (max_topo - min_topo + 1);

    atualizarHitboxes();
}

void Cano::atualizarHitboxes()
{
    hitboxTopo = RetanguloHitbox({ m_x, m_y_topo - sprite.getHeight() }, m_largura, sprite.getHeight());
    float alturaCanoBaixo = altura_tela - (m_y_topo + m_espaco);
    hitboxBaixo = RetanguloHitbox({ m_x, m_y_topo + m_espaco }, m_largura, alturaCanoBaixo);
}

void Cano::draw() const
{
    sprite.draw({ m_x, m_y_topo - sprite.getHeight() }, true);
    sprite.draw({ m_x, m_y_topo + m_espaco });
}

void Cano::move(float dx)
{
    m_x += dx;
    atualizarHitboxes();
}

void Cano::reset_if_out_of_screen(float limite_esquerdo, float posicao_ultimo_cano, float espacamento,
                                  float altura_tela)
{

    if (m_x + m_largura < limite_esquerdo) {
        m_x = posicao_ultimo_cano + espacamento;

        int min_topo = 50;
        int max_topo = static_cast<int>(altura_tela - m_espaco - 50);
        m_y_topo = min_topo + rand() % (max_topo - min_topo + 1);

        atualizarHitboxes();
        foiContado = false;
    }
}

void Cano::onCollision() { std::cout << "Cano colidido!" << std::endl; }

bool Cano::colisao(coordenadas p)
{
    return hitboxTopo.noInterior(p) || hitboxTopo.noPerimetro(p) || hitboxBaixo.noInterior(p) ||
           hitboxBaixo.noPerimetro(p);
}

bool Cano::noInterior(const coordenadas &p) const { return hitboxTopo.noInterior(p) || hitboxBaixo.noInterior(p); }

bool Cano::noPerimetro(const coordenadas &p) const { return hitboxTopo.noPerimetro(p) || hitboxBaixo.noPerimetro(p); }

std::vector<coordenadas> Cano::get_pontos() const
{
    auto pontosTopo = hitboxTopo.get_pontos();
    auto pontosBaixo = hitboxBaixo.get_pontos();
    pontosTopo.insert(pontosTopo.end(), pontosBaixo.begin(), pontosBaixo.end());
    return pontosTopo;
}
