#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

#include "tela_jogo.hpp"

/*
Comentários Guilherme Asafe: (pra quem está mexendo no main)
  Criei um método para o objeto cano. Ele apenas mostar um retângulo na tela.
  Pra testar se está funcionando coloque: 

  

  Flávia: Eu coloquei no pass, dentro da classe tela jogo, depois checa pra ver se é isso mesmo.
  Caso queira alterar algo no cano, não esqueça de mexer no tela_jogo ou coloque um aviso lá pra eu ou alguém mexer
*/



int main() {
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_KEYBOARD_STATE key_state;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    TelaBase* telaAtual = new TelaJogo();

    bool running = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    while (running) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
          telaAtual->update();

          al_clear_to_color(al_map_rgb(0, 0, 0));

          //Desenho de telas
          telaAtual->draw();
          al_flip_display();

          // Verifica se o jogo acabou, tentar outro método depois*
          TelaJogo* jogo = dynamic_cast<TelaJogo*>(telaAtual);
          if (jogo && jogo->acabouJogo()) {
          //
          running = false; // só para exemplo 
          }
        }else{
            telaAtual->step(event);
            
          }
    }

delete telaAtual;

    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    return 0;
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

    al_init_primitives_addon();   // Inicializa addon para primitivas gráficas (retângulos, linhas, etc)

    al_install_keyboard();        // Inicializa entrada do teclado


    const int LARGURA = 800;      // largura da janela
    const int ALTURA = 600;       // altura da janela


    ALLEGRO_DISPLAY* display = al_create_display(LARGURA, ALTURA);
    if (!display) {


        std::cerr << "Falha ao criar a janela.\n";
        return -1;


    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60); // timer para 60 frames por segundo 


    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();



    // Registra fontes de eventos (janela, timer e teclado)
    al_register_event_source(fila, al_get_display_event_source(display));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());



    // Cor verde para os canos
    ALLEGRO_COLOR verde = al_map_rgb(0, 255, 0);



    float largura_cano = 80.0f;          // largura dos canos
    float abertura = 100.0f;             // espaço vertical entre os blocos (abertura)
    float velocidade = -2.0f;            // velocidade de movimento para esquerda
    float espacamento_horizontal = 250.0f; // espaço horizontal entre os canos



    // Cria 4 canos com espaçamento horizontal constante
    std::vector<Cano> canos;
    for (int i = 0; i < 4; ++i) {
        float x_inicial = LARGURA + i * espacamento_horizontal;
        canos.emplace_back(x_inicial, largura_cano, abertura, ALTURA, verde, 2.0f);
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



            // Reseta os canos que saíram da tela, reposicionando após o último cano
            for (auto& cano : canos) {
                cano.reset_if_out_of_screen(0, pos_ultimo, espacamento_horizontal, ALTURA);
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