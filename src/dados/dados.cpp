// dados.cpp
//
// Criado por leand em 25/05/2025.
//

// Este arquivo fornece as definições para as classes declaradas em dados.hpp.
#include "../../include/dados/dados.hpp"
#include <iostream>  // Para saída de depuração potencial (ex.: em carregar/exportar)
#include <unordered_map>

using namespace std;

bool objeto::operator==(const objeto &outro) const { return dados == outro.dados; }

bool objeto::operator!=(const objeto &outro) const { return !(*this == outro); }


// Inicializa um objeto Dado_Jogador com valores padrão.
Dado_Jogador::Dado_Jogador() : nome_(""), apelido_(""), vitorias_(0), derrotas_(0) {
    // cout << "Construtor padrão de Dado_Jogador chamado." << endl;
}

// Construtor Parametrizado
// Inicializa um objeto Dado_Jogador com valores fornecidos.
Dado_Jogador::Dado_Jogador(string nome, string apelido, int vitorias, int derrotas) : nome_(move(nome)),
    apelido_(move(apelido)), vitorias_(vitorias), derrotas_(derrotas) {
    // cout << "Construtor parametrizado de Dado_Jogador chamado." << endl;
}

// Construtor a partir de 'objeto'
// Inicializa um objeto Dado_Jogador desserializando dados de um 'objeto'.
Dado_Jogador::Dado_Jogador(objeto obj) {
    // cout << "Construtor de Dado_Jogador a partir de 'objeto' chamado, desserializando..." << endl;
    Dado_Jogador::carregar(move(obj)); // Chama a função carregar para preencher os membros
}

// Função carregar
// Preenche os membros de Dado_Jogador a partir de um 'objeto'.
// Esta função precisa de tratamento robusto de erros para chaves ausentes ou tipos de dados inválidos.
void Dado_Jogador::carregar(objeto obj) {
    // cout << "Desserializando Dado_Jogador..." << endl;

    // Recupera 'nome'
    if (obj.dados.count("nome")) { nome_ = obj.dados["nome"]; }
    else {
        // Erro
        nome_ = ""; // Padrão para string vazia se ausente
        // throw runtime_error("Chave 'nome' ausente durante desserialização.");
    }

    // Recupera 'apelido'
    if (obj.dados.count("apelido")) { apelido_ = obj.dados["apelido"]; }
    else
        apelido_ = ""; // Padrão para string vazia


    // Recupera 'vitorias' - requer conversão de string para int
    if (obj.dados.count("vitorias"))
        vitorias_ = stoi(obj.dados["vitorias"]);

    else
        vitorias_ = 0; // Padrão para 0


    // Recupera 'derrotas' - requer conversão de string para int
    if (obj.dados.count("derrotas"))
        derrotas_ = stoi(obj.dados["derrotas"]);
    else
        derrotas_ = 0; // Padrão para 0


    // Recupera 'pontuacoes' - requer análise de string (ex.: "10,20,30") em vetor de ints
    pontuacoes_.clear(); // Limpa pontuações existentes
    if (obj.dados.count("pontuacoes") && !obj.dados["pontuacoes"].empty()) {
        const string scores_str = obj.dados["pontuacoes"];
        string current_score_str;
        for (const char c: scores_str) {
            if (c == ',') {
                pontuacoes_.push_back(stoi(current_score_str));
                current_score_str = "";
            }
            else { current_score_str += c; }
        }
        if (!current_score_str.empty()) {
            // Adiciona a última pontuação
            pontuacoes_.push_back(stoi(current_score_str));
        }
    }
}


// Função exportar
// Converte os membros de Dado_Jogador em um objeto'
objeto Dado_Jogador::exportar() {
    // cout << "Serializando Dado_Jogador..." << endl;
    objeto obj;
    obj.dados["nome"] = nome_;
    obj.dados["apelido"] = apelido_;
    obj.dados["vitorias"] = to_string(vitorias_);
    obj.dados["derrotas"] = to_string(derrotas_);

    // Converte vector<int> pontuacoes em uma única string (ex.: "10,20,30")
    string pontuacoes_str;
    for (int i = 0; i < pontuacoes_.size(); ++i) {
        pontuacoes_str += to_string(pontuacoes_[i]);
        if (i < pontuacoes_.size() - 1)
            pontuacoes_str += ",";
    }
    obj.dados["pontuacoes"] = pontuacoes_str;

    return obj;
}

// Função operator==
// Compara dois objetos Dado para igualdade.
// Usa dynamic_cast para verificar com segurança se 'outro' também é um Dado_Jogador.
bool Dado_Jogador::operator==(const Dado &outro) const {
    // cout << "Comparando objetos Dado_Jogador..." << endl;
    const Dado_Jogador *other_jogador = dynamic_cast<const Dado_Jogador *>(&outro);
    if (!other_jogador)
        return false;
    // 'outro' não é um Dado_Jogador

    // Compara todos os membros relevantes para igualdade
    return (nome_ == other_jogador->nome_ &&
        apelido_ == other_jogador->apelido_ &&
        vitorias_ == other_jogador->vitorias_ &&
        derrotas_ == other_jogador->derrotas_ &&
        pontuacoes_ == other_jogador->pontuacoes_); // Comparação de vetor
}

unordered_map<string, string> Dado_Jogador::get_sql_columns() {
    return {
        {"NOME", "TEXT NOT NULL"},
        {"APELIDO", "TEXT PRIMARY KEY"},
        {"VITORIAS", "INTEGER"},
        {"DERROTAS", "INTEGER"},
        {"PONTUACOES", "TEXT"}
    };
}

vector<string> Dado_Jogador::get_colunas() {
    vector<string> keys;
    for (const auto &pair: get_sql_columns()) {
        keys.push_back(pair.first);
    }
    return keys;
}

string Dado_Jogador::get_primary_key() { return "apelido"; };


// Função operator!=
// Compara dois objetos Dado para desigualdade.
// Simplesmente nega o resultado de operator==.
bool Dado_Jogador::operator!=(const Dado &outro) const { return !(*this == outro); }

string Dado_Jogador::nome() const { return nome_; }
void Dado_Jogador::nome(const string &nome) { this->nome_ = nome; }
string Dado_Jogador::apelido() const { return apelido_; }
void Dado_Jogador::apelido(const string &apelido) { this->apelido_ = apelido; }
int Dado_Jogador::vitorias() const { return vitorias_; }
void Dado_Jogador::vitorias(int vitorias) { this->vitorias_ = vitorias; }
int Dado_Jogador::derrotas() const { return derrotas_; }
void Dado_Jogador::derrotas(int derrotas) { this->derrotas_ = derrotas; }
vector<int> Dado_Jogador::pontuacoes() const { return pontuacoes_; }
void Dado_Jogador::pontuacoes(const vector<int> &pontuacoes) { this->pontuacoes_ = pontuacoes; }
