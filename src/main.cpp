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
#include "../include/interface/GerenciadorHighScore.hpp"
#include "../include/dados/config.hpp" 


void initialize();
void cleanup();


ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;

int main()
{
    initialize();

al_set_window_title(display, Flying Chicken);
    
    GerenciadorHighScores gerenciador_scores("scores.txt");
    
    //Ponteiro pra tela inicial
    TelaBase *tela_atual = new TelaInicial(LARGURA_NATIVA, ALTURA_NATIVA);
    tela_atual = new TelaInicial(LARGURA_TELA, ALTURA_TELA);

    ALLEGRO_EVENT event;
    bool rodando = true;
    bool redesenhar = true;

    al_start_timer(timer);

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
                                proxima_tela = new TelaInicial(LARGURA_TELA, ALTURA_TELA);
                                break;
                            case EstadoProximaTela::TELA_HIGHSCORES:
                                proxima_tela = new TelaHighScores(gerenciador_scores, LARGURA_TELA, ALTURA_TELA);
                                break;
                            default: break;
                        }
                        delete tela_atual;
                        tela_atual = proxima_tela;
                    }
                }
            }

            // Lógica especial para quando o jogo acaba
            if (TelaJogo* jogo_ptr = dynamic_cast<TelaJogo*>(tela_atual)) {
                if (jogo_ptr->acabouJogo()) {
                    int pontuacao = jogo_ptr->getPontuacaoFinal();
                    delete tela_atual;

                    if (gerenciador_scores.isHighScore(pontuacao)) {
                        tela_atual = new TelaCadastro(pontuacao, gerenciador_scores, LARGURA_TELA, ALTURA_TELA);
                    } else {
                        tela_atual = new tela_fimdejogo(pontuacao, LARGURA_TELA, ALTURA_TELA);
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

    display = al_create_display(LARGURA_JANELA, ALTURA_TELA);
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

/*
Implementação Asafe, canos: (movimento, geração aleatória)

int main() {


    // Inicializa o gerador de números aleatórios com a seed atual
    srand(time(nullptr));

    if (!al_init()) {

        std::cerr << "Falha ao inicializar o Allegro.\n";

        return -1;
    }

    al_init_primitives_addon();   // Inicializa addon para primitivas gráficas
(retângulos, linhas, etc)

    al_install_keyboard();        // Inicializa entrada do teclado


    const int LARGURA = LARGURA_TELA;      // largura das janela
    const int ALTURA = ALTURA_TELA;       // altura da janela


    ALLEGRO_DISPLAY* display = al_create_display(LARGURA, ALTURA);
    if (!display) {


        std::cerr << "Falha ao criar a janela.\n";
        return -1;


    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60); // timer para 60 frames
por segundo


    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();



    // Registra fontes de eventos (janela, timer e teclado)
    al_register_event_source(fila, al_get_display_event_source(display));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());



    // Cor verde para os canos
    ALLEGRO_COLOR verde = al_map_rgb(0, 255, 0);



    float largura_cano = 80.0f;          // largura dos canos
    float abertura = 100.0f;             // espaço vertical entre os blocos
(abertura) float velocidade = -2.0f;            // velocidade de movimento para
esquerda float espacamento_horizontal = 250.0f; // espaço horizontal entre os
canos



    // Cria 4 canos com espaçamento horizontal constante
    std::vector<Cano> canos;
    for (int i = 0; i < 4; ++i) {
        float x_inicial = LARGURA + i * espacamento_horizontal;
        canos.emplace_back(x_inicial, largura_cano, abertura, ALTURA,
verde, 2.0f);
    }



    bool rodando = true;
    bool desenhar = true;



    al_start_timer(timer);



    // Loop principal do jogo
    while (rodando) {


        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;  // Fecha se o usuário clicar no X da janela
        }


        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            // Move todos os canos para a esquerda
            for (auto& cano : canos) {
                cano.move(velocidade);
            }



            // Encontra o cano mais à direita para garantir o ciclo
            float pos_ultimo = canos[0].getX();
            for (const auto& cano : canos) {
                if (cano.getX() > pos_ultimo)
                    pos_ultimo = cano.getX();
            }



            // Reseta os canos que saíram da tela, reposicionando após o último
cano for (auto& cano : canos) { cano.reset_if_out_of_screen(0, pos_ultimo,
espacamento_horizontal, ALTURA);
            }



            desenhar = true;
        }


        // Se for para desenhar e a fila de eventos estiver vazia
        if (desenhar && al_is_event_queue_empty(fila)) {


            al_clear_to_color(al_map_rgb(0, 0, 0));  // limpa a tela com preto


            for (const auto& cano : canos) {


                cano.draw();    // desenha todos os canos
            }


            al_flip_display();   // atualiza a tela
            desenhar = false;
        }
    }



    // Libera recursos
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(display);


    return 0;


}

*/
