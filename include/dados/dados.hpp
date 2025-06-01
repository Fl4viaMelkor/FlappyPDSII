//
// Created by leand on 25/05/2025.
//



#ifndef DADOS_HPP
#define DADOS_HPP

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Objeto simples a ser armazenado na base de dados. Todos os elementos são strings
struct objeto{
    unordered_map<string, string> dados;
    bool operator==(const objeto& outro) const;
    bool operator!=(const objeto& outro) const;
};

// Implementação concreta do objeto de forma que possa ser lido e usado pelo sistema
class Dado{

    public:
        virtual ~Dado() = default;
        virtual void deserialize(objeto obj) = 0;
        virtual objeto serialize() = 0;
	    virtual bool operator==(const Dado& outro) const = 0;
        virtual bool operator!=(const Dado& outro) const = 0;
};

// Dado jogador que representa um jogador padrão
class Dado_Jogador : public Dado{
    protected:
      string nome, apelido;

  public:
      string nome1() const;

      void set_nome(const string &nome);

      string apelido1() const;

      void set_apelido(const string &apelido);

      int vitorias1() const;

      void set_vitorias(int vitorias);

      int derrotas1() const;

      void set_derrotas(int derrotas);

      vector<int> pontuacoes1() const;

      void set_pontuacoes(const vector<int> &pontuacoes);

  protected:
      int vitorias, derrotas;
      vector<int> pontuacoes;
    public:
      Dado_Jogador();
      Dado_Jogador(string nome, string apelido="", int vitorias=0, int derrotas=0);
      Dado_Jogador(objeto obj);
      void deserialize(objeto obj) override;
      virtual bool operator==(const Dado& outro) const;
      virtual bool operator!=(const Dado& outro) const;
      objeto serialize() override;
};


#endif //DADOS_HPP
