//
// Created by leand on 01/06/2025.
//
#include "../../include/dados/dados.hpp"
#include "../../include/dados/data_exception.hpp"
#include "../../include/dados/database.hpp"
#include "../doctest.h"
#include <iostream>

#include <algorithm>
#include <fstream> // Para remove (deletar arquivo)
#include <random>
#include <string>        // Para string
#include <unordered_set> // Para verificar chaves
#include <vector>        // Para vector

using namespace std;

// =============================================================================
// Funções Auxiliares para Testes
// =============================================================================

// Atualizada para corresponder ao schema de Dado_Jogador (sem 'id')
objeto create_jogador_objeto(const string &nome, const string &apelido, const string &pontuacoes_str = "")
{
    objeto obj;
    // NOME e APELIDO já são passados em maiúsculas por get_sql_columns().
    // Internamente, SQLDatabase::adicionar/atualizar normaliza chaves para maiúsculas.
    obj.dados["nome"] = nome; // Passamos em minúsculas e confiamos na normalização
    obj.dados["apelido"] = apelido;
    obj.dados["pontuacoes"] = pontuacoes_str; // String já serializada
    return obj;
}

// Helper para converter um Dado_Jogador em objeto 'objeto'
// útil para garantir que os dados estão sendo enviados no formato certo
objeto dadoJogador_to_objeto(const Dado_Jogador &jogador)
{
    objeto obj;
    obj.dados["nome"] = jogador.nome();
    obj.dados["apelido"] = jogador.apelido();

    // Serializa o vetor de pontuações para string
    string pontuacoes_str;
    const auto &pontuacoes_vec = jogador.pontuacoes();
    for (size_t i = 0; i < pontuacoes_vec.size(); ++i) {
        pontuacoes_str += to_string(pontuacoes_vec[i]);
        if (i < pontuacoes_vec.size() - 1)
            pontuacoes_str += ",";
    }
    obj.dados["pontuacoes"] = pontuacoes_str;
    return obj;
}

// Variável global (ou membro da fixture) para as colunas do DB
// Doctest organiza os testes em grupos, então o TEST_SUITE pode encapsular isso
unordered_set<string> expected_db_columns;

// =============================================================================
// Fixture para Testes de Banco de Dados
// =============================================================================
// Esta fixture garante que um novo arquivo de banco de dados seja criado antes
// de cada TEST_CASE e deletado após sua execução, garantindo isolamento.

struct DatabaseFixture {
    string db_file_name;
    JogadorSQLDatabase *db_instance;

    DatabaseFixture()
    {
        // Usa um gerador de números aleatórios para criar nomes de arquivos únicos
        // para garantir que os testes não interfiram entre si, mesmo se executados em paralelo
        // (Embora doctest seja single-threaded por padrão, é uma boa prática)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 999999);
        db_file_name = "test_db_fixture_" + to_string(distrib(gen)) + ".db";

        // Garante que o arquivo não exista de uma execução anterior falha (se o nome não for totalmente único)
        remove(db_file_name.c_str());

        // Instancia o banco de dados para o teste.
        // O construtor de JogadorSQLDatabase já abre o DB e cria a tabela.
        db_instance = new JogadorSQLDatabase(db_file_name);

        // Verifica se a inicialização da base de dados foi bem-sucedida
        // O construtor de SQLDatabase lança DataIOErrorException se falhar
        REQUIRE(db_instance->hasDb());

        // Inicializa as colunas esperadas para o loop de verificação
        auto cols_map = Dado_Jogador::get_sql_columns();
        for (const auto &pair : cols_map)
            expected_db_columns.insert(pair.first);
    }

    ~DatabaseFixture()
    {
        // Deleta a instância do banco de dados (fecha a conexão)
        delete db_instance;
        db_instance = nullptr;

        // Remove o arquivo físico do banco de dados
        remove(db_file_name.c_str());
    }

    // Função auxiliar para verificar um objeto retornado do DB
    void check_jogador_data(const objeto &obj, const string &expected_nome, const string &expected_apelido,
                            const string &expected_pontuacoes_str)
    {
        CHECK(obj.dados.count("NOME")); // Verifica se a chave existe (normalizada)
        CHECK(obj.dados.count("APELIDO"));
        CHECK(obj.dados.count("PONTUACOES"));

        CHECK(obj.dados.at("NOME") == expected_nome);
        CHECK(obj.dados.at("APELIDO") == expected_apelido);
        CHECK(obj.dados.at("PONTUACOES") == expected_pontuacoes_str);
    }
};

