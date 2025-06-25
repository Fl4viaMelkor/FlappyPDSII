#include "../../include/interface/TelaInicial.hpp"
#include <allegro5/allegro_primitives.h>
#include <stdexcept>
#include <iostream>
#include <allegro5/allegro_audio.h>   
#include <allegro5/allegro_acodec.h> 

TelaInicial::TelaInicial(int largura, int altura) :
    fonte_titulo(nullptr),
    fonte_opcoes(nullptr),
    musica_menu(nullptr),
    animacao_spritesheet(nullptr),
    anim_frame_largura(0),
    anim_frame_atual(0),
    anim_contador_tempo(0.0f),
    anim_delay_frame(0.2f),
    opcao_selecionada_idx(0),
    estado_escolhido(EstadoProximaTela::NENHUM),
    largura_tela(largura),
    altura_tela(altura)  {


    // ... seu código que carrega fontes, etc. ...

    // --- Carrega e toca a música do menu ---
    musica_menu = al_load_audio_stream("assets/audio/query.wav", 4, 2048); // Use um arquivo de música diferente aqui!
    if (musica_menu) {
        al_attach_audio_stream_to_mixer(musica_menu, al_get_default_mixer());
        al_set_audio_stream_playmode(musica_menu, ALLEGRO_PLAYMODE_LOOP);
    }

//carregar animação
         try {
        animacao_spritesheet = al_load_bitmap("assets/player/galinha_spritesheet.png");
        if (!animacao_spritesheet) {
            throw std::runtime_error("Falha ao carregar o spritesheet da animacao do menu");
        }

        anim_frame_largura = 24;

    } catch (const std::runtime_error& e) {
        std::cerr << "Erro em TelaInicial ao carregar animacao: " << e.what() << std::endl;
    }

    // Carrega fontes
    try {

        fonte_titulo = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 64, 0);
        fonte_opcoes = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 40, 0);
        if (!fonte_titulo || !fonte_opcoes) throw std::runtime_error("Falha ao carregar fontes para TelaInicial");
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro em TelaInicial: " << e.what() << std::endl;
    }

    // Define cores
    cor_titulo = al_map_rgb(230, 230, 250);
    cor_opcao_normal = al_map_rgb(180, 180, 200);
    cor_opcao_selecionada = al_map_rgb(255, 255, 0); // Amarelo para destaque

    // Define as opções do menu
    opcoes_menu.push_back("Iniciar Jogo");
    opcoes_menu.push_back("Recordes");
    opcoes_menu.push_back("Sair");
}

TelaInicial::~TelaInicial() {
    if (fonte_titulo) al_destroy_font(fonte_titulo);
    if (fonte_opcoes) al_destroy_font(fonte_opcoes);
    if (animacao_spritesheet) al_destroy_bitmap(animacao_spritesheet);
     if (musica_menu) al_destroy_audio_stream(musica_menu); 
}

void TelaInicial::update() {
    if (!animacao_spritesheet) {
        return;
    }
//fps
    anim_contador_tempo += (1.0f / 60.0f);

    // Verifica se já passou tempo suficiente para trocar de frame
    if (anim_contador_tempo >= anim_delay_frame) {
        anim_contador_tempo = 0.0f;


        anim_frame_atual++;
//loop
        if (anim_frame_atual >= 3) {
            anim_frame_atual = 0;
        }
    }
}

void TelaInicial::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    float centro_x = largura_tela / 2.0f;

    // Título do Jogo
    al_draw_text(fonte_titulo, cor_titulo, centro_x, altura_tela * 0.15f, ALLEGRO_ALIGN_CENTER, "FLYING CHICKEN");



    // Opções do Menu
    for (size_t i = 0; i < opcoes_menu.size(); ++i) {
        ALLEGRO_COLOR cor_atual = (static_cast<int>(i) == opcao_selecionada_idx) ? cor_opcao_selecionada : cor_opcao_normal;
        float pos_y_opcao = altura_tela * 0.5f + (i * 60);

        al_draw_text(fonte_opcoes, cor_atual, centro_x, pos_y_opcao, ALLEGRO_ALIGN_CENTER, opcoes_menu[i].c_str());

        if (static_cast<int>(i) == opcao_selecionada_idx) {


            if (animacao_spritesheet) {
                // Pega o frame certo para desenhar
                int pos_x_no_sprite = anim_frame_atual * anim_frame_largura;

                float largura_texto = al_get_text_width(fonte_opcoes, opcoes_menu[i].c_str());
                float altura_sprite = 32; // A altura do seu sprite

                // Posição X: à esquerda do texto, com uma pequena margem
                float pos_x_indicador = centro_x - (largura_texto / 2.0f) - anim_frame_largura - 15.0f;
                // Posição Y: para centralizar verticalmente com o texto
                float pos_y_indicador = pos_y_opcao + (al_get_font_line_height(fonte_opcoes) / 2.0f) - (altura_sprite / 2.0f);

                // Desenha o frame atual da animação na posição do indicador
                al_draw_bitmap_region(
                    animacao_spritesheet,
                    pos_x_no_sprite, 0,
                    anim_frame_largura, altura_sprite,
                    pos_x_indicador, pos_y_indicador,
                    0
                );
            }
        }
    }
}

void TelaInicial::step(ALLEGRO_EVENT& evento) {
    if (estado_escolhido != EstadoProximaTela::NENHUM) return;

    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                opcao_selecionada_idx = (opcao_selecionada_idx - 1 + opcoes_menu.size()) % opcoes_menu.size();
                break;
            case ALLEGRO_KEY_DOWN:
                opcao_selecionada_idx = (opcao_selecionada_idx + 1) % opcoes_menu.size();
                break;
            case ALLEGRO_KEY_ENTER:
            case ALLEGRO_KEY_SPACE:
                if (opcoes_menu[opcao_selecionada_idx] == "Iniciar Jogo") {
                    estado_escolhido = EstadoProximaTela::JOGO_PRINCIPAL;
                } else if (opcoes_menu[opcao_selecionada_idx] == "Recordes") {
                    estado_escolhido = EstadoProximaTela::TELA_HIGHSCORES;
                } else if (opcoes_menu[opcao_selecionada_idx] == "Sair") {
                    estado_escolhido = EstadoProximaTela::SAIR_DO_JOGO;
                }
                break;
            case ALLEGRO_KEY_ESCAPE:
                estado_escolhido = EstadoProximaTela::SAIR_DO_JOGO;
                break;
            default:
                break;
        }
    }
}

EstadoProximaTela TelaInicial::getProximaTelaEstado() const {
    return estado_escolhido;
}

void TelaInicial::resetProximaTelaEstado() {
    estado_escolhido = EstadoProximaTela::NENHUM;
}
