//
// Created by leand on 25/05/2025.
//

#include <string>
#include <unordered_map>

using namespace std;

#ifndef DADOS_HPP
#define DADOS_HPP

// Objeto simples a ser armazenado na base de dados. Todos os elementos são strings
struct objeto{
    unordered_map<string, string> dados;
};

// Implementação concreta do objeto de forma que possa ser lido e usado pelo sistema
class Dado{
    protected:
      string nome, apelido;
      int vitorias, derrotas;
      vector<int> pontuacoes;
    public:
      void deserialize(objeto obj);
      objeto serialize();
};

#endif //DADOS_HPP
