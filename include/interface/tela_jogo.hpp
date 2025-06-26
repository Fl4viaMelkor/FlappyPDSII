#ifndef TELAJOGO_HPP
#define TELAJOGO_HPP

#include "../fisica/fisica.hpp"
#include "../objeto_cano.hpp"
#include "../player.hpp"
#include "../util.hpp"
#include "parallax_background.hpp"
#include "tela_base.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>

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

//

enum class GameState {
    PAUSED_FOR_START, // Pausado aguardando o input do jogador para começar
    PLAYING,          // Jogo em andamento
    GAME_OVER
};

class TelaJogo : public TelaBase, public RetanguloHitboxAbstract {
  private:
    gravidade g{ 2, 3 };
    Player *player;

    // adicionando vector
    std::vector<Cano *> canos; // substitui MySquare

    // Cano *MySquare; *trocado pelo vector

    ALLEGRO_KEYBOARD_STATE keyState;
    ParallaxBackground *parallaxBg;
    void initializeRandomBackgroud();
    bool end; // Possível controlador para fim da tela

    Detector_Colisao *detector;

    GameState currentGameState;
    ALLEGRO_FONT *font;

    ALLEGRO_AUDIO_STREAM *musica_jogo;

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
    void onCollision() override
    {
        std::cout << "!!! COLISÃO COM A TELA !!!" << std::endl;
        this->end = true;
    }
    int pontos;
};

#endif
