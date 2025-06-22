#ifndef TELAINICIAL_HPP
#define TELAINICIAL_HPP

#include "tela_base.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <string>

class TelaInicial : public TelaBase {
private:
    ALLEGRO_FONT* fonte_titulo;
    ALLEGRO_FONT* fonte_opcoes;
    ALLEGRO_BITMAP* imagem_fundo; 
    ALLEGRO_BITMAP* animacao_spritesheet; //animação
    int anim_frame_largura;      
    int anim_frame_atual;        
    float anim_contador_tempo;   
    float anim_delay_frame;      

    std::vector<std::string> opcoes_menu;
    int opcao_selecionada_idx;

    EstadoProximaTela estado_escolhido;

    // Cores
    ALLEGRO_COLOR cor_titulo;
    ALLEGRO_COLOR cor_opcao_normal;
    ALLEGRO_COLOR cor_opcao_selecionada;

    int largura_tela;
    int altura_tela;

public:
    TelaInicial(int largura, int altura);
    ~TelaInicial() override;

    // Métodos obrigatórios da TelaBase
    void update() override;
    void draw() override;
    void step(ALLEGRO_EVENT& evento) override;
    EstadoProximaTela getProximaTelaEstado() const override;
    void resetProximaTelaEstado() override;
};

#endif 