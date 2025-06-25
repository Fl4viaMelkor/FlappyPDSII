
// Inclui o cabeçalho da classe Dado_Jogador
#include "../../include/dados/dados.hpp"

// Inclui a biblioteca de teste Doctest
#include "../doctest.h"
#include <iostream>
#include <set>       // Para a verifica├º├úo das chaves exportadas
#include <stdexcept> // Para std::runtime_error, std::invalid_argument
#include <string>
#include <unordered_map>
#include <vector>
// Usar std:: aqui é aceitável, pois é um arquivo .cpp de teste
using namespace std;

// Conjunto de chaves esperadas ao exportar Dado_Jogador para 'objeto'
// Use DOCTEST_TEST_SUITE para agrupar testes
DOCTEST_TEST_SUITE("Dado_Jogador Tests")
{

    // =============================================================================
    // Testes para a classe Dado_Jogador
    // =============================================================================

    TEST_CASE("Dado_Jogador: Construtor Padrao e Exportacao")
    {
        // Testa se o construtor padrão inicializa os membros corretamente
        // e se a exportação reflete esses valores padrão.
        Dado_Jogador jogador;
        objeto obj = jogador.exportar();

        // Verifica se as chaves exportadas são as esperadas
        set<string> expected_keys = { "nome", "apelido", "pontuacoes" };
        REQUIRE(obj.dados.size() == expected_keys.size()); // Garante que não há chaves extras
        for (const auto &[key, value] : obj.dados)
            CHECK(expected_keys.count(key)); // Garante que a chave existe

        // Verifica os valores padrão
        CHECK(obj.dados["nome"] == "");
        CHECK(obj.dados["apelido"] == "");
        CHECK(obj.dados["pontuacoes"] == ""); // Espera-se uma string vazia para vetor vazio
        CHECK(jogador.nome() == "");
        CHECK(jogador.apelido() == "");
        CHECK(jogador.pontuacoes().empty());
        CHECK(jogador.maior_pontuacao() == -1); // Teste de maior_pontuacao para vetor vazio
        CHECK(jogador.partidas_disputadas() == 0);
    }

    TEST_CASE("Dado_Jogador: Construtor Parametrizado (nome, apelido)")
    {
        // Testa se o construtor parametrizado inicializa os membros com os valores fornecidos
        Dado_Jogador jogador("Leandro", "Leo");
        objeto obj = jogador.exportar(); // Exporta para verificar os valores

        CHECK(jogador.nome() == "Leandro");
        CHECK(jogador.apelido() == "Leo");
        CHECK(jogador.pontuacoes().empty()); // Espera-se que pontuacoes seja vazio por padrão

        CHECK(obj.dados["nome"] == "Leandro");
        CHECK(obj.dados["apelido"] == "Leo");
        CHECK(obj.dados["pontuacoes"] == ""); // String vazia para vetor vazio

        // Testa com nome vazio
        Dado_Jogador jogador_nome_vazio("", "SemApelido");
        CHECK(jogador_nome_vazio.nome() == "");
        CHECK(jogador_nome_vazio.apelido() == "SemApelido");

        // Testa com apelido vazio
        Dado_Jogador jogador_apelido_vazio("SomenteNome", "");
        CHECK(jogador_apelido_vazio.nome() == "SomenteNome");
        CHECK(jogador_apelido_vazio.apelido() == "");

        // Testa ambos vazios
        Dado_Jogador jogador_ambos_vazios("", "");
        CHECK(jogador_ambos_vazios.nome() == "");
        CHECK(jogador_ambos_vazios.apelido() == "");
    }

    TEST_CASE("Dado_Jogador: Construtor a partir de 'objeto' e Carregar")
    {
        SUBCASE("Carregar objeto completo e válido")
        {
            objeto obj_in;
            obj_in.dados["nome"] = "Messi";
            obj_in.dados["apelido"] = "GOAT";
            obj_in.dados["pontuacoes"] = "100,200,150";
            obj_in.dados["extra_key"] = "extra_value"; // Testar chave extra

            Dado_Jogador jogador(obj_in); // Construtor a partir de objeto

            CHECK(jogador.nome() == "Messi");
            CHECK(jogador.apelido() == "GOAT");
            CHECK(jogador.pontuacoes() == vector{ 100, 200, 150 });
            CHECK(jogador.maior_pontuacao() == 200);
            CHECK(jogador.partidas_disputadas() == 3);

            // Testar a exportação do objeto deserializado
            objeto obj_out = jogador.exportar();
            CHECK(obj_out.dados["nome"] == "Messi");
            CHECK(obj_out.dados["apelido"] == "GOAT");
            CHECK(obj_out.dados["pontuacoes"] == "100,200,150");
            CHECK(obj_out.dados.count("extra_key") == 0); // Chaves extras não devem ser exportadas
        }

        SUBCASE("Carregar objeto com chaves ausentes")
        {
            objeto obj_partial;
            obj_partial.dados["nome"] = "Parcial";
            // 'apelido' e 'pontuacoes' estão ausentes

            Dado_Jogador jogador_partial;
            jogador_partial.carregar(obj_partial); // Chamar carregar diretamente

            CHECK(jogador_partial.nome() == "Parcial");
            CHECK(jogador_partial.apelido() == "");      // Deve ser padronizado para vazio
            CHECK(jogador_partial.pontuacoes().empty()); // Deve ser vazio
        }

        SUBCASE("Carregar objeto com 'pontuacoes' vazia ou mal formatada")
        {
            objeto obj_empty_scores;
            obj_empty_scores.dados["nome"] = "JogadorVazio";
            obj_empty_scores.dados["apelido"] = "JV";
            obj_empty_scores.dados["pontuacoes"] = ""; // String vazia de pontuações

            Dado_Jogador jogador_empty_scores;
            jogador_empty_scores.carregar(obj_empty_scores);
            CHECK(jogador_empty_scores.nome() == "JogadorVazio");
            CHECK(jogador_empty_scores.apelido() == "JV");
            CHECK(jogador_empty_scores.pontuacoes().empty());
            CHECK(jogador_empty_scores.maior_pontuacao() == -1);

            objeto obj_malformed_scores;
            obj_malformed_scores.dados["nome"] = "Erro";
            obj_malformed_scores.dados["apelido"] = "Erro";
            obj_malformed_scores.dados["pontuacoes"] = "10,abc,20"; // Mal formatado

            Dado_Jogador jogador_malformed_scores;
            // Espera-se que std::stoi lance uma exceção para "abc"
            CHECK_THROWS_AS(jogador_malformed_scores.carregar(obj_malformed_scores), std::invalid_argument);
        }

        SUBCASE("Carregar objeto com 'pontuacoes' contendo apenas um valor")
        {
            objeto obj_single_score;
            obj_single_score.dados["nome"] = "Um Score";
            obj_single_score.dados["apelido"] = "US";
            obj_single_score.dados["pontuacoes"] = "123";

            Dado_Jogador jogador_single_score(obj_single_score);
            CHECK(jogador_single_score.pontuacoes() == vector{ 123 });
        }
    }

    TEST_CASE("Dado_Jogador: Operadores de Comparacao (== e !=)")
    {
        Dado_Jogador jogador1("Ana", "Aninha");
        Dado_Jogador jogador2("Ana", "Aninha");
        Dado_Jogador jogador3("Bruno", "Bruninho");
        Dado_Jogador jogador4("Ana", "Aninha");
        jogador4.add_pontuacao(10);
        jogador4.add_pontuacao(20); // jogador4 com pontuações [10, 20]
        Dado_Jogador jogador5("Ana", "Aninha");
        jogador5.pontuacoes({ 10, 20 }); // jogador5 com pontuações [10, 20] (idêntico a jogador4)
        Dado_Jogador jogador6("Ana", "Aninha");
        jogador6.add_pontuacao(10); // jogador6 com pontuações [10]

        SUBCASE("Comparacao de igualdade")
        {
            CHECK(jogador1 == jogador2);       // Nome, apelido e pontuacoes vazias iguais
            CHECK_FALSE(jogador1 == jogador3); // Diferente nome/apelido
            CHECK_FALSE(jogador1 == jogador4); // Diferente por pontuações (vazio vs [10,20])
            CHECK(jogador4 == jogador5);       // Mesmos nome, apelido e pontuações
            CHECK_FALSE(jogador4 == jogador6); // Mesmos nome/apelido, mas pontuações diferentes ([10,20] vs [10])
        }

        SUBCASE("Comparacao de diferenca")
        {
            CHECK_FALSE(jogador1 != jogador2); // Objetos idênticos
            CHECK(jogador1 != jogador3);       // Objetos diferentes
            CHECK(jogador1 != jogador4);       // Diferente por pontuações
            CHECK_FALSE(jogador4 != jogador5); // Objetos idênticos (incluindo pontuações)
            CHECK(jogador4 != jogador6);       // Pontuações diferentes
        }

        SUBCASE("Comparacao polimorfica (Dado* vs Dado_Jogador)")
        {
            Dado_Jogador j1("Polimorfico", "Poli");
            j1.add_pontuacao(50);
            Dado_Jogador j2("Polimorfico", "Poli");
            j2.add_pontuacao(50);
            Dado_Jogador j3("Outro", "Outro");

            Dado *ptr1 = &j1;
            Dado *ptr2 = &j2;
            Dado *ptr3 = &j3;

            CHECK(*ptr1 == *ptr2);       // Ponteiros para objetos Dado_Jogador idênticos
            CHECK_FALSE(*ptr1 == *ptr3); // Ponteiros para objetos Dado_Jogador diferentes

            // Teste de comparação com um tipo Dado diferente
            // (A classe `Dado` não foi fornecida, então crio uma mock simples para o teste de polimorfismo)
            class MockDado : public Dado {
              public:
                void carregar(objeto obj) override {}
                objeto exportar() override { return objeto(); }
                // Implementação de operador de igualdade que retorna false se não for MockDado
                bool operator==(const Dado &outro) const override
                {
                    return dynamic_cast<const MockDado *>(&outro) != nullptr;
                }
                bool operator!=(const Dado &outro) const override { return !(*this == outro); }
            };
            MockDado mock_dado;
            CHECK_FALSE(*ptr1 == mock_dado); // Dado_Jogador vs MockDado (deve ser false)
            CHECK_FALSE(mock_dado == *ptr1); // MockDado vs Dado_Jogador (também deve ser false)
        }
    }

    TEST_CASE("Dado_Jogador: Getters e Setters")
    {
        Dado_Jogador jogador("Teste", "Testador");

        SUBCASE("Nome")
        {
            CHECK(jogador.nome() == "Teste");
            jogador.nome("Novo Nome");
            CHECK(jogador.nome() == "Novo Nome");
        }

        SUBCASE("Apelido")
        {
            CHECK(jogador.apelido() == "Testador");
            jogador.apelido("Novo Apelido");
            CHECK(jogador.apelido() == "Novo Apelido");
        }

        SUBCASE("Pontuacoes (get e set)")
        {
            CHECK(jogador.pontuacoes().empty());
            vector<int> scores = { 10, 20, 30 };
            jogador.pontuacoes(scores);
            CHECK(jogador.pontuacoes() == scores);

            vector<int> new_scores = { 5, 15 };
            jogador.pontuacoes(new_scores);
            CHECK(jogador.pontuacoes() == new_scores);

            // Testar com vetor vazio
            jogador.pontuacoes({});
            CHECK(jogador.pontuacoes().empty());
        }
    }

    TEST_CASE("Dado_Jogador: Funcoes de Pontuacao")
    {
        Dado_Jogador jogador;

        SUBCASE("add_pontuacao")
        {
            jogador.add_pontuacao(100);
            CHECK(jogador.pontuacoes() == vector<int>{ 100 });
            jogador.add_pontuacao(50);
            CHECK(jogador.pontuacoes() == vector<int>{ 100, 50 });
            jogador.add_pontuacao(120);
            CHECK(jogador.pontuacoes() == vector<int>{ 100, 50, 120 });
        }

        SUBCASE("maior_pontuacao")
        {
            CHECK(jogador.maior_pontuacao() == -1); // Vetor vazio

            jogador.add_pontuacao(10);
            CHECK(jogador.maior_pontuacao() == 10);

            jogador.add_pontuacao(50);
            CHECK(jogador.maior_pontuacao() == 50);

            jogador.add_pontuacao(20);
            CHECK(jogador.maior_pontuacao() == 50);

            jogador.add_pontuacao(100);
            CHECK(jogador.maior_pontuacao() == 100);

            jogador.add_pontuacao(-5); // Testa com número negativo
            CHECK(jogador.maior_pontuacao() == 100);

            jogador.pontuacoes({ -10, -5, -20 }); // Testa só negativos
            CHECK(jogador.maior_pontuacao() == -5);
        }

        SUBCASE("partidas_disputadas")
        {
            CHECK(jogador.partidas_disputadas() == 0);

            jogador.add_pontuacao(100);
            CHECK(jogador.partidas_disputadas() == 1);

            jogador.add_pontuacao(50);
            CHECK(jogador.partidas_disputadas() == 2);

            jogador.pontuacoes({ 1, 2, 3, 4, 5 });
            CHECK(jogador.partidas_disputadas() == 5);

            jogador.pontuacoes({});
            CHECK(jogador.partidas_disputadas() == 0);
        }
    }

    TEST_CASE("Dado_Jogador: Integracao com SQL Columns")
    {
        Dado_Jogador jogador; // Instancia para acessar métodos estáticos ou de membro

        SUBCASE("get_sql_columns")
        {
            auto columns = Dado_Jogador::get_sql_columns(); // Ou Dado_Jogador::get_sql_columns();
            CHECK(columns.size() == 3);
            CHECK(columns["NOME"] == "TEXT NOT NULL");
            CHECK(columns["APELIDO"] == "TEXT PRIMARY KEY");
            CHECK(columns["PONTUACOES"] == "TEXT");
        }

        SUBCASE("get_colunas")
        {
            vector<string> colunas = jogador.get_colunas();
            CHECK(colunas.size() == 3);
            // A ordem pode variar para unordered_map, então verificamos se contém os elementos
            CHECK(std::find(colunas.begin(), colunas.end(), "NOME") != colunas.end());
            CHECK(std::find(colunas.begin(), colunas.end(), "APELIDO") != colunas.end());
            CHECK(std::find(colunas.begin(), colunas.end(), "PONTUACOES") != colunas.end());
        }

        SUBCASE("get_primary_key") { CHECK(jogador.get_primary_key() == "apelido"); }
    }

} // End DOCTEST_TEST_SUITE("Dado_Jogador Tests")