#ifndef TELAFIMDEJOGO_HPP
#define TELAFIMDEJOGO_HPP

#include "tela_base.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <iostream>

// Escolhas do jogador


class tela_fimdejogo : public TelaBase {
    private:
        ALLEGRO_FONT *fonte_titulo;
        ALLEGRO_FONT *fonte_texto;
        ALLEGRO_BITMAP *imagem_fundo;

        int pontuacao_final;
        EstadoProximaTela proxima_tela_estado; // Qual ação o jogador escolheu

        // Cores para o texto e elementos
        ALLEGRO_COLOR cor_titulo;
        ALLEGRO_COLOR cor_texto_pontuacao;
        ALLEGRO_COLOR cor_texto_opcoes;


        // Dimensões da tela (para centralizar o texto e elementos)
        int largura_tela;
        int altura_tela;

    public:
        // Construtor
        tela_fimdejogo(int pontuacao, int largura_tela_jogo, int altura_tela_jogo);

        // Destrutor
        ~tela_fimdejogo() override;

        void update() override;
        void draw() override;
        void step(ALLEGRO_EVENT &evento) override;
        EstadoProximaTela getProximaTelaEstado() const override;
};

#endif
