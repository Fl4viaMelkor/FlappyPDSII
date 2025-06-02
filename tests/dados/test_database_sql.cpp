//
// Created by leand on 01/06/2025.
//
#include "../doctest.h"
#include "../../include/dados/sqlite3.h"
#include <iostream>
#include "../../include/dados/database.hpp" // Inclui o cabeçalho da classe SQLDatabase e JogadorSQLDatabase
#include "../../include/dados/dados.hpp"      // Inclui o cabeçalho da struct 'objeto' e Dado_Jogador
#include <ctime>  // For seeding

#include <random>
#include <string>        // Para string
#include <vector>        // Para vector
#include <unordered_map> // Para unordered_map
#include <iostream>      // Para cout, cerr
#include <fstream>       // Para remove (deletar arquivo)
#include <stdexcept>     // Para runtime_error
#include <unordered_set>
using namespace std;

// =============================================================================
// Funções Auxiliares para Testes
// =============================================================================

// Cria um objeto 'objeto' com dados de jogador
objeto create_jogador_objeto(const int id, const string &nome, const string &apelido, int vitorias = 0,
                             int derrotas = 0,
                             const string &pontuacoes_str = "") {
    objeto obj;
    obj.dados["id"] = to_string(id);
    obj.dados["NOME"] = nome;
    obj.dados["APELIDO"] = apelido;
    obj.dados["VITORIAS"] = to_string(vitorias);
    obj.dados["DERROTAS"] = to_string(derrotas);
    obj.dados["PONTUACOES"] = pontuacoes_str;
    return obj;
}

vector<string> colunas = Dado_Jogador::get_colunas();

// =============================================================================
// Fixture para Testes de Banco de Dados
// =============================================================================
// Esta fixture garante que um novo arquivo de banco de dados seja criado antes
// de cada TEST_CASE e deletado após sua execução, garantindo isolamento.

struct DatabaseFixture {
    string db_file_name;
    JogadorSQLDatabase *db_instance;


    DatabaseFixture() : db_file_name("test_db_fixture_0.db") {
        // Garante que o arquivo não exista de uma execução anterior falha
        remove(db_file_name.c_str());

        // Instancia o banco de dados para o teste.
        // O construtor de JogadorSQLDatabase já abre o DB e cria a tabela.
        db_instance = new JogadorSQLDatabase(db_file_name);

        // Verifica se a inicialização da base de dados foi bem-sucedida
        // (o construtor de SQLDatabase imprime erros para cerr se falhar)
        REQUIRE(db_instance->hasDb());
    }

    ~DatabaseFixture() {
        // Deleta a instância do banco de dados (fecha a conexão)
        delete db_instance;
        db_instance = nullptr;

        // Remove o arquivo físico do banco de dados
        remove(db_file_name.c_str());
    }
};

// =============================================================================
// Test Cases
// =============================================================================

TEST_CASE_FIXTURE(DatabaseFixture, "JogadorSQLDatabase: Inicializacao e Criacao de Tabela") {
    // O construtor da fixture já testa a inicialização e criação da tabela.
    // Se a fixture foi construída com sucesso, o banco de dados está aberto e a tabela criada.
    REQUIRE(db_instance->hasDb());

    // Tenta listar para ver se a tabela está vazia (como esperado após a criação)
    vector<objeto> players = db_instance->listar();
    CHECK(players.empty());
}

