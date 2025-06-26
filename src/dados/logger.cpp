//
// Created by leand on 25/05/2025.
//

#include "../../include/dados/logger.hpp"
#include "../../include/dados/dados.hpp"

#include <algorithm> // Para std::sort
#include <iostream>  // Para cerr, endl
#include <iostream>  // Para std::cerr
#include <iterator> // Para std::begin e std::end, embora muitas vezes n├ö├Â┬úÔö£Ôòæo seja estritamente necess├ö├Â┬úÔö£┬írio para cont├ö├Â┬úÔö¼┬╝ineres padr├ö├Â┬úÔö£Ôòæo como vector/map/unordered_map
#include <iterator> // Para std::begin e std::end, embora muitas vezes nÔö£├║o seja estritamente necessÔö£├¡rio para contÔö£┬¼ineres padrÔö£├║o como vector/map/unordered_map
#include <sstream>  // Para std::ostringstream
#include <string>   // Para string
#include <utility>  // Para std::pair, e indiretamente para begin/end em alguns contextos
#include <vector>   // Para vector
using namespace std;

Logger::Logger(Database *db) { db_ = db; }
// Destrutor
// Libera a memória alocada para 'atual_' se houver um Dado carregado.
Logger::~Logger() = default;

bool PlayerLogger::isHighScorePlayer(const int novo_score) const { return novo_score > getHighScorePlayer(); }

bool PlayerLogger::isHighScoreAll(const int novo_score) const { return novo_score >= getHighScoreAll(); }

int PlayerLogger::getHighScorePlayer() const
{
    try {
        return atual_->maior_pontuacao();
    }
    catch (DataException &e) {
        cerr << e.what() << endl;
        return -1;
    }
}

int PlayerLogger::getHighScoreAll() const
{
    vector<int> pontuacoes;
    Dado_Jogador aux;

    for (const auto &jogador : db_->listar()) {
        aux.carregar(jogador);
        int high_score;
        try {
            high_score = aux.maior_pontuacao();
        }
        catch (DataException &_) {
            high_score = -1;
        }
        pontuacoes.emplace_back(high_score);
    }
    if (pontuacoes.empty())
        return -1;
    return *max_element(pontuacoes.begin(), pontuacoes.end());
}
int PlayerLogger::getPartidasDisputadas() const { return atual_->partidas_disputadas(); }

// Função carregar
void PlayerLogger::carregar(const string &apelido)
{
    if (!db_)
        throw DataIOErrorException("Logger error: Ponteiro para Database e nulo ao carregar.");

    const vector<objeto> results = db_->buscar("apelido", apelido);
    if (atual_)
        delete atual_;

    if (!results.empty()) {
        // Se já houver um objeto carregado, deleta-o para evitar vazamento de memória

        // Cria uma instância de Dado_Jogador a partir do primeiro resultado encontrado.
        try {
            atual_ = new Dado_Jogador(results[0]);
            return;
        }
        catch (const exception &e) {
            cerr << "Logger error: Falha ao desserializar Dado_Jogador de objeto: " << e.what()
                 << "Criando um novo usuario com esse nome" << endl;
        }
    }
    atual_ = new Dado_Jogador(apelido, apelido);
}

// Função salvar
// Salva o objeto Dado atualmente carregado no Database.
void PlayerLogger::salvar() const
{
    if (!db_)
        throw DataIOErrorException("Logger error: Ponteiro para Database e nulo ao salvar.");

    if (!atual_) {
        cerr << "Logger error: Nenhum dado carregado (atual_ é nulo) para salvar." << endl;
        return;
    }

    // Serializa o objeto Dado atual para obter a sua representação 'objeto'
    const objeto obj_to_save = atual_->exportar();

    db_->atualizar_ou_adicionar(obj_to_save);
}
PlayerLogger::~PlayerLogger()
{
    delete atual_;    // Libera a memória do objeto Dado carregado
    atual_ = nullptr; // Garante que o ponteiro não aponte para memória inválida
}

