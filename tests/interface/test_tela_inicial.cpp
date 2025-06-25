// Define que este arquivo conterá a 'main' do nosso programa de testes.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

// Inclui a biblioteca de testes e a classe que queremos testar.
#include "../doctest.h" 
#include "../include/interface/TelaInicial.hpp"

// Precisamos simular um evento do Allegro, então incluímos o header.
#include <allegro5/allegro5.h>

// --- INÍCIO DOS TESTES ---

TEST_CASE("1. Testando o estado inicial da TelaInicial") {
    // Cria uma instância da classe a ser testada.
    TelaInicial tela_teste(800, 600);

    // Verifica se a primeira opção (índice 0) está selecionada por padrão.
    CHECK(tela_teste.getOpcaoSelecionadaIdx() == 0);
    
    // Verifica se o estado de transição inicial é NENHUM.
    CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::NENHUM);
}

TEST_CASE("2. Testando a navegação no menu") {
    TelaInicial tela_teste(800, 600);

    // Cria um evento "falso" do Allegro para simular o pressionamento de teclas.
    ALLEGRO_EVENT evento;
    evento.type = ALLEGRO_EVENT_KEY_DOWN;

    SUBCASE("Navegando para baixo") {
        // Simula pressionar a seta para baixo
        evento.keyboard.keycode = ALLEGRO_KEY_DOWN;
        tela_teste.step(evento);
        // A seleção deve ir para o índice 1 ("Recordes")
        CHECK(tela_teste.getOpcaoSelecionadaIdx() == 1);

        tela_teste.step(evento);
        // A seleção deve ir para o índice 2 ("Sair")
        CHECK(tela_teste.getOpcaoSelecionadaIdx() == 2);
    }

    SUBCASE("Dando a volta no menu (de baixo para cima)") {
        evento.keyboard.keycode = ALLEGRO_KEY_DOWN;
        tela_teste.step(evento); // Vai para 1
        tela_teste.step(evento); // Vai para 2
        tela_teste.step(evento); // Deve voltar para 0 ("Iniciar Jogo")
        CHECK(tela_teste.getOpcaoSelecionadaIdx() == 0);
    }
    
    SUBCASE("Navegando para cima") {
        // Simula pressionar a seta para cima
        evento.keyboard.keycode = ALLEGRO_KEY_UP;
        tela_teste.step(evento);
        // Estava em 0, deve dar a volta para o último item (índice 2, "Sair")
        CHECK(tela_teste.getOpcaoSelecionadaIdx() == 2);

        tela_teste.step(evento);
        // Deve ir para o índice 1 ("Recordes")
        CHECK(tela_teste.getOpcaoSelecionadaIdx() == 1);
    }
}

TEST_CASE("3. Testando a seleção de opções") {
    TelaInicial tela_teste(800, 600);
    ALLEGRO_EVENT evento_selecao;
    evento_selecao.type = ALLEGRO_EVENT_KEY_DOWN;
    evento_selecao.keyboard.keycode = ALLEGRO_KEY_ENTER;

    SUBCASE("Selecionando 'Iniciar Jogo'") {
        // Garante que a seleção está no item 0
        CHECK(tela_teste.getOpcaoSelecionadaIdx() == 0);
        // Simula o pressionar de Enter
        tela_teste.step(evento_selecao);
        // Verifica se o estado de transição foi definido corretamente
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::JOGO_PRINCIPAL);
    }

    SUBCASE("Selecionando 'Recordes'") {
        // Move a seleção para o item 1
        ALLEGRO_EVENT evento_navegacao;
        evento_navegacao.type = ALLEGRO_EVENT_KEY_DOWN;
        evento_navegacao.keyboard.keycode = ALLEGRO_KEY_DOWN;
        tela_teste.step(evento_navegacao);
        
        // Simula o pressionar de Enter
        tela_teste.step(evento_selecao);
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::TELA_HIGHSCORES);
    }
    
    SUBCASE("Selecionando 'Sair' com ESC") {
        ALLEGRO_EVENT evento_esc;
        evento_esc.type = ALLEGRO_EVENT_KEY_DOWN;
        evento_esc.keyboard.keycode = ALLEGRO_KEY_ESCAPE;

        tela_teste.step(evento_esc);
        CHECK(tela_teste.getProximaTelaEstado() == EstadoProximaTela::SAIR_DO_JOGO);
    }
}