// =============================================================================
// Test Cases
// =============================================================================

DOCTEST_TEST_SUITE("JogadorSQLDatabase Tests")
{

    TEST_CASE_FIXTURE(DatabaseFixture, "Inicializacao e Criacao de Tabela")
    {
        REQUIRE(db_instance->hasDb());        // Já testado na fixture, mas reforça
        CHECK(db_instance->listar().empty()); // A tabela deve estar vazia após a criação
    }

    TEST_CASE_FIXTURE(DatabaseFixture, "Adicionar Jogador")
    {
        SUBCASE("Adicionar um jogador com pontuacoes vazias")
        {
            objeto player1_obj = create_jogador_objeto("Alice", "Ali"); // pontuacoes_str default ""
            CHECK(db_instance->adicionar(player1_obj));

            vector<objeto> players = db_instance->listar();
            REQUIRE(players.size() == 1);

            // Verifica se todas as colunas esperadas estão presentes e não há extras
            REQUIRE(players[0].dados.size() == expected_db_columns.size());
            for (const auto &pair : players[0].dados)
                CHECK(expected_db_columns.count(pair.first)); // Garante que a chave existe nas colunas esperadas

            check_jogador_data(players[0], "Alice", "Ali", "");
        }

        SUBCASE("Adicionar um jogador com pontuacoes")
        {
            objeto player2_obj = create_jogador_objeto("Bob", "Bobby", "0,1,2,3,5");
            CHECK(db_instance->adicionar(player2_obj));

            vector<objeto> players = db_instance->listar();
            REQUIRE(players.size() == 1); // Apenas um jogador, pois é um novo banco por teste

            check_jogador_data(players[0], "Bob", "Bobby", "0,1,2,3,5");
        }

        SUBCASE("Adicionar varios jogadores")
        {
            CHECK(db_instance->adicionar(create_jogador_objeto("Alice", "Ali", "10")));
            CHECK(db_instance->adicionar(create_jogador_objeto("Bob", "Bobby", "20,30")));
            CHECK(db_instance->adicionar(create_jogador_objeto("Charlie", "Char", "40,50,60")));

            vector<objeto> players = db_instance->listar();
            REQUIRE(players.size() == 3);

            // Verificar um jogador específico pelo apelido (PK)
            vector<objeto> found_ali = db_instance->buscar("APELIDO", "Ali");
            REQUIRE(found_ali.size() == 1);
            check_jogador_data(found_ali[0], "Alice", "Ali", "10");

            vector<objeto> found_bobby = db_instance->buscar("APELIDO", "Bobby");
            REQUIRE(found_bobby.size() == 1);
            check_jogador_data(found_bobby[0], "Bob", "Bobby", "20,30");
        }

        SUBCASE("Tentar adicionar jogador com apelido duplicado (PRIMARY KEY)")
        {
            objeto player1_obj = create_jogador_objeto("Alice", "Ali");
            REQUIRE(db_instance->adicionar(player1_obj)); // Deve adicionar com sucesso

            objeto player_duplicate_nickname_obj = create_jogador_objeto("Alicia", "Ali");
            // Espera-se que a adição falhe devido à chave primária duplicada
            CHECK_FALSE(db_instance->adicionar(player_duplicate_nickname_obj));
            CHECK(db_instance->listar().size() == 1); // O número de jogadores não deve mudar

            // Verifica se o jogador original ainda está lá, não o duplicado
            vector<objeto> players = db_instance->listar();
            REQUIRE(players.size() == 1);
            check_jogador_data(players[0], "Alice", "Ali", "");
        }
    }

    TEST_CASE_FIXTURE(DatabaseFixture, "Buscar Jogador")
    {
        db_instance->adicionar(create_jogador_objeto("Alice", "Ali", "10"));
        db_instance->adicionar(create_jogador_objeto("Bob", "Bobby", "20,30"));
        db_instance->adicionar(create_jogador_objeto("Alice", "Alicia", "40")); // Outra Alice, apelido diferente

        SUBCASE("Buscar por apelido existente (PRIMARY KEY)")
        {
            vector<objeto> results = db_instance->buscar("APELIDO", "Ali");
            REQUIRE(results.size() == 1);
            check_jogador_data(results[0], "Alice", "Ali", "10");
        }

        SUBCASE("Buscar por nome (nao eh PK, pode ter varios)")
        {
            vector<objeto> results = db_instance->buscar("NOME", "Alice");
            REQUIRE(results.size() == 2); // Deve encontrar "Ali" e "Alicia"
            // A ordem dos resultados não é garantida sem ORDER BY
            unordered_set<string> apelidos_encontrados;
            for (const auto &obj : results)
                apelidos_encontrados.insert(obj.dados.at("APELIDO"));
            CHECK(apelidos_encontrados.count("Ali"));
            CHECK(apelidos_encontrados.count("Alicia"));
        }

        SUBCASE("Buscar por apelido inexistente")
        {
            vector<objeto> results = db_instance->buscar("APELIDO", "Charlie");
            CHECK(results.empty());
        }

        SUBCASE("Buscar por chave invalida")
        {
            CHECK_THROWS_AS(db_instance->buscar("CHAVE_INVALIDA", "valor"), DataException);
        }
    }

    TEST_CASE_FIXTURE(DatabaseFixture, "Listar Todos os Jogadores")
    {
        REQUIRE(db_instance->listar().empty()); // Inicialmente vazio

        db_instance->adicionar(create_jogador_objeto("Alice", "Ali"));
        db_instance->adicionar(create_jogador_objeto("Bob", "Bobby"));
        db_instance->adicionar(create_jogador_objeto("Charlie", "Char"));

        vector<objeto> players = db_instance->listar();
        REQUIRE(players.size() == 3);

        // Usa um conjunto para verificar a presença dos apelidos, pois a ordem não é garantida
        unordered_set<string> found_apelidos;
        for (const auto &obj : players)
            found_apelidos.insert(obj.dados.at("APELIDO"));
        CHECK(found_apelidos.count("Ali"));
        CHECK(found_apelidos.count("Bobby"));
        CHECK(found_apelidos.count("Char"));
        CHECK(found_apelidos.size() == 3); // Garante que não há extras
    }

    TEST_CASE_FIXTURE(DatabaseFixture, "Atualizar Jogador")
    {
        objeto original_alice_obj = create_jogador_objeto("Alice", "Ali", "100");
        REQUIRE(db_instance->adicionar(original_alice_obj));

        SUBCASE("Atualizar nome e pontuacoes de jogador existente")
        {
            objeto updated_alice_obj = create_jogador_objeto("Alicia", "Ali", "150,200"); // Mesmo apelido
            CHECK(db_instance->atualizar(updated_alice_obj));

            vector<objeto> results = db_instance->buscar("APELIDO", "Ali");
            REQUIRE(results.size() == 1);
            check_jogador_data(results[0], "Alicia", "Ali", "150,200");
        }

        SUBCASE("Atualizar apenas um campo (nome)")
        {
            objeto partial_update_obj;
            partial_update_obj.dados["apelido"] = "Ali";        // Chave primária para encontrar
            partial_update_obj.dados["nome"] = "NovoNomeAlice"; // Apenas o nome para atualizar
            CHECK(db_instance->atualizar(partial_update_obj));

            vector<objeto> results = db_instance->buscar("APELIDO", "Ali");
            REQUIRE(results.size() == 1);
            check_jogador_data(results[0], "NovoNomeAlice", "Ali", "100"); // Pontuações devem permanecer as mesmas
        }

        SUBCASE("Tentar atualizar jogador que nao existe")
        {
            objeto non_existent_player_obj = create_jogador_objeto("Fake", "NoExist", "0");
            CHECK_FALSE(db_instance->atualizar(non_existent_player_obj));
            CHECK(db_instance->listar().size() == 1); // Quantidade não muda
        }

        SUBCASE("Tentar atualizar sem a chave primaria no objeto de entrada")
        {
            objeto malformed_obj;
            malformed_obj.dados["nome"] = "Nome"; // Sem apelido
            CHECK_THROWS_AS(db_instance->atualizar(malformed_obj), DataException);
        }
    }

    TEST_CASE_FIXTURE(DatabaseFixture, "Atualizar ou Adicionar (Upsert)")
    {
        SUBCASE("Adicionar novo jogador (nao existe)")
        {
            objeto new_player_obj = create_jogador_objeto("David", "Dave", "250");
            CHECK(db_instance->atualizar_ou_adicionar(new_player_obj));
            REQUIRE(db_instance->listar().size() == 1);
            check_jogador_data(db_instance->buscar("APELIDO", "Dave")[0], "David", "Dave", "250");
        }

        SUBCASE("Atualizar jogador existente")
        {
            objeto existing_player_obj = create_jogador_objeto("Eve", "Evie", "300");
            REQUIRE(db_instance->adicionar(existing_player_obj)); // Adiciona primeiro

            objeto updated_eve_obj = create_jogador_objeto("Evelyn", "Evie", "350,400"); // Mesmo apelido
            CHECK(db_instance->atualizar_ou_adicionar(updated_eve_obj));                 // Deve chamar 'atualizar'

            vector<objeto> results = db_instance->buscar("APELIDO", "Evie");
            REQUIRE(results.size() == 1);
            check_jogador_data(results[0], "Evelyn", "Evie", "350,400");
        }

        SUBCASE("Atualizar ou adicionar com objeto incompleto (sem PK)")
        {
            objeto incomplete_obj;
            incomplete_obj.dados["nome"] = "Incompleto"; // Falta apelido
            // O método buscar usado internamente por atualizar_ou_adicionar vai falhar se PK não estiver presente
            CHECK_THROWS_AS(db_instance->atualizar_ou_adicionar(incomplete_obj), DataException);
            CHECK(db_instance->listar().empty()); // Não deve ter adicionado nada
        }
    }

    TEST_CASE_FIXTURE(DatabaseFixture, "Excluir Jogador")
    {
        objeto frank_obj = create_jogador_objeto("Frank", "Frankie", "50");
        db_instance->adicionar(frank_obj);
        db_instance->adicionar(create_jogador_objeto("Grace", "Gracie", "60"));
        REQUIRE(db_instance->listar().size() == 2);

        SUBCASE("Excluir jogador existente")
        {
            CHECK(db_instance->excluir(frank_obj));   // Exclui pelo apelido
            CHECK(db_instance->listar().size() == 1); // Resta apenas Grace
            vector<objeto> remaining_players = db_instance->listar();
            REQUIRE(remaining_players.size() == 1);
            check_jogador_data(remaining_players[0], "Grace", "Gracie", "60");
        }

        SUBCASE("Tentar excluir jogador que nao existe")
        {
            objeto non_existent_delete_obj = create_jogador_objeto("NonExistent", "NonExistent", "0");
            CHECK_FALSE(db_instance->excluir(non_existent_delete_obj));
            CHECK(db_instance->listar().size() == 2); // Não deve mudar o tamanho
        }

        SUBCASE("Tentar excluir sem a chave primaria no objeto de entrada")
        {
            objeto malformed_obj;
            malformed_obj.dados["nome"] = "NomeQualquer"; // Falta apelido (PK)
            CHECK_THROWS_AS(db_instance->excluir(malformed_obj), DataException);
            CHECK(db_instance->listar().size() == 2); // Não deve mudar o tamanho
        }
    }

    TEST_CASE_FIXTURE(DatabaseFixture, "Limpar Tabela")
    {
        db_instance->adicionar(create_jogador_objeto("Grace", "Gracie", "50"));
        db_instance->adicionar(create_jogador_objeto("Heidi", "H", "60"));
        REQUIRE(db_instance->listar().size() == 2);

        CHECK(db_instance->limpar());         // Limpa todos os registros
        CHECK(db_instance->listar().empty()); // Tabela deve estar vazia após limpar

        // Testar limpar tabela já vazia
        CHECK(db_instance->limpar());
        CHECK(db_instance->listar().empty());
    }

    TEST_CASE_FIXTURE(DatabaseFixture, "Listar Ordenado")
    {
        db_instance->adicionar(create_jogador_objeto("Zulu", "Z", "10"));
        db_instance->adicionar(create_jogador_objeto("Alpha", "A", "30"));
        db_instance->adicionar(create_jogador_objeto("Charlie", "C", "20"));
        db_instance->adicionar(create_jogador_objeto("Bravo", "B", "5")); // Para testar ordem de apelido

        SUBCASE("Ordenar por APELIDO crescente (chave primaria)")
        {
            vector<objeto> players = db_instance->listar_ordenado("APELIDO", true);
            REQUIRE(players.size() == 4);
            CHECK(players[0].dados.at("APELIDO") == "A");
            CHECK(players[1].dados.at("APELIDO") == "B");
            CHECK(players[2].dados.at("APELIDO") == "C");
            CHECK(players[3].dados.at("APELIDO") == "Z");
        }

        SUBCASE("Ordenar por APELIDO decrescente")
        {
            vector<objeto> players = db_instance->listar_ordenado("APELIDO", false);
            REQUIRE(players.size() == 4);
            CHECK(players[0].dados.at("APELIDO") == "Z");
            CHECK(players[1].dados.at("APELIDO") == "C");
            CHECK(players[2].dados.at("APELIDO") == "B");
            CHECK(players[3].dados.at("APELIDO") == "A");
        }

        SUBCASE("Ordenar por NOME crescente")
        {
            vector<objeto> players = db_instance->listar_ordenado("NOME", true);
            REQUIRE(players.size() == 4);
            CHECK(players[0].dados.at("NOME") == "Alpha");
            CHECK(players[1].dados.at("NOME") == "Bravo");
            CHECK(players[2].dados.at("NOME") == "Charlie");
            CHECK(players[3].dados.at("NOME") == "Zulu");
        }

        SUBCASE("Ordenar por NOME decrescente")
        {
            vector<objeto> players = db_instance->listar_ordenado("NOME", false);
            REQUIRE(players.size() == 4);
            CHECK(players[0].dados.at("NOME") == "Zulu");
            CHECK(players[1].dados.at("NOME") == "Charlie");
            CHECK(players[2].dados.at("NOME") == "Bravo");
            CHECK(players[3].dados.at("NOME") == "Alpha");
        }

        SUBCASE("Ordenar por PONTUACOES crescente (se for TEXT, ordem alfabética)")
        {
            db_instance->limpar(); // Limpa para testar apenas por pontuacoes
            db_instance->adicionar(create_jogador_objeto("P1", "P1", "100"));
            db_instance->adicionar(create_jogador_objeto("P2", "P2", "20"));
            db_instance->adicionar(create_jogador_objeto("P3", "P3", "5"));
            db_instance->adicionar(create_jogador_objeto("P4", "P4", "15"));
            db_instance->adicionar(create_jogador_objeto("P5", "P5", "50"));

            vector<objeto> players = db_instance->listar_ordenado("PONTUACOES", true);
            REQUIRE(players.size() == 5);
            // SQLite ordena TEXT lexicograficamente. "10" < "2" < "5"
            CHECK(players[0].dados.at("PONTUACOES") == "100"); // "10" vem antes de "20" ou "5"
            CHECK(players[1].dados.at("PONTUACOES") == "15");
            CHECK(players[2].dados.at("PONTUACOES") == "20");
            CHECK(players[3].dados.at("PONTUACOES") == "5");
            CHECK(players[4].dados.at("PONTUACOES") == "50");
        }

        SUBCASE("Ordenar por chave inválida")
        {
            // A função get_sql_type_from_schema imprime um erro em cerr se a chave não for válida.
            // O método listar_ordenado deve retornar um vetor vazio.
            CHECK_THROWS_AS(db_instance->listar_ordenado("CHAVE_INEXISTENTE", true), DataException);
        }
    }

    // TEST_CASE_FIXTURE(DatabaseFixture, "Gerenciamento de Erros e Exceções") {
    //     SUBCASE("Abrir DB com nome de arquivo inválido") {
    //         // Tentar abrir um DB com nome de arquivo inválido
    //         // Isso deveria ser testado no construtor de SQLDatabase diretamente
    //         // fora da fixture, pois a fixture já assume que o DB abre.
    //         // Ex:
    //         // CHECK_THROWS_AS(JogadorSQLDatabase("nao/existe/path/db.db"), DataIOErrorException);
    //     }

    //     SUBCASE("Operacao em DB nao aberto") {
    //         // Criar um JogadorSQLDatabase, mas fechar o DB explicitamente ou simular falha na abertura
    //         // Isso exigiria uma forma de simular falhas ou uma instância de SQLDatabase diretamente
    //         // para testar esses cenários específicos.
    //     }
    // }

} // End DOCTEST_TEST_SUITE("JogadorSQLDatabase Tests")