//
// Created by leand on 31/05/2025.
//
#include <iostream>

#include "../../include/dados/dados.hpp" // Inclui o cabeÔö£┬║alho da classe Dado_Jogador
#include "../doctest.h"
#include <set>
#include <stdexcept>     // Para std::runtime_error
#include <string>        // Para std::string
#include <unordered_map> // Para std::unordered_map
#include <vector>        // Para std::vector

using namespace std;
// Para compilação manual: g++ test_dados_jogador.cpp dados.cpp -o test_dados_jogador -std=c++17
set<string> dados = { "nome", "apelido", "pontuacoes" };
// =============================================================================
// Testes para a classe Dado_Jogador
// =============================================================================

TEST_CASE("Dado_Jogador: Construtor Padrao")
{
    // Testa se o construtor padrão inicializa os membros corretamente
    Dado_Jogador jogador;
    objeto obj = jogador.exportar();

    for (const auto &[first, second] : obj.dados)
        REQUIRE(dados.find(first) != dados.end());

    CHECK(obj.dados["nome"] == "");
    CHECK(obj.dados["apelido"] == "");
    CHECK(obj.dados["pontuacoes"] == "");
}

TEST_CASE("Dado_Jogador: Construtor Parametrizado")
{
    // Testa se o construtor parametrizado inicializa os membros com os valores fornecidos
    objeto obj;
    Dado_Jogador jogador("Leandro", "Leo");
    obj = jogador.exportar();
    CHECK(obj.dados["nome"] == "Leandro");
    CHECK(obj.dados["apelido"] == "Leo");
    CHECK(obj.dados["pontuacoes"] == "");

    // Testa com valores padrão
    Dado_Jogador jogador2("Luiz");
    obj = jogador2.exportar();
    CHECK(obj.dados["nome"] == "Luiz");
    CHECK(obj.dados["apelido"] == "");
    CHECK(obj.dados["pontuacoes"] == "");
}

TEST_CASE("Dado_Jogador: Construtor Parametrizado com Objeto")
{
    // Testa se o construtor parametrizado inicializa os membros com os valores fornecidos
    objeto p1, p2;
    p1.dados["nome"] = "Leandro";
    p1.dados["apelido"] = "Leo";
    p1.dados["pontuacoes"] = "1,2,3,4,5";
    p1.dados["Nenhum"] = "Nada";

    objeto obj;
    Dado_Jogador jogador(p1);
    obj = jogador.exportar();
    CHECK(obj.dados["nome"] == "Leandro");
    CHECK(obj.dados["apelido"] == "Leo");
    CHECK(obj.dados["vitorias"] == "7");
    CHECK(obj.dados["derrotas"] == "15");
    CHECK(obj.dados["pontuacoes"] == "1,2,3,4,5");
}

TEST_CASE("Dado_Jogador: serialize e deserialize")
{
    // Cria um Dado_Jogador, serializa, e depois desserializa para outro objeto
    // e verifica a igualdade.

    Dado_Jogador original("Cristiano Ronaldo", "CR7");
    original.pontuacoes({ 100, 95, 110 });

    SUBCASE("Serialização e Desserialização de um objeto completo")
    {
        objeto obj = original.exportar();

        CHECK(obj.dados["nome"] == "Cristiano Ronaldo");
        CHECK(obj.dados["apelido"] == "CR7");
        CHECK(obj.dados["pontuacoes"] == "100,95,110");

        Dado_Jogador deserialized;
        deserialized.carregar(obj);

        CHECK(deserialized.nome() == "Cristiano Ronaldo");
        CHECK(deserialized.apelido() == "CR7");
        CHECK(deserialized.pontuacoes() == std::vector<int>{ 100, 95, 110 });
    }

    SUBCASE("Desserialização de objeto com chaves ausentes")
    {
        objeto partial_obj;
        partial_obj.dados["nome"] = "Meio";
        partial_obj.dados["vitorias"] = "50";

        Dado_Jogador partial_jogador;
        partial_jogador.carregar(partial_obj);

        CHECK(partial_jogador.nome() == "Meio");
        CHECK(partial_jogador.apelido() == "");      // Deve ser padrão
        CHECK(partial_jogador.pontuacoes().empty()); // Deve ser vazio
    }

    // SUBCASE("Desserialização de objeto com pontuacoes vazias ou mal formatadas") {
    //     objeto obj_empty_scores;
    //     obj_empty_scores.dados["nome"] = "Vazio";
    //     obj_empty_scores.dados["pontuacoes"] = ""; // String vazia
    //
    //     Dado_Jogador jogador_empty_scores;
    //     jogador_empty_scores.deserialize(obj_empty_scores);
    //     CHECK(jogador_empty_scores.pontuacoes.empty());
    //
    //     objeto obj_malformed_scores;
    //     obj_malformed_scores.dados["nome"] = "Malformed Scores";
    //     obj_malformed_scores.dados["pontuacoes"] = "10,abc,20"; // Mal formatado
    //
    //     // Espera-se que std::stoi lance uma exceção para "abc"
    //     CHECK_THROWS_AS(jogador_malformed_scores.deserialize(obj_malformed_scores), std::invalid_argument);
    // }
}

TEST_CASE("Dado_Jogador: Operadores de Comparação (== e !=)")
{
    Dado_Jogador jogador1("Ana", "Aninha");
    Dado_Jogador jogador2("Ana", "Aninha");     // Idêntico a jogador1
    Dado_Jogador jogador3("Bruno", "Bruninho"); // Diferente
    Dado_Jogador jogador4("Ana", "Aninha");
    jogador4.pontuacoes({ 1, 2, 3 }); // Igual, mas com pontuações diferentes
    Dado_Jogador jogador5("Ana", "Aninha");
    jogador5.pontuacoes({ 1, 2, 3 }); // Idêntico a jogador4

    SUBCASE("Comparação de igualdade")
    {
        CHECK(jogador1 == jogador2);       // Objetos idênticos
        CHECK_FALSE(jogador1 == jogador3); // Objetos diferentes
        CHECK_FALSE(jogador1 == jogador4); // Diferente por pontuações
        CHECK(jogador4 == jogador5);       // Objetos idênticos (incluindo pontuações)
    }

    SUBCASE("Comparação de diferença")
    {
        CHECK_FALSE(jogador1 != jogador2); // Objetos idênticos
        CHECK(jogador1 != jogador3);       // Objetos diferentes
        CHECK(jogador1 != jogador4);       // Diferente por pontuações
        CHECK_FALSE(jogador4 != jogador5); // Objetos idênticos (incluindo pontuações)
    }

    SUBCASE("Comparação polimórfica")
    {
        Dado *ptr_jogador1 = &jogador1;
        Dado *ptr_jogador2 = &jogador2;
        Dado *ptr_jogador3 = &jogador3;

        CHECK(*ptr_jogador1 == *ptr_jogador2);       // Ponteiros para objetos idênticos
        CHECK_FALSE(*ptr_jogador1 == *ptr_jogador3); // Ponteiros para objetos diferentes

        // Teste de comparação com um tipo Dado diferente (embora não tenhamos outros tipos Dado concretos,
        // o dynamic_cast deve retornar nullptr, resultando em false)
        class Dado_Teste : public Dado {
          public:
            void carregar(objeto obj) override {}

            objeto exportar() override { return objeto(); }
            bool operator==(const Dado &outro) const override { return false; }
            bool operator!=(const Dado &outro) const override { return true; }
        };
        Dado_Teste dado_teste;
        CHECK_FALSE(*ptr_jogador1 == dado_teste); // Dado_Jogador vs Dado_Teste
    }
}
