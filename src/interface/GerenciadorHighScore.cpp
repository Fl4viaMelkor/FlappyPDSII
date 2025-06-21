#include "../../include/interface/GerenciadorHighScore.hpp"
#include <fstream>   
#include <iostream>

GerenciadorHighScores::GerenciadorHighScores(const std::string& arquivo, size_t max)
    : arquivo_scores(arquivo), max_scores(max) {
    carregarScores();
}

void GerenciadorHighScores::carregarScores() {
    scores.clear();
    std::ifstream arquivo(arquivo_scores);
    if (!arquivo.is_open()) {
        std::cerr << "Aviso: Nao foi possivel abrir o arquivo de scores '" << arquivo_scores << "'. Um novo sera criado." << std::endl;
        return;
    }

    PontuacaoEntry entrada;
    // Formato: <score> <nome> por linha
    while (arquivo >> entrada.score && std::getline(arquivo >> std::ws, entrada.nome)) {
        scores.push_back(entrada);
    }
    arquivo.close();
    
    std::sort(scores.begin(), scores.end()); //Garantir que estão ordenados
}

void GerenciadorHighScores::salvarScores() {
    std::ofstream arquivo(arquivo_scores);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel salvar no arquivo de scores '" << arquivo_scores << "'." << std::endl;
        return;
    }

    for (const auto& entrada : scores) {
        arquivo << entrada.score << " " << entrada.nome << std::endl;
    }
    arquivo.close();
}

const std::vector<PontuacaoEntry>& GerenciadorHighScores::getScores() const {
    return scores;
}

bool GerenciadorHighScores::isHighScore(int novo_score) const {
    if (scores.size() < max_scores) {
        return true; // Há espaço na lista
    }
    // Se a lista está cheia, verifica se o novo score é maior que o menor score na lista
    return novo_score > scores.back().score;
}

void GerenciadorHighScores::adicionarScore(const std::string& nome, int novo_score) {
    if (!isHighScore(novo_score)) {
        return;
    }

    scores.push_back({novo_score, nome});
    std::sort(scores.begin(), scores.end());

    // Mantém a lista com o tamanho máximo
    if (scores.size() > max_scores) {
        scores.resize(max_scores);
    }

    salvarScores(); // Salva as alterações no arquivo
}