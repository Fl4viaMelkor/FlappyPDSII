// Define que este arquivo conterá a 'main' do nosso programa de testes.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

// --- INCLUDES CORRIGIDOS ---
// Com a flag -I include/ no comando de compilação, o compilador saberá onde encontrar estes arquivos.
#include "../doctest.h"
#include "../../include/interface/TelaHighScore.hpp"
#include "../../include/dados/logger.hpp"
#include "../../include/dados/dados.hpp" 
#include "../../include/interface/tela_base.hpp" 

#include <allegro5/allegro5.h>

// --- INÍCIO DOS TESTES ---

TEST_CASE("1. TelaHighScores - Estado Inicial") {
    // Para testar, criamos uma instância real do PlayerLogger.
    PlayerLogger logger_teste;
    
    // Criamos a tela, passando o logger para ela.
    TelaHighScores tela_teste(logger_teste, 800, 600);

    // Verificamos se, ao ser criada, a tela não tem nenhuma ação de transição definida.
    CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::NENHUM);
}

TEST_CASE("2. TelaHighScores - Interação do Teclado") {
    PlayerLogger logger_teste;
    TelaHighScores tela_teste(logger_teste, 800, 600);

    // Cria um evento "falso" do Allegro para simular o pressionamento de teclas.
    ALLEGRO_EVENT evento;
    evento.type = ALLEGRO_EVENT_KEY_DOWN;

    SUBCASE("Pressionar ESC deve mudar o estado para MENU_PRINCIPAL") {
        evento.keyboard.keycode = ALLEGRO_KEY_ESCAPE;
        tela_teste.step(evento);
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::MENU_PRINCIPAL);
    }

    SUBCASE("Pressionar qualquer outra tecla não deve fazer nada") {
        evento.keyboard.keycode = ALLEGRO_KEY_A; // Testa com a tecla 'A'
        tela_teste.step(evento);
        // O estado deve continuar como NENHUM
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::NENHUM);

        evento.keyboard.keycode = ALLEGRO_KEY_ENTER; // Testa com a tecla 'Enter'
        tela_teste.step(evento);
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::NENHUM);
    }
}

TEST_CASE("3. TelaHighScores - Reset de Estado") {
    PlayerLogger logger_teste;
    TelaHighScores tela_teste(logger_teste, 800, 600);

    // Simula o pressionamento de ESC para mudar o estado
    ALLEGRO_EVENT evento;
    evento.type = ALLEGRO_EVENT_KEY_DOWN;
    evento.keyboard.keycode = ALLEGRO_KEY_ESCAPE;
    tela_teste.step(evento);

    // Confirma que o estado mudou para MENU_PRINCIPAL
    REQUIRE(tela_teste.getProximaTelaEstado() == EstadoProximaTela::MENU_PRINCIPAL);

    // Chama a função de reset
    tela_teste.resetProximaTelaEstado();

    // Verifica se o estado voltou a ser NENHUM
    CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::NENHUM);
}