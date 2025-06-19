#ifndef TELAJOGO_HPP
#define TELAJOGO_HPP

#include "../objeto_cano.hpp"
#include "../player.hpp"
#include "../util.hpp"
#include "./fisica/fisica.hpp"
#include "parallax_background.hpp"
#include "tela_base.hpp"
#include <allegro5/allegro5.h>

// Caso algum item ou coisa seja adicionado na tela de JOGO deve criar um ponteiro para esse objeto no PRIVATE

// class TelaJogo : public TelaBase {
// private:
//     Player* player;
//
//     Cano* MySquare; //teste
//     ALLEGRO_KEYBOARD_STATE keyState;
//     bool end;   //Possível controlador para fim da tela
//
// public:
//     TelaJogo();
//     ~TelaJogo();
//
//     void update() override;
//     void draw() override;
//     void step(ALLEGRO_EVENT& evento) override;
//
//     bool acabouJogo() const;
// };

// Exemplo
class TelaJogo : public TelaBase, public RetanguloHitbox {
  private:
    gravidade g{ 2, 3 };
    Player *player;

    Cano *MySquare; // teste
    ALLEGRO_KEYBOARD_STATE keyState;
    ParallaxBackground *parallaxBg;
    bool end; // Possível controlador para fim da tela

    Detector_Colisao *detector;

  public:
    TelaJogo();
    ~TelaJogo() override;

    void update() override;
    void draw() override;
    void step(ALLEGRO_EVENT &evento) override;

    bool acabouJogo() const;
    EstadoProximaTela getProximaTelaEstado() const override;
    void resetEstado() override;
    void resetProximaTelaEstado() override;

    bool colisao(coordenadas p) override { return !noInterior(p); }
    void onCollision() override { cout << "Colisão aconteceu " << endl; }
};

#endif
