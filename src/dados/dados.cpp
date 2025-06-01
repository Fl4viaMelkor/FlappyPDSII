// dados.cpp
//
// Criado por leand em 25/05/2025.
//

// Este arquivo fornece as definições para as classes declaradas em dados.hpp.
#include "../../include/dados/dados.hpp"
#include <iostream>  // Para saída de depuração potencial (ex.: em deserialize/serialize)

using namespace std;

bool objeto::operator==(const objeto& outro) const {
    return dados == outro.dados;
}

bool objeto::operator!=(const objeto& outro) const {
    return !(*this == outro);
}

string Dado_Jogador::nome1() const {return nome;}

void Dado_Jogador::set_nome(const string &nome) {this->nome = nome;}

string Dado_Jogador::apelido1() const {return apelido;}

void Dado_Jogador::set_apelido(const string &apelido) {this->apelido = apelido;}

int Dado_Jogador::vitorias1() const {return vitorias;}

void Dado_Jogador::set_vitorias(int vitorias) {this->vitorias = vitorias;}

int Dado_Jogador::derrotas1() const {return derrotas;}

void Dado_Jogador::set_derrotas(int derrotas) {this->derrotas = derrotas;}

vector<int> Dado_Jogador::pontuacoes1() const {return pontuacoes;}

void Dado_Jogador::set_pontuacoes(const vector<int> &pontuacoes) {this->pontuacoes = pontuacoes;}


// Inicializa um objeto Dado_Jogador com valores padrão.
Dado_Jogador::Dado_Jogador() : nome(""), apelido(""), vitorias(0), derrotas(0) {
    cout << "Construtor padrão de Dado_Jogador chamado." << endl;
}

// Construtor Parametrizado
// Inicializa um objeto Dado_Jogador com valores fornecidos.
Dado_Jogador::Dado_Jogador(string nome, string apelido, int vitorias, int derrotas)
    : nome(move(nome)), apelido(move(apelido)), vitorias(vitorias), derrotas(derrotas) {
    cout << "Construtor parametrizado de Dado_Jogador chamado." << endl;
}

// Construtor a partir de 'objeto'
// Inicializa um objeto Dado_Jogador desserializando dados de um 'objeto'.
Dado_Jogador::Dado_Jogador(objeto obj) {
    cout << "Construtor de Dado_Jogador a partir de 'objeto' chamado, desserializando..." << endl;
    Dado_Jogador::deserialize(move(obj)); // Chama a função deserialize para preencher os membros
}

// Função deserialize
// Preenche os membros de Dado_Jogador a partir de um 'objeto'.
// Esta função precisa de tratamento robusto de erros para chaves ausentes ou tipos de dados inválidos.
void Dado_Jogador::deserialize(objeto obj) {
    cout << "Desserializando Dado_Jogador..." << endl;

        // Recupera 'nome'
        if (obj.dados.count("nome")) {
            nome = obj.dados["nome"];
        } else {
            // Erro
            nome = ""; // Padrão para string vazia se ausente
            // throw runtime_error("Chave 'nome' ausente durante desserialização.");
        }

        // Recupera 'apelido'
        if (obj.dados.count("apelido")) {
            apelido = obj.dados["apelido"];
        } else {
            apelido = ""; // Padrão para string vazia
        }

        // Recupera 'vitorias' - requer conversão de string para int
        if (obj.dados.count("vitorias")) {
            vitorias = stoi(obj.dados["vitorias"]);
        } else {
            vitorias = 0; // Padrão para 0
        }

        // Recupera 'derrotas' - requer conversão de string para int
        if (obj.dados.count("derrotas")) {
            derrotas = stoi(obj.dados["derrotas"]);
        } else {
            derrotas = 0; // Padrão para 0
        }

        // Recupera 'pontuacoes' - requer análise de string (ex.: "10,20,30") em vetor de ints
        pontuacoes.clear(); // Limpa pontuações existentes
        if (obj.dados.count("pontuacoes") && !obj.dados["pontuacoes"].empty()) {
            string scores_str = obj.dados["pontuacoes"];
            string current_score_str;
            for (const char c : scores_str) {
                if (c == ',') {
                    pontuacoes.push_back(stoi(current_score_str));
                    current_score_str = "";
                } else {
                    current_score_str += c;
                }
            }
            if (!current_score_str.empty()) { // Adiciona a última pontuação
                pontuacoes.push_back(stoi(current_score_str));
            }
        }
    }


// Função serialize
// Converte os membros de Dado_Jogador em um objeto'
objeto Dado_Jogador::serialize() {
    cout << "Serializando Dado_Jogador..." << endl;
    objeto obj;
    obj.dados["nome"] = nome;
    obj.dados["apelido"] = apelido;
    obj.dados["vitorias"] = to_string(vitorias);
    obj.dados["derrotas"] = to_string(derrotas);

    // Converte vector<int> pontuacoes em uma única string (ex.: "10,20,30")
    string pontuacoes_str;
    for (int i = 0; i < pontuacoes.size(); ++i) {
        pontuacoes_str += to_string(pontuacoes[i]);
        if (i < pontuacoes.size() - 1) {
            pontuacoes_str += ",";
        }
    }
    obj.dados["pontuacoes"] = pontuacoes_str;

    return obj;
}

// Função operator==
// Compara dois objetos Dado para igualdade.
// Usa dynamic_cast para verificar com segurança se 'outro' também é um Dado_Jogador.
bool Dado_Jogador::operator==(const Dado& outro) const{
    cout << "Comparando objetos Dado_Jogador..." << endl;
    const Dado_Jogador* other_jogador = dynamic_cast<const Dado_Jogador*>(&outro);
    if (!other_jogador) {
        return false; // 'outro' não é um Dado_Jogador
    }

    // Compara todos os membros relevantes para igualdade
    return (nome == other_jogador->nome &&
            apelido == other_jogador->apelido &&
            vitorias == other_jogador->vitorias &&
            derrotas == other_jogador->derrotas &&
            pontuacoes == other_jogador->pontuacoes); // Comparação de vetor
}

// Função operator!=
// Compara dois objetos Dado para desigualdade.
// Simplesmente nega o resultado de operator==.
bool Dado_Jogador::operator!=(const Dado& outro) const{
    return !(*this == outro);
}