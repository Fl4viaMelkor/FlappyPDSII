//
// Created by leand on 25/05/2025.
//

#ifndef DADOS_HPP
#define DADOS_HPP

#include <any>
#include <iterator> // Para std::begin e std::end, embora muitas vezes n├úo seja estritamente necess├írio para cont├¬ineres padr├úo como vector/map/unordered_map
#include <map>
#include <string>
#include <unordered_map>
#include <utility> // Para std::pair, e indiretamente para begin/end em alguns contextos
#include <vector>
using namespace std;

// Objeto simples a ser armazenado na base de dados. Todos os elementos são strings
struct objeto {
    map<string, string> dados;
    bool operator==(const objeto &outro) const;
    bool operator!=(const objeto &outro) const;
};

// Implementação concreta do objeto de forma que possa ser lido e usado pelo sistema
class Dado {
  public:
    virtual ~Dado() = default;
    virtual void carregar(objeto obj) = 0;
    virtual objeto exportar() = 0;
    virtual bool operator==(const Dado &outro) const = 0;
    virtual bool operator!=(const Dado &outro) const = 0;
};

// Dado jogador que representa um jogador padrão
class Dado_Jogador : public Dado {
  protected:
    string nome_, apelido_;
    int vitorias_, derrotas_;
    vector<int> pontuacoes_;

  public:
    Dado_Jogador();
    Dado_Jogador(string nome, string apelido = "", int vitorias = 0, int derrotas = 0);
    Dado_Jogador(objeto obj);
    void carregar(objeto obj) override;
    objeto exportar() override;
    bool operator==(const Dado &outro) const;
    bool operator!=(const Dado &outro) const;
    void apelido(const string &apelido);
    string apelido() const;
    int vitorias() const;
    void vitorias(int vitorias);
    int derrotas() const;
    void derrotas(int derrotas);
    vector<int> pontuacoes() const;
    void pontuacoes(const vector<int> &pontuacoes);
    string nome() const;
    void nome(const string &nome);

    static unordered_map<string, string> get_sql_columns();
    static vector<string> get_colunas();
    static string get_primary_key();
};

#endif // DADOS_HPP
