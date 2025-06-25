#ifndef CANO_H
#define CANO_H

#include "sprite.hpp"
#include "fisica/hitbox.hpp"

#include <allegro5/allegro.h>
#include <string>

class Cano : public RetanguloHitbox {
public:
    Cano(float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura, const std::string& sprite_path);

    void draw() const;
    void move(float dx);
    void reset_if_out_of_screen(float limite_esquerdo, float posicao_ultimo_cano, float espacamento, float altura_tela);

    void onCollision() override;
    bool colisao(coordenadas p) override;

    float getX() const { return m_x; }
    float getLargura() const { return m_largura; }

private:
    float m_x;
    float m_largura;
    float m_espaco;
    float m_y_topo;
    float altura_tela;
    float m_espessura;
    ALLEGRO_COLOR m_color;

    SpriteAnimado sprite;
    RetanguloHitbox hitboxTopo;
    RetanguloHitbox hitboxBaixo;
    bool foiContado;

    void atualizarHitboxes();

    // Implementações herdadas de RetanguloHitbox
    bool noInterior(const coordenadas &p) const override;
    bool noPerimetro(const coordenadas &p) const override;
    std::vector<coordenadas> get_pontos() const override;
};

#endif
