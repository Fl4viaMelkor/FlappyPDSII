// Define que este arquivo conterá a 'main' do nosso programa de testes.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest.h" 
#include "../../include/interface/TelaCadastro.hpp"
#include "../../include/dados/logger.hpp"
#include "../../include/dados/dados.hpp" 
#include <allegro5/allegro5.h>

// --- INÍCIO DOS TESTES ---

TEST_CASE("1. TelaCadastro - Estado Inicial") {
    PlayerLogger logger_teste;
    TelaCadastro tela_teste(100, logger_teste, 800, 600);

    // Verifica se, ao ser criada, o nome do jogador está vazio.
    CHECK(tela_teste.getNomeJogador().empty() == true);
    
    // Verifica se o estado de transição inicial é NENHUM.
    CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::NENHUM);
}

TEST_CASE("2. TelaCadastro - Lógica de Input de Texto") {
    PlayerLogger logger_teste;
    TelaCadastro tela_teste(100, logger_teste, 800, 600);

    // Cria um evento "falso" para simular a digitação de caracteres.
    ALLEGRO_EVENT evento;
    evento.type = ALLEGRO_EVENT_KEY_CHAR;

    SUBCASE("Digitando caracteres") {
        evento.keyboard.unichar = 'A';
        tela_teste.step(evento);
        CHECK(tela_teste.getNomeJogador() == "A");

        evento.keyboard.unichar = 'S';
        tela_teste.step(evento);
        CHECK(tela_teste.getNomeJogador() == "AS");

        evento.keyboard.unichar = 'A';
        tela_teste.step(evento);
        CHECK(tela_teste.getNomeJogador() == "ASA");
    }

    SUBCASE("Usando Backspace") {
        // Primeiro, digita "TESTE"
        evento.keyboard.unichar = 'T'; tela_teste.step(evento);
        evento.keyboard.unichar = 'E'; tela_teste.step(evento);
        evento.keyboard.unichar = 'S'; tela_teste.step(evento);
        evento.keyboard.unichar = 'T'; tela_teste.step(evento);
        evento.keyboard.unichar = 'E'; tela_teste.step(evento);
        REQUIRE(tela_teste.getNomeJogador() == "TESTE");

        // Agora, simula o backspace
        evento.keyboard.keycode = ALLEGRO_KEY_BACKSPACE;
        evento.keyboard.unichar = 0; // Para backspace, unichar é 0

        tela_teste.step(evento);
        CHECK(tela_teste.getNomeJogador() == "TEST");

        tela_teste.step(evento);
        CHECK(tela_teste.getNomeJogador() == "TES");
    }

    SUBCASE("Respeitando o limite de caracteres") {
        // Digita 15 caracteres
        for(int i = 0; i < 15; ++i) {
            evento.keyboard.unichar = 'A';
            tela_teste.step(evento);
        }
        CHECK(tela_teste.getNomeJogador() == "AAAAAAAAAAAAAAA");
        CHECK(tela_teste.getNomeJogador().length() == 15);

        // Tenta digitar o 16º caractere
        evento.keyboard.unichar = 'B';
        tela_teste.step(evento);

        // O nome não deve ter mudado
        CHECK(tela_teste.getNomeJogador().length() == 15);
        CHECK(tela_teste.getNomeJogador() == "AAAAAAAAAAAAAAA");
    }
}

TEST_CASE("3. TelaCadastro - Lógica de Confirmação (Enter)") {
    PlayerLogger logger_teste;
    TelaCadastro tela_teste(100, logger_teste, 800, 600);

    ALLEGRO_EVENT evento_enter;
    evento_enter.type = ALLEGRO_EVENT_KEY_CHAR;
    evento_enter.keyboard.keycode = ALLEGRO_KEY_ENTER;
    evento_enter.keyboard.unichar = '\r';

    SUBCASE("Confirmando um nome preenchido") {
        // Digita um nome primeiro
        ALLEGRO_EVENT evento_char;
        evento_char.type = ALLEGRO_EVENT_KEY_CHAR;
        evento_char.keyboard.unichar = 'F';
        tela_teste.step(evento_char);
        
        // Simula o pressionar de Enter
        tela_teste.step(evento_enter);
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::MENU_PRINCIPAL);
    }
    
    SUBCASE("Confirmando com nome vazio deve usar 'JOGADOR' e transicionar") {
        // Não digita nada, apenas pressiona Enter
        tela_teste.step(evento_enter);
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::MENU_PRINCIPAL);
    }
}