// tests/interface/test_camada_background.cpp
#include "doctest.h"
#include "../include/interface/camada_background.hpp"

TEST_CASE("Testando a Classe CamadaBackground") {

    // --- Sub-caso 1: Robustez do Construtor ---
    SUBCASE("Construtor lida com caminho de textura inválido sem quebrar") {
        CamadaBackground* camada = nullptr;

        // Verifica que criar a camada com um caminho falso não causa um crash.
        CHECK_NOTHROW(camada = new CamadaBackground("caminho/impossivel/de/existir.png", 50.0f, 800, 600));

        // Se o teste chegou até aqui, significa que a criação foi segura.
        // Verificamos o estado inicial.
        REQUIRE(camada != nullptr);
        CHECK(camada->getPosicaoX() == 0.0f);

        delete camada;
    }

    // --- Sub-caso 2: Robustez dos Métodos Update e Draw ---
    SUBCASE("Update e Draw são seguros mesmo quando a textura não foi carregada") {
        // Cria uma camada que sabemos que falhou ao carregar a textura.
        CamadaBackground camada("caminho/falso.png", 50.0f, 800, 600);

        // Guarda a posição inicial.
        float posicao_inicial = camada.getPosicaoX();

        // Tenta atualizar e desenhar. O teste passa se não houver crash.
        float delta_time = 0.16f; // Um valor de exemplo para o tempo.
        CHECK_NOTHROW(camada.update(delta_time));
        CHECK_NOTHROW(camada.draw());

        // A posição não deve ter mudado, pois a guarda "if (!textura_)" deve ter sido ativada.
        CHECK(camada.getPosicaoX() == posicao_inicial);
    }

    // --- Sub-caso 3: Lógica de Update (quando a textura existe)

    SUBCASE("Update move a camada para a esquerda") {
        // ASSUMINDO que 'assets/testes/pixel.png' existe.
        CamadaBackground camada("assets/testes/pixel.png", 100.0f, 800, 600);

        float delta_time = 0.5f;

        camada.update(delta_time);
    }
}