// Função deletar
// Deleta um objeto do Database por ‘ID’.
bool Logger::deletar(string &apelido) const
{
    if (!db_)
        throw DataIOErrorException("Logger error: Ponteiro para Database e nulo ao deletar.");

    // Cria um 'objeto' temporário contendo apenas o ‘ID’ para a operação de exclusão
    objeto obj_to_delete;
    obj_to_delete.dados["apelido"] = apelido; // Assume "id" é a chave identificadora para exclusão

    const bool success = db_->excluir(obj_to_delete);

    return success;
}

// Retorna uma ‘string’ formatada com todos os dados do objeto carregado.
// As chaves são listadas em ordem alfabética.
string Logger::listar_dados(const string &sep_chave_valor, const string &sep_dados, const string &sep_entidade) const
{
    if (!db_)
        throw DataIOErrorException("Logger error: Ponteiro para Database e nulo ao carregar.");

    vector<Dado_Jogador> jogadores;
    for (const objeto &jogador : db_->listar())
        jogadores.emplace_back(jogador);

    string dados = "";

    for (const Dado_Jogador &j : jogadores) {
        dados += "Nome" + sep_chave_valor + j.nome() + sep_dados;
        dados += "Apelido" + sep_chave_valor + j.apelido() + sep_dados;
        dados += "Partidas Disputadas" + sep_chave_valor + to_string(j.partidas_disputadas()) + sep_dados;
        dados += "Maior Pontuacao" + sep_chave_valor + to_string(j.maior_pontuacao()) + sep_dados;
        dados += "Pontuacoes" + sep_chave_valor;
        for (const int p : j.pontuacoes())
            dados += to_string(p) + ",";
        dados += sep_entidade;
    }
    return dados;
}

// Construtor
PlayerLogger::PlayerLogger(Database *db)
  : Logger(db)
{
    atual_ = new Dado_Jogador();
    cout << "PlayerLogger construído e associado a um Database." << endl;
}

void PlayerLogger::resetar()
{
    if (atual_) {
        delete atual_;    // Deleta o objeto atual
        atual_ = nullptr; // Garante que o ponteiro seja nulo antes de recriar
    }

    // Cria um Dado_Jogador no estado padrão.
    atual_ = new Dado_Jogador();
}
void PlayerLogger::salvar(int pontuacao_mais_recente) const
{
    atual_->add_pontuacao(pontuacao_mais_recente);
    return salvar();
}

// Esta implementação fornece uma ordenação específica para as vitórias de um jogador
string PlayerLogger::listar_dados_ordenados(const string &sep_chave_valor, const string &sep_dados,
                                            const string &sep_entidade) const
{
    if (!db_)
        throw DataIOErrorException("Logger error: Ponteiro para Database e nulo ao carregar.");

    vector<Dado_Jogador> jogadores;
    for (const objeto &jogador : db_->listar())
        jogadores.emplace_back(jogador);

    std::sort(jogadores.begin(), jogadores.end(), [](const Dado_Jogador &a, const Dado_Jogador &b) {
        if (a.pontuacoes().empty())
            return false;
        if (b.pontuacoes().empty())
            return true;
        return a.maior_pontuacao() > b.maior_pontuacao();
    });

    string dados = "";

    for (const Dado_Jogador &j : jogadores) {
        dados += "Nome" + sep_chave_valor + j.nome() + sep_dados;
        dados += "Apelido" + sep_chave_valor + j.apelido() + sep_dados;
        dados += "Partidas Disputadas" + sep_chave_valor + to_string(j.partidas_disputadas()) + sep_dados;
        dados += "Maior Pontuacao" + sep_chave_valor + (j.pontuacoes().empty() ? " " : to_string(j.maior_pontuacao())) +
                 sep_dados;
        dados += "Pontuacoes" + sep_chave_valor;
        for (const int p : j.pontuacoes())
            dados += to_string(p) + ",";
        dados += sep_entidade;
    }
    return dados;
}
