//
// Created by leand on 24/05/2025.
//
#include<string>

using namespace std;
#ifndef ACAO_HPP
#define ACAO_HPP

// Estrutura básica que representa as ações possíveis. Cada ação possui um código e um nome.
struct acao{
    int id;
    string name;
};

// Classe sem construtor que armazena as ações.
class Acoes{
    Acoes() = delete;
    const static acao pular;
    const static acao inexistente;
};

#endif //ACAO_HPP
