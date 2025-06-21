#ifndef TELACADASTRO_HPP
#define TELACADASTRO_HPP

#include "tela_base.hpp"
#include "GerenciadorHighScore.hpp"
#include <allegro5/allegro_font.h>
#include <string>
#include <iostream>
#include <stdexcept>

class TelaCadastro : public TelaBase {
private:
    ALLEGRO_FONT* fonte_titulo;
    ALLEGRO_FONT* fonte_input;
    ALLEGRO_FONT* fonte_subtext;

    std::string nome_jogador;
    int pontuacao_final;
    bool entrada_concluida;
    EstadoProximaTela proxima_tela_estado;
//para o cursor piscar
    float cursor_timer;
    bool cursor_visivel;

    GerenciadorHighScores& gerenciador_scores;

    int largura_tela;
    int altura_tela;

public:
    TelaCadastro(int pontuacao, GerenciadorHighScores& manager, int largura, int altura);
    ~TelaCadastro() override;

   
    void update() override;
    void draw() override;
    void step(ALLEGRO_EVENT& evento) override;
    EstadoProximaTela getProximaTelaEstado() const override;
    void resetProximaTelaEstado() override;
};
#endif