TEST_CASE_FIXTURE(DatabaseFixture, "JogadorSQLDatabase: Adicionar Jogador") {
    objeto player1 = create_jogador_objeto(0, "Alice", "Ali", 10, 2);
    CHECK(db_instance->adicionar(player1));

    vector<objeto> players = db_instance->listar();
    REQUIRE(players.size() == 1);
    CHECK(players[0].dados.size() == colunas.size());
    // Create a set from the vector
    unordered_set<string> vecSet(colunas.begin(), colunas.end());
    // Check each map key exists in the set
    for (const auto &pair: players[0].dados)
        CHECK(vecSet.find(pair.first) != vecSet.end());

    // Valores são strings no objeto
    CHECK(players[0].dados.at("NOME") == "Alice");
    CHECK(players[0].dados.at("VITORIAS") == "10");
    CHECK(players[0].dados.at("APELIDO") == "Ali");
    CHECK(players[0].dados.at("DERROTAS") == "2");
    CHECK(players[0].dados.at("PONTUACOES") == "");
    objeto player2 = create_jogador_objeto(0, "Bob", "Bobby", 5, 5, "0,1,2,3,5");
    CHECK(db_instance->adicionar(player2));
    players = db_instance->listar();

    REQUIRE(players.size() == 2);
    CHECK(players[1].dados.at("NOME") == "Bob");
    CHECK(players[1].dados.at("VITORIAS") == "5");
    CHECK(players[1].dados.at("APELIDO") == "Bobby");
    CHECK(players[1].dados.at("DERROTAS") == "5");
    CHECK(players[1].dados.at("PONTUACOES") == "0,1,2,3,5");

    // Tentar adicionar jogador com apelido duplicado (PRIMARY KEY)
    objeto player_duplicate_nickname = create_jogador_objeto(0, "Alicia", "Ali", 12, 3);
    // A função adicionar deve retornar false e o SQLite deve reportar um erro de UNIQUE constraint
    // (A saída de erro para cerr é esperada aqui)
    CHECK_FALSE(db_instance->adicionar(player_duplicate_nickname));
    CHECK(db_instance->listar().size() == 2); // O número de jogadores não deve mudar
}


TEST_CASE_FIXTURE(DatabaseFixture, "JogadorSQLDatabase: Buscar Jogador") {
    db_instance->adicionar(create_jogador_objeto(0, "Alice", "Ali", 10, 2));
    db_instance->adicionar(create_jogador_objeto(0, "Bob", "Bobby", 5, 5));
    db_instance->adicionar(create_jogador_objeto(0, "Bob", "Bob", 12, 3)); // Outra Alice

    SUBCASE("Buscar por nome existente") {
        vector<objeto> results = db_instance->buscar("nome", "Alice");
        REQUIRE(results.size() == 1);
        CHECK(results[0].dados.at("APELIDO") == "Ali");
        CHECK(results[0].dados.at("VITORIAS") == "10");
    }

    SUBCASE("Buscar por nome inexistente") {
        vector<objeto> results = db_instance->buscar("NOME", "Charlie");
        CHECK(results.empty());
    }

    SUBCASE("Buscar por apelido (não-único)") {
        vector<objeto> results = db_instance->buscar("NOME", "Bob");
        REQUIRE(results.size() == 2); // Deve encontrar Bob e Bobby
        // A ordem dos resultados não é garantida sem ORDER BY na busca
        bool found_bob = false;
        bool found_bobby = false;
        for (const auto &obj: results) {
            if (obj.dados.at("APELIDO") == "Bob")
                found_bob = true;
            if (obj.dados.at("APELIDO") == "Bobby")
                found_bobby = true;
        }
        CHECK(found_bob);
        CHECK(found_bobby);
    }
}

TEST_CASE_FIXTURE(DatabaseFixture, "JogadorSQLDatabase: Listar Todos os Jogadores") {
    REQUIRE(db_instance->listar().empty()); // Inicialmente vazio

    db_instance->adicionar(create_jogador_objeto(0, "Alice", "Ali"));
    db_instance->adicionar(create_jogador_objeto(0, "Bob", "Bobby"));
    db_instance->adicionar(create_jogador_objeto(0, "Charlie", "Char"));

    vector<objeto> players = db_instance->listar();
    REQUIRE(players.size() == 3);

    // Verifica se os nomes esperados estão presentes (ordem não garantida)
    bool found_alice = false, found_bob = false, found_charlie = false;
    for (const auto &obj: players) {
        if (obj.dados.at("NOME") == "Alice")
            found_alice = true;
        if (obj.dados.at("NOME") == "Bob")
            found_bob = true;
        if (obj.dados.at("NOME") == "Charlie")
            found_charlie = true;
    }
    CHECK(found_alice);
    CHECK(found_bob);
    CHECK(found_charlie);
}


