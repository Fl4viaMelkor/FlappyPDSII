#include "../include/dados/logger.hpp"

#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>


#include "../include/interface/tela_base.hpp"
#include "../include/interface/TelaInicial.hpp"
#include "../include/interface/tela_jogo.hpp"
#include "../include/interface/tela_fimdejogo.hpp"
#include "../include/interface/TelaCadastro.hpp"
#include "../include/interface/TelaHighScore.hpp"
#include "../include/dados/dados.hpp"
#include "../include/dados/logger.hpp"
#include "../include/dados/config.hpp"


void initialize();
void cleanup();


ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;

int main()
{
    initialize();

al_set_window_title(display, "Flying Chicken");


    //Ponteiro pra tela inicial
TelaBase *tela_atual = new TelaInicial(LARGURA_JANELA, ALTURA_JANELA);

    ALLEGRO_EVENT event;
    bool rodando = true;
    bool redesenhar = true;

    al_start_timer(timer);

PlayerLogger logger;

    //loop principal
    while (rodando) {
        al_wait_for_event(queue, &event);

        // Se a janela for fechada no X
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }
        // Se a janela for redimensionada
        else if(event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            al_acknowledge_resize(display);
        }
        // Passa eventos de input para a tela atual
        else if (tela_atual) {
            tela_atual->step(event);
        }

        //transição/animação de telas
        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (tela_atual) {
                tela_atual->update();
            }

            // Lógica de transição
            if (tela_atual) {
                EstadoProximaTela proxima = tela_atual->getProximaTelaEstado();

                if (proxima != EstadoProximaTela::NENHUM) {
                    if (proxima == EstadoProximaTela::SAIR_DO_JOGO) {
                        rodando = false;
                    } else {
                        TelaBase* proxima_tela = nullptr;
                        switch (proxima) {
                            case EstadoProximaTela::JOGO_PRINCIPAL:
                                proxima_tela = new TelaJogo();
                                break;
                            case EstadoProximaTela::MENU_PRINCIPAL:
                                proxima_tela = new TelaInicial(LARGURA_JANELA, ALTURA_JANELA);
                                break;
                            case EstadoProximaTela::TELA_HIGHSCORES:
                                proxima_tela = new TelaHighScores(logger, LARGURA_JANELA, ALTURA_JANELA);
                                break;
                            default: break;
                        }
                        delete tela_atual;
                        tela_atual = proxima_tela;
                    }
                }
            }

         if (TelaJogo* jogo_ptr = dynamic_cast<TelaJogo*>(tela_atual)) {
                if (jogo_ptr->acabouJogo()) {
                    // Pega a pontuação final da partida
                    int pontuacao = jogo_ptr->pontos;
                    cout << "Pontuação: " << pontuacao << endl;
                    delete tela_atual;

                    // Pergunta ao logger se a pontuação é um novo recorde
                    if (logger.isHighScore(pontuacao)) {
                        // Se for, vai para a tela de cadastro
                        tela_atual = new TelaCadastro(pontuacao, logger, LARGURA_JANELA, ALTURA_JANELA);
                    } else {
                        // Se não for, vai para a tela de fim de jogo padrão
                        tela_atual = new tela_fimdejogo(pontuacao, LARGURA_JANELA, ALTURA_JANELA);
                    }
                }
            }

            redesenhar = true;
        }
//desenho
        if (redesenhar && al_is_event_queue_empty(queue)) {
            redesenhar = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            if (tela_atual) {
                tela_atual->draw();
            }
            al_flip_display();
        }
    }

    // --- LIMPEZA FINAL ---
    delete tela_atual;
    cleanup();
    return 0;
}

void initialize()
{
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    display = al_create_display(LARGURA_JANELA, ALTURA_JANELA);
    queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
}

void cleanup()
{

    al_shutdown_image_addon();
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_uninstall_keyboard();
    al_uninstall_system();
}