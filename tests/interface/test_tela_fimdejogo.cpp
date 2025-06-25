// Define que este arquivo conterá a 'main' do nosso programa de testes.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest.h" 
#include "../include/interface/tela_fimdejogo.hpp"
#include <allegro5/allegro5.h>

// --- INÍCIO DOS TESTES ---

TEST_CASE("1. TelaFimDeJogo - Estado Inicial") {
    // Para testar, criamos uma instância da tela, passando valores de exemplo.
    tela_fimdejogo tela_teste(100, 800, 600);

    // Verificamos se, ao ser criada, a tela não tem nenhuma ação de transição definida.
    CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::NENHUM);
}

TEST_CASE("2. TelaFimDeJogo - Interação do Teclado") {
    tela_fimdejogo tela_teste(100, 800, 600);

    // Cria um evento "falso" do Allegro para simular o pressionamento de teclas.
    ALLEGRO_EVENT evento;
    evento.type = ALLEGRO_EVENT_KEY_DOWN;

    SUBCASE("Pressionar R deve mudar o estado para REINICIAR_JOGO") {
        evento.keyboard.keycode = ALLEGRO_KEY_R;
        tela_teste.step(evento);
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::REINICIAR_JOGO);
    }

    SUBCASE("Pressionar M deve mudar o estado para MENU_PRINCIPAL") {
        evento.keyboard.keycode = ALLEGRO_KEY_M;
        tela_teste.step(evento);
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::MENU_PRINCIPAL);
    }

    SUBCASE("Pressionar ESC deve mudar o estado para SAIR_DO_JOGO") {
        evento.keyboard.keycode = ALLEGRO_KEY_ESCAPE;
        tela_teste.step(evento);
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::SAIR_DO_JOGO);
    }

    SUBCASE("Pressionar qualquer outra tecla não deve fazer nada") {
        evento.keyboard.keycode = ALLEGRO_KEY_ENTER; // Testa com a tecla 'Enter'
        tela_teste.step(evento);
        // O estado deve continuar como NENHUM
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::NENHUM);
    }
}