TEST_CASE_FIXTURE(DatabaseFixture, "JogadorSQLDatabase: Atualizar Jogador") {
    db_instance->adicionar(create_jogador_objeto(0, "Alice", "Ali", 10, 2));

    // Tenta atualizar jogador com apelido duplicado (PRIMARY KEY)
    objeto updated_alice = create_jogador_objeto(0, "Alice", "Wonder", 12, 3, "100,120");
    CHECK_FALSE(db_instance->atualizar(updated_alice));

    updated_alice = create_jogador_objeto(0, "Alice", "Ali", 12, 3, "100,120");
    CHECK(db_instance->atualizar(updated_alice));


    vector<objeto> results = db_instance->buscar("APELIDO", "Ali");
    REQUIRE(results.size() == 1);
    CHECK(results[0].dados.at("NOME") == "Alice");
    CHECK(results[0].dados.at("APELIDO") == "Ali");
    CHECK(results[0].dados.at("VITORIAS") == "12");
    CHECK(results[0].dados.at("DERROTAS") == "3");
    CHECK(results[0].dados.at("PONTUACOES") == "100,120");

    // Tentar atualizar jogador que não existe (deve retornar false ou não afetar nada)
    objeto non_existent_player = create_jogador_objeto(999, "Fake", "Player", 0, 0);
    CHECK_FALSE(db_instance->atualizar(non_existent_player));
}


TEST_CASE_FIXTURE(DatabaseFixture, "JogadorSQLDatabase: Atualizar ou Adicionar") {
    SUBCASE("Adicionar novo jogador") {
        objeto new_player = create_jogador_objeto(0, "David", "Dave", 20, 1);
        CHECK(db_instance->atualizar_ou_adicionar(new_player));
        REQUIRE(db_instance->listar().size() == 1);
        CHECK(db_instance->buscar("NOME", "David")[0].dados.at("VITORIAS") == "20");
    }

    SUBCASE("Atualizar jogador existente") {
        db_instance->adicionar(create_jogador_objeto(0, "Eve", "Evie", 30, 5));

        objeto updated_eve = create_jogador_objeto(0, "Eve", "Evelyn", 35, 6);
        CHECK(db_instance->atualizar_ou_adicionar(updated_eve)); // Deve chamar 'adicionar'

        vector<objeto> results = db_instance->buscar("NOME", "Eve");
        REQUIRE(results.size() == 2);

        updated_eve = create_jogador_objeto(0, "Eve", "Evelyn", 3, 1);
        CHECK(db_instance->atualizar_ou_adicionar(updated_eve)); // Deve chamar 'atualizar'

        results = db_instance->buscar("NOME", "Eve");
        REQUIRE(results.size() == 2);

        for (const auto &obj: results) {
            if (obj.dados.at("APELIDO") == "Evelyn") {
                CHECK(obj.dados.at("VITORIAS") == "3");
                CHECK(obj.dados.at("DERROTAS") == "1");
            }
            else {
                CHECK(obj.dados.at("APELIDO") == "Evie");
                CHECK(obj.dados.at("VITORIAS") == "30");
                CHECK(obj.dados.at("DERROTAS") == "5");
            }
        }
    }
}

