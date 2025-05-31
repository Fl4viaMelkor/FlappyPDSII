//
// Created by leand on 25/05/2025.
//

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#ifndef DADOS_HPP
#define DADOS_HPP

// Objeto simples a ser armazenado na base de dados. Todos os elementos são strings
struct objeto{
    unordered_map<string, string> dados;
    bool operator==(const objeto& outro) const {
      	return dados == outro.dados;
    }
    bool operator!=(const objeto& outro) const {
      	return !(*this == outro);
    }
};

// Implementação concreta do objeto de forma que possa ser lido e usado pelo sistema
class Dado{
    public:
        virtual void deserialize(objeto obj) = 0;
        virtual objeto serialize() = 0;
		virtual bool operator==(const Dado& outro) const = 0;
    	virtual bool operator!=(const Dado& outro) const = 0;
};

// Dado jogador que representa um jogador padrão
class Dado_Jogador : public Dado{
    protected:
      string nome, apelido;
      int vitorias, derrotas;
      vector<int> pontuacoes;
    public:
      Dado_Jogador();
      Dado_Jogador(string nome, string apelido="", int vitorias=0, int derrotas=0);
      Dado_Jogador(objeto obj);
      void deserialize(objeto obj);
      virtual bool operator==(const Dado& outro);
      virtual bool operator!=(const Dado& outro);
      objeto serialize();
};


#endif //DADOS_HPP
