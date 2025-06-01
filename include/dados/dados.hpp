//
// Created by leand on 25/05/2025.
//


#ifndef DADOS_HPP
#define DADOS_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <any>

using namespace std;

// Objeto simples a ser armazenado na base de dados. Todos os elementos são strings
struct objeto {
    unordered_map<string, string> dados;
    bool operator==(const objeto &outro) const;
    bool operator!=(const objeto &outro) const;
};

// Implementação concreta do objeto de forma que possa ser lido e usado pelo sistema
class Dado {
    public:
        virtual void set(string chave, const std::any &valor) = 0;
        virtual any get(string chave) = 0;
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
        void set(string chave, const any &valor) override;
        any get(string chave) override;
};


#endif //DADOS_HPP
