//
// Created by leand on 24/05/2025.
//

// Arquivo para structs e variáveis utilizadas por múltiplas classes

#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>

using namespace std;

struct acao {
    int id;
    string name;
};
constexpr float GRAVIDADE = 0.5f;

struct gravidade {
    float acc_Y, acc_X;
};

struct coordenadas {
    float x, y;
};

struct momento_linear {
    coordenadas posicao, velocidade;
};

// Estrutura básica que representa as ações possíveis. Cada ação possui um código e um nome.

// Classe sem construtor que armazena as ações.
class Acoes {
  public:
    Acoes() = delete;
    const static acao pular;
    const static acao inexistente;
    const static acao mover_esquerda_devagar;
};

string toUpper(const string &input);
string toLower(const string &input);

#endif // UTIL_HPP
