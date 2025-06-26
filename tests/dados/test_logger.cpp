//
// Created by leand on 25/06/2025.
//
//
// Created by leand on 25/05/2025.
//

#include "../doctest.h"
#include <algorithm> // Para std::max_element, std::sort (no seu c├│digo de classe)
#include <fstream>   // Para std::remove
#include <iostream>
#include <limits> // Para std::numeric_limits
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

// Inclui as classes a serem testadas
#include "../../include/dados/dados.hpp" // Para Dado_Jogador e objeto
#include "../../include/dados/data_exception.hpp"
#include "../../include/dados/database.hpp"
#include "../../include/dados/logger.hpp"

using namespace std;

// =============================================================================
// Funções Auxiliares para Testes (Reutilizadas e Adaptadas)
// =============================================================================

// Helper para criar um objeto 'objeto' com dados de jogador
// Adaptação para o schema de Dado_Jogador (sem 'id', com apelido como PK)
objeto create_jogador_objeto_for_db(const string &nome, const string &apelido, const string &pontuacoes_str = "")
{
    objeto obj;
    obj.dados["nome"] = nome;
    obj.dados["apelido"] = apelido; // Apelido é a chave primária
    obj.dados["pontuacoes"] = pontuacoes_str;
    return obj;
}

// Helper para criar uma instância de Dado_Jogador (útil para inicializar 'atual_')
Dado_Jogador *create_dado_jogador_instance(const string &nome, const string &apelido,
                                           const vector<int> &pontuacoes = {})
{
    Dado_Jogador *jogador = new Dado_Jogador(nome, apelido);
    jogador->pontuacoes(pontuacoes); // Define as pontuações iniciais
    return jogador;
}

// =============================================================================
// Fixture para Testes de Banco de Dados
// =============================================================================
// Garante que cada teste de Logger/PlayerLogger tenha seu próprio DB limpo.

struct LoggerDatabaseFixture {
    string db_file_name;
    JogadorSQLDatabase *db_real_instance; // O banco de dados real que o Logger usará
    PlayerLogger *player_logger_instance; // A instância do PlayerLogger a ser testada

    LoggerDatabaseFixture()
    {
        // Cria um nome de arquivo de DB único para cada instância da fixture
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 999999);
        db_file_name = "test_logger_db_fixture_" + to_string(distrib(gen)) + ".db";

        // Garante que o arquivo DB não exista de uma execução anterior falha
        remove(db_file_name.c_str());

        // 1. Instancia o banco de dados real para o teste
        db_real_instance = new JogadorSQLDatabase(db_file_name);
        // O construtor de SQLDatabase pode lançar DataIOErrorException, então REQUIRE garante que ele abra.
        REQUIRE(db_real_instance->hasDb());

        // 2. Instancia o PlayerLogger, associando-o ao DB real
        player_logger_instance = new PlayerLogger(db_real_instance);
        REQUIRE(player_logger_instance != nullptr); // Garante que o logger foi construído
        // O construtor de PlayerLogger imprime para cout, o que é OK em testes
    }

    ~LoggerDatabaseFixture()
    {
        // Ordem de deleção é importante para evitar double-free ou use-after-free:
        // O PlayerLogger (que gerencia 'atual_') deve ser deletado primeiro.
        // Em seguida, o Database (que fecha a conexão com o arquivo).
        if (player_logger_instance) {
            delete player_logger_instance;
            player_logger_instance = nullptr;
        }

        if (db_real_instance) {
            delete db_real_instance;
            db_real_instance = nullptr;
        }
        // Por último, remove o arquivo físico do banco de dados
        remove(db_file_name.c_str());
    }

    // Função auxiliar para verificar um jogador no DB real através do apelido
    void check_player_in_db(const string &apelido, const string &expected_nome, const string &expected_pontuacoes_str)
    {
        vector<objeto> results = db_real_instance->buscar("APELIDO", apelido);
        REQUIRE(results.size() == 1); // Garante que o jogador foi encontrado e é único
        CHECK(results[0].dados.at("NOME") == expected_nome);
        CHECK(results[0].dados.at("APELIDO") == apelido);
        CHECK(results[0].dados.at("PONTUACOES") == expected_pontuacoes_str);
    }
};

// =============================================================================
// Test Cases para Logger e PlayerLogger
// =============================================================================

