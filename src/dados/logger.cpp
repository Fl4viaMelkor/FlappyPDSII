//
// Created by leand on 25/05/2025.
//

#include "../../include/dados/logger.hpp"
#include "../../include/dados/dados.hpp"

#include <iostream>    // Para cerr, endl
#include <vector>      // Para vector
#include <string>      // Para string

using namespace std;

// =============================================================================
// Implementação da classe Logger
// =============================================================================

// Construtor
// Inicializa o Logger com uma referência para um objeto Database.
Logger::Logger(const Database &db) : db_(&db), atual_(nullptr) {
    cout << "Logger construído e associado a um Database const." << endl;
}

// Destrutor
// Libera a memória alocada para 'atual_' se houver um Dado carregado.
Logger::~Logger() {
    cout << "Logger destruído." << endl;
    delete atual_; // Libera a memória do objeto Dado carregado
    atual_ = nullptr; // Garante que o ponteiro não aponte para memória inválida
}

// Função carregar
// Tenta carregar um objeto Dado do Database usando um ‘ID’.
bool Logger::carregar(const string &id) {
    if (!db_) {
        cerr << "Logger error: Ponteiro para Database é nulo ao carregar." << endl;
        return false;
    }

    vector<objeto> results = db_->buscar("id", id);

    if (!results.empty()) {
        // Se já houver um objeto carregado, deleta-o para evitar vazamento de memória
        if (atual_) {
            delete atual_;
        }
        // Cria uma instância de Dado_Jogador a partir do primeiro resultado encontrado.
        try {
            atual_ = new Dado_Jogador(results[0]);
            return true;
        }
        catch (const exception &e) {
            cerr << "Logger error: Falha ao desserializar Dado_Jogador de objeto: " << e.what() << endl;
            delete atual_; // Limpa o objeto alocado se a construção falhou
            atual_ = nullptr;
            return false;
        }
    }
    atual_ = nullptr; // Nenhum objeto encontrado, 'atual_' deve ser nulo
    return false;
}

// Função salvar
// Salva o objeto Dado atualmente carregado no Database.
bool Logger::salvar() const {
    if (!db_) {
        cerr << "Logger error: Ponteiro para Database é nulo ao salvar." << endl;
        return false;
    }
    if (!atual_) {
        cerr << "Logger error: Nenhum dado carregado (atual_ é nulo) para salvar." << endl;
        return false;
    }

    // Serializa o objeto Dado atual para obter sua representação 'objeto'
    objeto obj_to_save = atual_->exportar();

    return db_->atualizar_ou_adicionar(obj_to_save);
}

// Função deletar
// Deleta um objeto do Database por ‘ID’.
bool Logger::deletar(string id) const {
    if (!db_) {
        cerr << "Logger error: Ponteiro para Database é nulo ao deletar." << endl;
        return false;
    }

    // Cria um 'objeto' temporário contendo apenas o ‘ID’ para a operação de exclusão
    objeto obj_to_delete;
    obj_to_delete.dados["id"] = id; // Assume "id" é a chave identificadora para exclusão

    bool success = db_->excluir(obj_to_delete);

    return success;
}

// Retorna uma string formatada com todos os dados do objeto carregado.
// As chaves são listadas em ordem alfabética.
string Logger::listar_dados(string sep_chave_valor, string sep_dados) {
    const vector<objeto> results = db_->listar();
    string dados = "";
    for (objeto obj: results) {
        for (const auto &pair: obj)
            dados += pair.first + sep_chave_valor + pair.second + sep_dados;
    }
    return dados;
}


// Construtor
PlayerLogger::PlayerLogger(const Database &db) : Logger(db) {
    atual_ = new Dado_Jogador();
    cout << "PlayerLogger construído e associado a um Database." << endl;
}


void PlayerLogger::resetar() override {
    if (atual_) {
        delete atual_; // Deleta o objeto atual
        atual_ = nullptr; // Garante que o ponteiro seja nulo antes de recriar
    }

    // Cria um Dado_Jogador no estado padrão.
    atual_ = new Dado_Jogador();
}

// Esta implementação fornece uma ordenação específica para as vitorias de um jogador
string PlayerLogger::listar_dados_ordenados(string sep_chave_valor, string sep_dados) {
    const vector<objeto> results = db_->listar_ordenado("vitorias", false);
    string dados = "";
    for (objeto obj: results) {
        for (const auto &pair: obj)
            dados += pair.first + sep_chave_valor + pair.second + sep_dados;
    }
    return dados;
}