TEST_CASE_FIXTURE(DatabaseFixture, "JogadorSQLDatabase: Excluir Jogador") {
    db_instance->adicionar(create_jogador_objeto(0, "Frank", "Frankie", 40, 10));

    objeto frank_to_delete = create_jogador_objeto(0, "", "Frankie", 0, 0);
    // Apenas ID é necessário para exclusão
    CHECK(db_instance->excluir(frank_to_delete));

    CHECK(db_instance->buscar("nome", "Frank").empty()); // Não deve mais encontrar Frank
    CHECK(db_instance->listar().empty()); // A tabela deve estar vazia agora

    //Jogadores homonimos não devem ser afetados
    db_instance->adicionar(create_jogador_objeto(0, "Frank", "Frankie"));
    db_instance->adicionar(create_jogador_objeto(0, "Frank", "Frank"));
    CHECK(db_instance->excluir(frank_to_delete));
    CHECK(db_instance->buscar("nome", "Frank").size() == 1); // Deve existir apenas um Frank


    // Tentar excluir jogador que não existe (deve retornar false ou não afetar nada)
    objeto non_existent_delete = create_jogador_objeto(999, "NonExistent", "", 0, 0);
    CHECK_FALSE(db_instance->excluir(non_existent_delete));
}


TEST_CASE_FIXTURE(DatabaseFixture, "JogadorSQLDatabase: Limpar Tabela") {
    db_instance->adicionar(create_jogador_objeto(0, "Grace", "Gracie", 50, 0));
    db_instance->adicionar(create_jogador_objeto(0, "Heidi", "H", 60, 1));
    REQUIRE(db_instance->listar().size() == 2);

    CHECK(db_instance->limpar());
    CHECK(db_instance->listar().empty()); // Tabela deve estar vazia após limpar
}


TEST_CASE_FIXTURE(DatabaseFixture, "JogadorSQLDatabase: Listar Ordenado") {
    db_instance->adicionar(create_jogador_objeto(0, "Zulu", "Z", 10, 0));
    db_instance->adicionar(create_jogador_objeto(0, "Alpha", "A", 30, 0));
    db_instance->adicionar(create_jogador_objeto(0, "Charlie", "C", 20, 0));

    SUBCASE("Ordenar por nome crescente") {
        vector<objeto> players = db_instance->listar_ordenado("NOME", true);
        REQUIRE(players.size() == 3);
        CHECK(players[0].dados.at("NOME") == "Alpha");
        CHECK(players[1].dados.at("NOME") == "Charlie");
        CHECK(players[2].dados.at("NOME") == "Zulu");
    }

    SUBCASE("Ordenar por nome decrescente") {
        vector<objeto> players = db_instance->listar_ordenado("NOME", false);
        REQUIRE(players.size() == 3);
        CHECK(players[0].dados.at("NOME") == "Zulu");
        CHECK(players[1].dados.at("NOME") == "Charlie");
        CHECK(players[2].dados.at("NOME") == "Alpha");
    }

    SUBCASE("Ordenar por vitorias crescente") {
        vector<objeto> players = db_instance->listar_ordenado("VITORIAS", true);
        REQUIRE(players.size() == 3);
        CHECK(players[0].dados.at("VITORIAS") == "10"); // Zulu
        CHECK(players[1].dados.at("VITORIAS") == "20"); // Charlie
        CHECK(players[2].dados.at("VITORIAS") == "30"); // Alpha
    }

    SUBCASE("Ordenar por vitorias decrescente") {
        vector<objeto> players = db_instance->listar_ordenado("VITORIAS", false);
        REQUIRE(players.size() == 3);
        CHECK(players[0].dados.at("VITORIAS") == "30"); // Alpha
        CHECK(players[1].dados.at("VITORIAS") == "20"); // Charlie
        CHECK(players[2].dados.at("VITORIAS") == "10"); // Zulu
    }

    SUBCASE("Ordenar por chave inválida") {
        // A função deve imprimir um erro para cerr e retornar um vetor vazio
        vector<objeto> players = db_instance->listar_ordenado("chave_invalida", true);
        CHECK(players.empty());
    }
}
