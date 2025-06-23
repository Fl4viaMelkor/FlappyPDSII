//
// Created by leand on 24/05/2025.
//

// Arquivo para structs e variáveis utilizadas por múltiplas classes

#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
using namespace std;

/**
 * @brief Representa uma ação com identificador e nome.
 */
struct acao {
    int id;         ///< Identificador único da ação.
    string name;    ///< Nome descritivo da ação.
};

/**
 * @brief Constante de gravidade aplicada no jogo.
 */
constexpr float GRAVIDADE = 0.5f;

/**
 * @brief Representa uma gravidade com aceleração nos eixos X e Y.
 */
struct gravidade {
    float acc_Y; ///< Aceleração vertical.
    float acc_X; ///< Aceleração horizontal (geralmente zero no Flappy).
};

/**
 * @brief Representa uma posição ou ponto no espaço bidimensional.
 */
struct coordenadas {
    float x; ///< Coordenada horizontal.
    float y; ///< Coordenada vertical.
};

/**
 * @brief Representa o momento linear de um objeto com posição e velocidade.
 */
struct momento_linear {
    coordenadas posicao;   ///< Posição atual.
    coordenadas velocidade;///< Velocidade atual.
};

/**
 * @brief Armazena ações predefinidas usadas no jogo.
 * 
 * Classe estática (sem construtor) que centraliza constantes de ações.
 */
class Acoes {
  public:
    Acoes() = delete; ///< Impede a criação de instâncias.

    /// Ação de pulo do jogador.
    const static acao pular;

    /// Ação inexistente (placeholder).
    const static acao inexistente;

    /// Ação para mover lentamente à esquerda (usada para obstáculos, talvez).
    const static acao mover_esquerda_devagar;
};

/**
 * @brief Converte uma string para letras maiúsculas.
 * @param input Texto de entrada.
 * @return String convertida em caixa alta.
 */
string toUpper(const string &input);

/**
 * @brief Converte uma string para letras minúsculas.
 * @param input Texto de entrada.
 * @return String convertida em caixa baixa.
 */
string toLower(const string &input);

#endif // UTIL_HPP
