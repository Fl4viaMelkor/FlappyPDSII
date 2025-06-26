// tests/test_parallax_background.cpp
#include "doctest.h"
#include "../include/interface/parallax_background.hpp"

// NOTA: Para este teste funcionar, o construtor de CamadaBackground
// não deve quebrar o programa se não conseguir carregar a textura.
// Geralmente, al_load_bitmap retorna nullptr, o que é seguro.

TEST_CASE("Testando a Classe ParallaxBackground") {
    // Dimensões da tela para o teste
    int larguraTela = 800;
    int alturaTela = 600;

    ParallaxBackground parallax(larguraTela, alturaTela);

    SUBCASE("Construtor inicializa sem camadas") {
        // Verifica se o objeto começa com zero camadas
        CHECK(parallax.getNumeroDeCamadas() == 0);
    }

    SUBCASE("Adicionar uma ou mais camadas") {
        // Inicialmente, não há camadas
        CHECK(parallax.getNumeroDeCamadas() == 0);

        // Adiciona a primeira camada
        parallax.adicionarCamada("caminho/fake/textura1.png", 0.5f);
        CHECK(parallax.getNumeroDeCamadas() == 1);

        // Adiciona a segunda camada
        parallax.adicionarCamada("caminho/fake/textura2.png", 1.0f);
        CHECK(parallax.getNumeroDeCamadas() == 2);
    }

    SUBCASE("Adicionar múltiplas camadas até o limite") {
        const int maxCamadas = 100; // Limite para o teste
        for (int i = 0; i < maxCamadas; ++i) {
            parallax.adicionarCamada("caminho/fake/textura" + std::to_string(i) + ".png", 0.1f * i);
        }
        CHECK(parallax.getNumeroDeCamadas() == maxCamadas);
    }

    SUBCASE("Camadas com velocidades extremas") {
        parallax.adicionarCamada("caminho/fake/textura_lenta.png", 0.0f); // Parada
        parallax.adicionarCamada("caminho/fake/textura_rapida.png", 100.0f); // Muito rápida

        // Atualiza o parallax
        parallax.update(1.0f / 60.0f);

        // As chamadas não devem causar erros
        CHECK_NOTHROW(parallax.draw());
    }

    SUBCASE("Adicionar camadas com texturas inválidas") {
        // Adiciona camadas com caminhos inválidos.
        parallax.adicionarCamada("caminho/invalido1.png", 0.5f);
        parallax.adicionarCamada("caminho/invalido2.png", 1.0f);

        // O número de camadas deve ser igual ao número de chamadas,
        // mesmo que as texturas não sejam carregadas corretamente.
        CHECK(parallax.getNumeroDeCamadas() == 2);
    }

    SUBCASE("Update e Draw não causam erros") {
        // Adiciona algumas camadas para garantir que os loops não estejam vazios
        parallax.adicionarCamada("caminho/fake/textura1.png", 0.5f);
        parallax.adicionarCamada("caminho/fake/textura2.png", 1.0f);

        // O teste passa se essas chamadas forem executadas sem crashar.
        // O valor de deltaTime é arbitrário para este teste.
        CHECK_NOTHROW(parallax.update(1.0f / 60.0f));
        CHECK_NOTHROW(parallax.draw());
    }
}
