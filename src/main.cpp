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
