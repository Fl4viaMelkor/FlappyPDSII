#ifndef GERENCIADORHIGHSCORES_HPP
#define GERENCIADORHIGHSCORES_HPP

#include <vector>
#include <string>
#include <algorithm> 

// Guardar uma entrada de pontuação
struct PontuacaoEntry {
    int score;
    std::string nome;

    // usar std::sort
    bool operator<(const PontuacaoEntry& outra) const {
        return score > outra.score; // Ordena do maior para o menor score
    }
};

class GerenciadorHighScores {
private:
    std::string arquivo_scores;
    std::vector<PontuacaoEntry> scores;
    size_t max_scores; 

    void carregarScores();
    void salvarScores();

public:
    GerenciadorHighScores(const std::string& arquivo, size_t max = 10);
    
    // Retorna a lista de scores para exibição
    const std::vector<PontuacaoEntry>& getScores() const;
    
    bool isHighScore(int novo_score) const;
    
    void adicionarScore(const std::string& nome, int novo_score);
};

#endif