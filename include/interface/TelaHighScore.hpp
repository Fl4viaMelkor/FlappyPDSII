#ifndef TELAHIGHSCORE_HPP
#define TELAHIGHSCORE_HPP

#include "tela_base.hpp"
#include "GerenciadorHighScore.hpp"
#include <allegro5/allegro_font.h>
#include <string>
#include <vector>

class TelaHighScores : public TelaBase {
private:
    ALLEGRO_FONT* fonte_titulo;
    ALLEGRO_FONT* fonte_scores;
    GerenciadorHighScores& gerenciador_scores;
    EstadoProximaTela proxima_tela_estado;
    int largura_tela, altura_tela;

public:
    TelaHighScores(GerenciadorHighScores& manager, int largura, int altura);
    ~TelaHighScores() override;

    void update() override;
    void draw() override;
    void step(ALLEGRO_EVENT& evento) override;
    EstadoProximaTela getProximaTelaEstado() const override;
    void resetProximaTelaEstado() override;
};
#endif