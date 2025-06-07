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

// Construtor
// Inicializa o Logger com uma referência para um objeto Database.
Logger::Logger(Database &db)
  : db_(&db)
  , atual_(nullptr)
{
    cout << "Logger construído e associado a um Database const." << endl;
}

// Destrutor
// Libera a memória alocada para 'atual_' se houver um Dado carregado.
Logger::~Logger()
{
    cout << "Logger destruído." << endl;
    delete atual_;    // Libera a memória do objeto Dado carregado
    atual_ = nullptr; // Garante que o ponteiro não aponte para memória inválida
}

// Função carregar
// Tenta carregar um objeto Dado do Database usando um ‘ID’.
bool Logger::carregar(const string &id)
{
    if (!db_) {
        cerr << "Logger error: Ponteiro para Database é nulo ao carregar." << endl;
        return false;
    }

    const vector<objeto> results = db_->buscar("id", id);

    if (!results.empty()) {
        // Se já houver um objeto carregado, deleta-o para evitar vazamento de memória
        if (atual_)
            delete atual_;
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
bool Logger::salvar() const
{
    if (!db_) {
        cerr << "Logger error: Ponteiro para Database é nulo ao salvar." << endl;
        return false;
    }
    if (!atual_) {
        cerr << "Logger error: Nenhum dado carregado (atual_ é nulo) para salvar." << endl;
        return false;
    }

    // Serializa o objeto Dado atual para obter a sua representação 'objeto'
    const objeto obj_to_save = atual_->exportar();

    return db_->atualizar_ou_adicionar(obj_to_save);
}

// Função deletar
// Deleta um objeto do Database por ‘ID’.
bool Logger::deletar(string id) const
{
    if (!db_) {
        cerr << "Logger error: Ponteiro para Database é nulo ao deletar." << endl;
        return false;
    }

    // Cria um 'objeto' temporário contendo apenas o ‘ID’ para a operação de exclusão
    objeto obj_to_delete;
    obj_to_delete.dados["id"] = id; // Assume "id" é a chave identificadora para exclusão

    const bool success = db_->excluir(obj_to_delete);

    return success;
}

// Retorna uma ‘string’ formatada com todos os dados do objeto carregado.
// As chaves são listadas em ordem alfabética.
string Logger::listar_dados(const string &sep_chave_valor, const string &sep_dados, const string &sep_entidade)
{
    if (!db_)
        return "Erro: Base de dados nao inicializada.";
    const std::vector<objeto> results = db_->listar(); // ou db_->buscar(chave, valor);

    std::ostringstream oss;
    oss << "Dados encontrados:\n";
    for (const auto &obj : results) {
        for (const auto &pair : obj.dados) // Agora begin/end deve ser encontrado.
            oss << ", " << pair.first << ": " << pair.second;
        oss << "\n";
    }
    return oss.str();
}

// Construtor
PlayerLogger::PlayerLogger(Database &db)
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

// Esta implementação fornece uma ordenação específica para as vitórias de um jogador
string PlayerLogger::listar_dados_ordenados(const string &sep_chave_valor, const string &sep_dados,
                                            const string &sep_entidade)
{
    const vector<objeto> results = db_->listar_ordenado("vitorias", false);
    string dados = "";
    for (objeto obj : results) {
        for (const auto &pair : obj.dados)
            dados += pair.first + sep_chave_valor + pair.second + sep_dados;
        dados += sep_entidade;
    }
    return dados;
}
