//
// Created by leand on 24/05/2025.
//
#include<string>

// Arquivo para structs e variáveis utilizadas por múltiplas classes

using namespace std;
#ifndef UTIL_HPP
#define UTIL_HPP

// Estrutura básica que representa as ações possíveis. Cada ação possui um código e um nome.
struct acao{
    int id;
    string name;
};

struct gravidade{
    float acc_Y, acc_X;
};

struct coordenadas{
	int x, y;
};

struct momento_linear{
  coordenadas posicao, velocidade;
};


// Classe sem construtor que armazena as ações.
class Acoes{
    Acoes() = delete;
    const static acao pular;
    const static acao inexistente;
};

#endif //UTIL_HPP