DOCTEST_TEST_SUITE("Logger & PlayerLogger Tests")
{

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: Construtor e Reset")
    {
        // A fixture já inicializa o PlayerLogger. current_match_state deve ter um Dado_Jogador padrão.
        // getPartidas() e getHighScorePlayer() devem refletir um Dado_Jogador vazio
        CHECK(player_logger_instance->getPartidasDisputadas() == 0);

        CHECK(player_logger_instance->getHighScorePlayer() == -1); // -1 é o padrão para pontuações vazias

        SUBCASE("Resetar PlayerLogger para o estado padrão")
        {
            // Simular um jogador carregado e com pontuações
            // Carregar um jogador inexistente fará com que um novo Dado_Jogador(apelido, apelido) seja criado
            player_logger_instance->carregar("TempPlayer"); // apelido: TempPlayer
            player_logger_instance->salvar(100);            // Adiciona pontuação e salva no DB

            // Verifica se 'atual_' está no estado esperado após salvar
            CHECK(player_logger_instance->getPartidasDisputadas() == 1);
            CHECK(player_logger_instance->getHighScorePlayer() == 100);

            player_logger_instance->resetar(); // Reseta o estado interno de 'atual_'
            CHECK(player_logger_instance->getPartidasDisputadas() == 0);
            CHECK(player_logger_instance->getHighScorePlayer() == -1); // Deve voltar ao estado inicial

            // O jogador 'TempPlayer' ainda existe no DB, apenas 'atual_' foi resetado
            check_player_in_db("TempPlayer", "TempPlayer", "100");
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: carregar()")
    {
        SUBCASE("Carregar jogador existente com sucesso")
        {
            // Prepara o DB com um jogador
            objeto existing_player_obj = create_jogador_objeto_for_db("Alice", "Ali", "10,20,30");
            REQUIRE(db_real_instance->adicionar(existing_player_obj));
            // Tenta carregar o jogador pelo apelido
            player_logger_instance->carregar("Ali");
            CHECK(player_logger_instance->getPartidasDisputadas() == 3);
            CHECK(player_logger_instance->getHighScorePlayer() == 30); // Maior pontuação: 30
        }
        SUBCASE("Carregar jogador existente e verificar estado apos recarregar")
        {
            objeto existing_player_obj = create_jogador_objeto_for_db("Bob", "Bobby", "100,150");
            REQUIRE(db_real_instance->adicionar(existing_player_obj));
            cout << "CARREGANDO BOBBY" << endl;
            player_logger_instance->carregar("Bobby"); // Carrega inicialmente
            player_logger_instance->salvar(170);       // Adiciona 170 e salva no DB (pontuações: 100,150,170)
                                                       // Verifica se o DB foi atualizado
            check_player_in_db("Bobby", "Bob", "100,150,170");
            // Recarrega o mesmo jogador (o ponteiro 'atual_' deve ser deletado e recriado corretamente)
            player_logger_instance->carregar("Bobby");
            CHECK(player_logger_instance->getPartidasDisputadas() == 3);
            CHECK(player_logger_instance->getHighScorePlayer() == 170); // Deve ter a nova maior pontuação
        }
        SUBCASE("Carregar jogador inexistente (deve retornar false e criar Dado_Jogador padrão)")
        {
            // A lógica do seu carregar atualiza 'atual_' para um novo Dado_Jogador(apelido, apelido) e retorna false
            player_logger_instance->carregar("NovoApelido");
            // Verifica se 'atual_' foi setado para um Dado_Jogador com nome/apelido 'NovoApelido'
            // Testamos isso indiretamente através dos getters do PlayerLogger
            CHECK(player_logger_instance->getPartidasDisputadas() == 0); // O novo jogador não tem partidas ainda
            CHECK(player_logger_instance->getHighScorePlayer() == -1);
            // Se este jogador for salvo agora, ele será adicionado ao DB com esse nome/apelido
            player_logger_instance->salvar(5);
            check_player_in_db("NovoApelido", "NovoApelido", "5");
        }
        SUBCASE("Carregar jogador com dados malformatados no DB (deve retornar false e logar erro)")
        {
            // Simular um objeto malformado no DB (ex: pontuacoes invalidas)
            objeto malformed_obj = create_jogador_objeto_for_db("Malformed", "Mal", "10,abc,20");
            REQUIRE(db_real_instance->adicionar(malformed_obj)); // Tentar adicionar (sqlite pode aceitar texto)

            // Quando carregar, a desserialização (Dado_Jogador(obj) -> carregar()) vai falhar
            // O catch interno no PlayerLogger::carregar vai imprimir erro e retornar false
            player_logger_instance->carregar("Mal");
            // 'atual_' deve ser nulo após a falha de desserialização
            // (Sua implementação atual define atual_=nullptr em caso de catch)
            CHECK(player_logger_instance->getPartidasDisputadas() ==
                  0); // Se atual_ for nulo, deve retornar 0 ou um erro
            CHECK(player_logger_instance->getHighScorePlayer() ==
                  -1); // Se atual_ for nulo, deve retornar -1 ou um erro
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: getHighScorePlayer()")
    {
        SUBCASE("High Score de jogador recem-carregado")
        {
            player_logger_instance->carregar("JogadorA");
            player_logger_instance->salvar(10);
            player_logger_instance->salvar(50);
            player_logger_instance->salvar(20);
            CHECK(player_logger_instance->getHighScorePlayer() == 50);
        }
        SUBCASE("High Score de jogador com pontuacoes vazias")
        {
            player_logger_instance->carregar("JogadorB");              // Pontuacoes vazias no 'atual_'
            CHECK(player_logger_instance->getHighScorePlayer() == -1); // Deve retornar -1 conforme Dado_Jogador
        }
        SUBCASE("High Score de jogador com pontuacoes negativas")
        {
            // Cria um jogador diretamente, adiciona ao DB e carrega
            objeto neg_player_obj = create_jogador_objeto_for_db("Negativo", "Neg", "-5,-10,-2");
            db_real_instance->adicionar(neg_player_obj);

            player_logger_instance->carregar("Neg");                   // Carrega do DB
            CHECK(player_logger_instance->getHighScorePlayer() == -2); // A maior é -2
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: getHighScoreAll()")
    {
        SUBCASE("High Score geral com varios jogadores")
        {
            db_real_instance->adicionar(create_jogador_objeto_for_db("J1", "P1", "10,20,5")); // Max 20
            db_real_instance->adicionar(create_jogador_objeto_for_db("J2", "P2", "30,15"));   // Max 30
            db_real_instance->adicionar(create_jogador_objeto_for_db("J3", "P3", "5,40,10")); // Max 40
            db_real_instance->adicionar(create_jogador_objeto_for_db("J4", "P4", ""));        // Sem pontuacoes
            CHECK(player_logger_instance->getHighScoreAll() == 40);
        }

        SUBCASE("High Score geral com DB vazio")
        {
            CHECK(db_real_instance->listar().empty());              // Garante que o DB está vazio
            CHECK(player_logger_instance->getHighScoreAll() == -1); // Retorno esperado para DB vazio
        }

        SUBCASE("High Score geral com apenas um jogador e pontuacoes vazias")
        {
            db_real_instance->adicionar(create_jogador_objeto_for_db("J5", "P5", ""));
            CHECK(player_logger_instance->getHighScoreAll() == -1);
        }

        SUBCASE("High Score geral com todos os jogadores com pontuacoes negativas")
        {
            db_real_instance->adicionar(create_jogador_objeto_for_db("N1", "N1", "-10,-5")); // Max -5
            db_real_instance->adicionar(create_jogador_objeto_for_db("N2", "N2", "-20,-1")); // Max -1
            CHECK(player_logger_instance->getHighScoreAll() == -1); // -1 é o default para max_element de vazios
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: isHighScorePlayer e isHighScoreAll")
    {
        // Configura um jogador no 'atual_' e outros no DB para testar high scores
        db_real_instance->adicionar(
          create_jogador_objeto_for_db("Max", "Maxi", "100,50,120")); // Player high no DB: 120
        db_real_instance->adicionar(
          create_jogador_objeto_for_db("Global", "Glob", "200,10,30")); // Global high no DB: 200
        player_logger_instance->carregar("Maxi");                       // Carrega o jogador Maxi (current high: 120)
        SUBCASE("isHighScorePlayer")
        {
            CHECK_FALSE(player_logger_instance->isHighScorePlayer(100)); // Menor que 120
            CHECK_FALSE(player_logger_instance->isHighScorePlayer(120)); // Não é estritamente MAIOR (se usar >)
            CHECK(player_logger_instance->isHighScorePlayer(121));       // É maior
        }
        SUBCASE("isHighScoreAll")
        {
            // Sua implementação: return novo_score >= getHighScoreAll();
            // getHighScoreAll() deve retornar 200 neste cenário
            CHECK_FALSE(player_logger_instance->isHighScoreAll(199)); // Menor que 200
            CHECK(player_logger_instance->isHighScoreAll(200));       // É igual (então é true)
            CHECK(player_logger_instance->isHighScoreAll(201));       // É maior
        }
        SUBCASE("isHighScore com DB vazio")
        {
            db_real_instance->limpar(); // Limpa o DB
            // Carrega um novo jogador para 'atual_' (sem pontuações)
            player_logger_instance->carregar("EmptyPlayer");

            CHECK(player_logger_instance->isHighScorePlayer(1)); // 1 > -1
            CHECK(player_logger_instance->isHighScoreAll(1));    // 1 >= -1
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: getPartidas()")
    {
        SUBCASE("Partidas de jogador recem-carregado")
        {
            player_logger_instance->carregar("PartidasA");               // Cria novo Dado_Jogador
            CHECK(player_logger_instance->getPartidasDisputadas() == 0); // Nenhuma partida ainda

            player_logger_instance->salvar(10); // Adiciona 1 partida
            CHECK(player_logger_instance->getPartidasDisputadas() == 1);

            player_logger_instance->salvar(20); // Adiciona 1 partida
            player_logger_instance->salvar(30); // Adiciona 1 partida
            CHECK(player_logger_instance->getPartidasDisputadas() == 3);
        }

        SUBCASE("Partidas de jogador carregado do DB")
        {
            db_real_instance->adicionar(create_jogador_objeto_for_db("ComPartidas", "CP", "1,2,3,4,5"));
            player_logger_instance->carregar("CP");
            CHECK(player_logger_instance->getPartidasDisputadas() == 5);
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "Logger (Base): deletar()")
    {
        string apelido1 = "DM", apelido2 = "KM"; // Usar strings para apelidos
        objeto p1 = create_jogador_objeto_for_db("DeleteMe", apelido1, "10");
        objeto p2 = create_jogador_objeto_for_db("KeepMe", apelido2, "20");
        REQUIRE(db_real_instance->adicionar(p1));
        REQUIRE(db_real_instance->adicionar(p2));
        REQUIRE(db_real_instance->listar().size() == 2);
        SUBCASE("Deletar jogador existente")
        {
            // 'deletar' na classe base Logger usa o apelido para exclusão
            CHECK(player_logger_instance->deletar(apelido1)); // Note: apelido1 é string&
            CHECK(db_real_instance->listar().size() == 1);
            check_player_in_db(apelido2, "KeepMe", "20"); // Verifica que o outro jogador permanece
        }
        SUBCASE("Deletar jogador inexistente")
        {
            string inexistente_apelido = "NaoExiste";
            CHECK_FALSE(player_logger_instance->deletar(inexistente_apelido)); // Retorna false
            CHECK(db_real_instance->listar().size() == 2);                     // Não deve mudar o tamanho
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "Logger (Base): listar_dados()")
    {
        db_real_instance->adicionar(create_jogador_objeto_for_db("Ana", "A", "1"));
        db_real_instance->adicionar(create_jogador_objeto_for_db("Beto", "B", "2,3"));
        SUBCASE("Listar dados com DB valido")
        {
            string output = player_logger_instance->listar_dados(": ", " | ", "---\n");
            // A ordem dos jogadores do db_->listar() não é garantida.
            // A ordem das chaves dentro do loop (Nome, Apelido, etc.) é fixa no seu código.
            // Então, verificamos a presença de substrings dos jogadores.
            CHECK(
              output.find("Nome: Ana | Apelido: A | Partidas Disputadas: 1 | Maior Pontuacao: 1 | Pontuacoes: 1,") !=
              string::npos);
            CHECK(
              output.find("Nome: Beto | Apelido: B | Partidas Disputadas: 2 | Maior Pontuacao: 3 | Pontuacoes: 2,3,") !=
              string::npos);
            // Verifica que ambos estão presentes
            CHECK(output.find("Ana") != string::npos);
            CHECK(output.find("Beto") != string::npos);
        }
        SUBCASE("Listar dados com DB vazio")
        {
            db_real_instance->limpar(); // Garante que o DB está vazio
            string output = player_logger_instance->listar_dados(": ", " | ", "---\n");
            CHECK(output.empty()); // Deve retornar uma string vazia
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: listar_dados_ordenados()")
    {
        // Inserir dados com nomes/apelidos/pontuacoes variados para testar ordenacao
        db_real_instance->adicionar(create_jogador_objeto_for_db("Charlie", "C", "100,50"));  // Max 100
        db_real_instance->adicionar(create_jogador_objeto_for_db("Alice", "A", "200,10,30")); // Max 200
        db_real_instance->adicionar(create_jogador_objeto_for_db("Bob", "B", "150"));         // Max 150
        db_real_instance->adicionar(create_jogador_objeto_for_db("David", "D", ""));          // Max -1 (padrao)
        SUBCASE("Listar dados ordenados por Maior Pontuacao (decrescente)")
        {
            string output = player_logger_instance->listar_dados_ordenados(": ", " | ", "---\n");
            // As pontuações máximas são: Alice(200), Bob(150), Charlie(100), David(-1)
            // A ordem esperada na string de saída deve ser essa.
            size_t pos_alice = output.find("Apelido: A");
            size_t pos_bob = output.find("Apelido: B");
            size_t pos_charlie = output.find("Apelido: C");
            size_t pos_david = output.find("Apelido: D");
            // Verifica a ordem das ocorrências na string
            REQUIRE(pos_alice != string::npos);
            REQUIRE(pos_bob != string::npos);
            REQUIRE(pos_charlie != string::npos);
            REQUIRE(pos_david != string::npos);
            CHECK(pos_alice < pos_bob);     // Alice (200) antes de Bob (150)
            CHECK(pos_bob < pos_charlie);   // Bob (150) antes de Charlie (100)
            CHECK(pos_charlie < pos_david); // Charlie (100) antes de David (-1)
        }
        SUBCASE("Listar dados ordenados com DB vazio")
        {
            db_real_instance->limpar(); // Garante que o DB está vazio
            string output = player_logger_instance->listar_dados_ordenados(": ", " | ", "---\n");
            CHECK(output.empty()); // Deve retornar uma string vazia
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: salvar()")
    {
        SUBCASE("Salvar novo jogador recem-carregado (atraves de salvar(score))")
        {
            player_logger_instance->carregar("NewPlayer"); // Cria um Dado_Jogador("NewPlayer", "NewPlayer") interno
            player_logger_instance->salvar(100);           // Adiciona 100 ao 'atual_' e salva
                                                           // Verificar se foi adicionado ao DB
            check_player_in_db("NewPlayer", "NewPlayer", "100");
        }
        SUBCASE("Salvar atualizacao de jogador existente (atraves de salvar(score))")
        {
            objeto existing_player_obj = create_jogador_objeto_for_db("Original", "Existente", "10,20");
            REQUIRE(db_real_instance->adicionar(existing_player_obj));
            player_logger_instance->carregar("Existente"); // Carrega o jogador
            player_logger_instance->salvar(30);            // Adiciona 30 e salva (pontuações: 10,20,30)
            check_player_in_db("Existente", "Original", "10,20,30");

            player_logger_instance->salvar(5); // Adiciona mais 5 e salva (pontuações: 10,20,30,5)
            check_player_in_db("Existente", "Original", "10,20,30,5");
        }

        SUBCASE("Salvar com atual_ nulo (deve retornar false)")
        {
            player_logger_instance->resetar(); // Deixa atual_ como nullptr explicitamente
            // Sua lógica interna imprime para cerr e retorna false
            player_logger_instance->salvar(1);
            player_logger_instance->salvar();
        }
    }
    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: carregar()")
    {
        SUBCASE("Carregar jogador existente com sucesso")
        {
            // Prepara o DB com um jogador
            objeto existing_player_obj = create_jogador_objeto_for_db("Alice", "Ali", "10,20,30");
            REQUIRE(db_real_instance->adicionar(existing_player_obj));
            // Tenta carregar o jogador pelo apelido
            player_logger_instance->carregar("Ali");
            CHECK(player_logger_instance->getPartidasDisputadas() == 3);
            CHECK(player_logger_instance->getHighScorePlayer() == 30); // Maior pontuação: 30
        }
        SUBCASE("Carregar jogador existente e verificar estado apos recarregar")
        {
            objeto existing_player_obj = create_jogador_objeto_for_db("Bob", "Bobby", "100,150");
            REQUIRE(db_real_instance->adicionar(existing_player_obj));
            cout << "CARREGANDO BOBBY" << endl;
            player_logger_instance->carregar("Bobby"); // Carrega inicialmente
            player_logger_instance->salvar(170);       // Adiciona 170 e salva no DB (pontuações: 100,150,170)
                                                       // Verifica se o DB foi atualizado
            check_player_in_db("Bobby", "Bob", "100,150,170");
            // Recarrega o mesmo jogador (o ponteiro 'atual_' deve ser deletado e recriado corretamente)
            player_logger_instance->carregar("Bobby");
            CHECK(player_logger_instance->getPartidasDisputadas() == 3);
            CHECK(player_logger_instance->getHighScorePlayer() == 170); // Deve ter a nova maior pontuação
        }
        SUBCASE("Carregar jogador inexistente (deve retornar false e criar Dado_Jogador padrão)")
        {
            // A lógica do seu carregar atualiza 'atual_' para um novo Dado_Jogador(apelido, apelido) e retorna false
            player_logger_instance->carregar("NovoApelido");
            // Verifica se 'atual_' foi setado para um Dado_Jogador com nome/apelido 'NovoApelido'
            // Testamos isso indiretamente através dos getters do PlayerLogger
            CHECK(player_logger_instance->getPartidasDisputadas() == 0); // O novo jogador não tem partidas ainda
            CHECK(player_logger_instance->getHighScorePlayer() == -1);
            // Se este jogador for salvo agora, ele será adicionado ao DB com esse nome/apelido
            player_logger_instance->salvar(5);
            check_player_in_db("NovoApelido", "NovoApelido", "5");
        }
        SUBCASE("Carregar jogador com dados malformatados no DB (deve retornar false e logar erro)")
        {
            // Simular um objeto malformado no DB (ex: pontuacoes invalidas)
            objeto malformed_obj = create_jogador_objeto_for_db("Malformed", "Mal", "10,abc,20");
            REQUIRE(db_real_instance->adicionar(malformed_obj)); // Tentar adicionar (sqlite pode aceitar texto)
            // Quando carregar, a desserialização (Dado_Jogador(obj) -> carregar()) vai falhar
            // O catch interno no PlayerLogger::carregar vai imprimir erro e retornar false
            player_logger_instance->carregar("Mal");
            // 'atual_' deve ser nulo após a falha de desserialização
            // (Sua implementação atual define atual_=nullptr em caso de catch)
            CHECK(player_logger_instance->getPartidasDisputadas() ==
                  0); // Se atual_ for nulo, deve retornar 0 ou um erro
            CHECK(player_logger_instance->getHighScorePlayer() ==
                  -1); // Se atual_ for nulo, deve retornar -1 ou um erro
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: getHighScorePlayer()")
    {
        SUBCASE("High Score de jogador recem-carregado")
        {
            player_logger_instance->carregar("JogadorA");
            player_logger_instance->salvar(10);
            player_logger_instance->salvar(50);
            player_logger_instance->salvar(20);
            CHECK(player_logger_instance->getHighScorePlayer() == 50);
        }
        SUBCASE("High Score de jogador com pontuacoes vazias")
        {
            player_logger_instance->carregar("JogadorB");              // Pontuacoes vazias no 'atual_'
            CHECK(player_logger_instance->getHighScorePlayer() == -1); // Deve retornar -1 conforme Dado_Jogador
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: getHighScoreAll()")
    {
        SUBCASE("High Score geral com varios jogadores")
        {
            db_real_instance->adicionar(create_jogador_objeto_for_db("J1", "P1", "10,20,5")); // Max 20
            db_real_instance->adicionar(create_jogador_objeto_for_db("J2", "P2", "30,15"));   // Max 30
            db_real_instance->adicionar(create_jogador_objeto_for_db("J3", "P3", "5,40,10")); // Max 40
            db_real_instance->adicionar(create_jogador_objeto_for_db("J4", "P4", ""));        // Sem pontuacoes
            CHECK(player_logger_instance->getHighScoreAll() == 40);
        }
        SUBCASE("High Score geral com DB vazio")
        {
            CHECK(db_real_instance->listar().empty());              // Garante que o DB está vazio
            CHECK(player_logger_instance->getHighScoreAll() == -1); // Retorno esperado para DB vazio
        }
        SUBCASE("High Score geral com apenas um jogador e pontuacoes vazias")
        {
            db_real_instance->adicionar(create_jogador_objeto_for_db("J5", "P5", ""));
            CHECK(player_logger_instance->getHighScoreAll() == -1);
        }
        SUBCASE("High Score geral com todos os jogadores com pontuacoes negativas")
        {
            db_real_instance->adicionar(create_jogador_objeto_for_db("N1", "N1", "-10,-5")); // Max -5
            db_real_instance->adicionar(create_jogador_objeto_for_db("N2", "N2", "-20,-1")); // Max -1
            CHECK(player_logger_instance->getHighScoreAll() == -1); // -1 é o default para max_element de vazios
        }
    }
    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: isHighScorePlayer e isHighScoreAll")
    {
        // Configura um jogador no 'atual_' e outros no DB para testar high scores
        db_real_instance->adicionar(
          create_jogador_objeto_for_db("Max", "Maxi", "100,50,120")); // Player high no DB: 120
        db_real_instance->adicionar(
          create_jogador_objeto_for_db("Global", "Glob", "200,10,30")); // Global high no DB: 200
        player_logger_instance->carregar("Maxi");                       // Carrega o jogador Maxi (current high: 120)
        SUBCASE("isHighScorePlayer")
        {
            CHECK_FALSE(player_logger_instance->isHighScorePlayer(100)); // Menor que 120
            CHECK_FALSE(player_logger_instance->isHighScorePlayer(120)); // Não é estritamente MAIOR (se usar >)
            CHECK(player_logger_instance->isHighScorePlayer(121));       // É maior
        }
        SUBCASE("isHighScoreAll")
        {
            // Sua implementação: return novo_score >= getHighScoreAll();
            // getHighScoreAll() deve retornar 200 neste cenário
            CHECK_FALSE(player_logger_instance->isHighScoreAll(199)); // Menor que 200
            CHECK(player_logger_instance->isHighScoreAll(200));       // É igual (então é true)
            CHECK(player_logger_instance->isHighScoreAll(201));       // É maior
        }
        SUBCASE("isHighScore com DB vazio")
        {
            db_real_instance->limpar(); // Limpa o DB
            // Carrega um novo jogador para 'atual_' (sem pontuações)
            player_logger_instance->carregar("EmptyPlayer");
            CHECK(player_logger_instance->isHighScorePlayer(1)); // 1 > -1
            CHECK(player_logger_instance->isHighScoreAll(1));    // 1 >= -1
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: getPartidas()")
    {
        SUBCASE("Partidas de jogador recem-carregado")
        {
            player_logger_instance->carregar("PartidasA");               // Cria novo Dado_Jogador
            CHECK(player_logger_instance->getPartidasDisputadas() == 0); // Nenhuma partida ainda

            player_logger_instance->salvar(10); // Adiciona 1 partida
            CHECK(player_logger_instance->getPartidasDisputadas() == 1);

            player_logger_instance->salvar(20); // Adiciona 1 partida
            player_logger_instance->salvar(30); // Adiciona 1 partida
            CHECK(player_logger_instance->getPartidasDisputadas() == 3);
        }

        SUBCASE("Partidas de jogador carregado do DB")
        {
            db_real_instance->adicionar(create_jogador_objeto_for_db("ComPartidas", "CP", "1,2,3,4,5"));
            player_logger_instance->carregar("CP");
            CHECK(player_logger_instance->getPartidasDisputadas() == 5);
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "Logger (Base): deletar()")
    {
        string apelido1 = "DM", apelido2 = "KM"; // Usar strings para apelidos
        objeto p1 = create_jogador_objeto_for_db("DeleteMe", apelido1, "10");
        objeto p2 = create_jogador_objeto_for_db("KeepMe", apelido2, "20");
        REQUIRE(db_real_instance->adicionar(p1));
        REQUIRE(db_real_instance->adicionar(p2));
        REQUIRE(db_real_instance->listar().size() == 2);
        SUBCASE("Deletar jogador existente")
        {
            // 'deletar' na classe base Logger usa o apelido para exclusão
            CHECK(player_logger_instance->deletar(apelido1)); // Note: apelido1 é string&
            CHECK(db_real_instance->listar().size() == 1);
            check_player_in_db(apelido2, "KeepMe", "20"); // Verifica que o outro jogador permanece
        }

        SUBCASE("Deletar jogador inexistente")
        {
            string inexistente_apelido = "NaoExiste";
            CHECK_FALSE(player_logger_instance->deletar(inexistente_apelido)); // Retorna false
            CHECK(db_real_instance->listar().size() == 2);                     // Não deve mudar o tamanho
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "Logger (Base): listar_dados()")
    {
        db_real_instance->adicionar(create_jogador_objeto_for_db("Ana", "A", "1"));
        db_real_instance->adicionar(create_jogador_objeto_for_db("Beto", "B", "2,3"));
        SUBCASE("Listar dados com DB valido")
        {
            string output = player_logger_instance->listar_dados(": ", " | ", "---\n");
            // A ordem dos jogadores do db_->listar() não é garantida.
            // A ordem das chaves dentro do loop (Nome, Apelido, etc.) é fixa no seu código.
            // Então, verificamos a presença de substrings dos jogadores.
            CHECK(
              output.find("Nome: Ana | Apelido: A | Partidas Disputadas: 1 | Maior Pontuacao: 1 | Pontuacoes: 1,") !=
              string::npos);
            CHECK(
              output.find("Nome: Beto | Apelido: B | Partidas Disputadas: 2 | Maior Pontuacao: 3 | Pontuacoes: 2,3,") !=
              string::npos);
            // Verifica que ambos estão presentes
            CHECK(output.find("Ana") != string::npos);
            CHECK(output.find("Beto") != string::npos);
        }

        SUBCASE("Listar dados com DB vazio")
        {
            db_real_instance->limpar(); // Garante que o DB está vazio
            string output = player_logger_instance->listar_dados(": ", " | ", "---\n");
            CHECK(output.empty()); // Deve retornar uma string vazia
        }
    }

    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: listar_dados_ordenados()")
    {
        // Inserir dados com nomes/apelidos/pontuacoes variados para testar ordenacao
        db_real_instance->adicionar(create_jogador_objeto_for_db("Charlie", "C", "100,50"));  // Max 100
        db_real_instance->adicionar(create_jogador_objeto_for_db("Alice", "A", "200,10,30")); // Max 200
        db_real_instance->adicionar(create_jogador_objeto_for_db("Bob", "B", "150"));         // Max 150
        db_real_instance->adicionar(create_jogador_objeto_for_db("David", "D", ""));          // Max -1 (padrao)
        SUBCASE("Listar dados ordenados por Maior Pontuacao (decrescente)")
        {
            string output = player_logger_instance->listar_dados_ordenados(": ", " | ", "---\n");
            // As pontuações máximas são: Alice(200), Bob(150), Charlie(100), David(-1)
            // A ordem esperada na string de saída deve ser essa.
            size_t pos_alice = output.find("Apelido: A");
            size_t pos_bob = output.find("Apelido: B");
            size_t pos_charlie = output.find("Apelido: C");
            size_t pos_david = output.find("Apelido: D");
            // Verifica a ordem das ocorrências na string
            REQUIRE(pos_alice != string::npos);
            REQUIRE(pos_bob != string::npos);
            REQUIRE(pos_charlie != string::npos);
            REQUIRE(pos_david != string::npos);
            CHECK(pos_alice < pos_bob);     // Alice (200) antes de Bob (150)
            CHECK(pos_bob < pos_charlie);   // Bob (150) antes de Charlie (100)
            CHECK(pos_charlie < pos_david); // Charlie (100) antes de David (-1)
        }
        SUBCASE("Listar dados ordenados com DB vazio")
        {
            db_real_instance->limpar(); // Garante que o DB está vazio
            string output = player_logger_instance->listar_dados_ordenados(": ", " | ", "---\n");
            CHECK(output.empty()); // Deve retornar uma string vazia
        }
    }
    TEST_CASE_FIXTURE(LoggerDatabaseFixture, "PlayerLogger: salvar()")
    {
        SUBCASE("Salvar novo jogador recem-carregado (atraves de salvar(score))")
        {
            player_logger_instance->carregar("NewPlayer"); // Cria um Dado_Jogador("NewPlayer", "NewPlayer") interno
            player_logger_instance->salvar(100);           // Adiciona 100 ao 'atual_' e salva
                                                           // Verificar se foi adicionado ao DB
            check_player_in_db("NewPlayer", "NewPlayer", "100");
        }
        SUBCASE("Salvar atualizacao de jogador existente (atraves de salvar(score))")
        {
            objeto existing_player_obj = create_jogador_objeto_for_db("Original", "Existente", "10,20");
            REQUIRE(db_real_instance->adicionar(existing_player_obj));
            player_logger_instance->carregar("Existente"); // Carrega o jogador
            player_logger_instance->salvar(30);            // Adiciona 30 e salva (pontuações: 10,20,30)
            check_player_in_db("Existente", "Original", "10,20,30");
            player_logger_instance->salvar(5); // Adiciona mais 5 e salva (pontuações: 10,20,30,5)
            check_player_in_db("Existente", "Original", "10,20,30,5");
        }

        SUBCASE("Salvar com atual_ nulo (deve retornar false)")
        {
            player_logger_instance->resetar(); // Deixa atual_ como nullptr explicitamente
            // Sua lógica interna imprime para cerr e retorna false
            player_logger_instance->salvar(1);
            player_logger_instance->salvar();
        }
    }

} // End DOCTEST_TEST_SUITE("Logger & PlayerLogger Tests")