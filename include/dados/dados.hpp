//
// Created by leand on 25/05/2025.
//

#ifndef DADOS_HPP
#define DADOS_HPP

#include "data_exception.hpp"
#include <algorithm>
#include <any>
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

/**
 * @brief Representa um objeto genérico que pode ser armazenado em uma base de dados.
 *
 * Todos os elementos são pares chave-valor de strings.
 */
struct objeto {
    map<string, string> dados; ///< Mapa contendo os dados do objeto como chave e valor string.

    /**
     * @brief Compara dois objetos quanto à igualdade.
     * @param outro Outro objeto a ser comparado.
     * @return true se todos os pares chave-valor forem iguais.
     */
    bool operator==(const objeto &outro) const;

    /**
     * @brief Compara dois objetos quanto à diferença.
     * @param outro Outro objeto a ser comparado.
     * @return true se houver qualquer diferença nos dados.
     */
    bool operator!=(const objeto &outro) const;
};

/**
 * @brief Classe abstrata que define a interface para dados que podem ser carregados e exportados.
 *
 * Permite leitura e gravação genérica de objetos no sistema.
 */
class Dado {
  public:
    virtual ~Dado() = default;

    /**
     * @brief Carrega os dados a partir de um objeto.
     * @param obj Objeto genérico contendo os dados.
     */
    virtual void carregar(objeto obj) = 0;

    /**
     * @brief Exporta os dados no formato de objeto.
     * @return objeto contendo os dados da instância.
     */
    virtual objeto exportar() = 0;

    /**
     * @brief Compara dois dados quanto à igualdade.
     * @param outro Outro dado a ser comparado.
     * @return true se forem equivalentes.
     */
    virtual bool operator==(const Dado &outro) const = 0;

    /**
     * @brief Compara dois dados quanto à diferença.
     * @param outro Outro dado a ser comparado.
     * @return true se forem diferentes.
     */
    virtual bool operator!=(const Dado &outro) const = 0;
};

/**
 * @brief Representa um dado específico de jogador, com nome, apelido e pontuações.
 */
class Dado_Jogador : public Dado {
  protected:
    string nome_;            ///< Nome do jogador.
    string apelido_;         ///< Apelido do jogador.
    vector<int> pontuacoes_; ///< Histórico de pontuações do jogador.
  public:
    /**
     * @brief Construtor padrão do jogador.
     */
    Dado_Jogador();

    /**
     * @brief Construtor com nome e opcionalmente apelido.
     * @param nome Nome do jogador.
     * @param apelido Apelido do jogador (opcional).
     */
    explicit Dado_Jogador(string nome, string apelido = "");

    /**
     * @brief Construtor que carrega os dados a partir de um objeto.
     * @param obj Objeto contendo os dados.
     */
    explicit Dado_Jogador(objeto obj);

    void carregar(objeto obj) override;
    objeto exportar() override;

    bool operator==(const Dado &outro) const override;
    bool operator!=(const Dado &outro) const override;

    /**
     * @brief Define o apelido do jogador.
     * @param apelido Novo apelido.
     */
    void apelido(const string &apelido);

    /**
     * @brief Retorna o apelido atual do jogador.
     * @return Apelido como string.
     */
    string apelido() const;

    /**
     * @brief Retorna a lista de pontuações do jogador.
     * @return Vetor de inteiros com pontuações.
     */
    vector<int> pontuacoes() const;

    /**
     * @brief Retorna a maior pontuação do jogador.
     * @return Valor inteiro com pontuação.
     */
    int maior_pontuacao() const;
    int partidas_disputadas() const;
    /**
     * @brief Adiciona uma pontuação ao vetor de pontuações.
     */
    void add_pontuacao(int pontuacao);
    /**
     * @brief Define a lista de pontuações do jogador.
     * @param pontuacoes Vetor com novas pontuações.
     */
    void pontuacoes(const vector<int> &pontuacoes);

    /**
     * @brief Retorna o nome do jogador.
     * @return Nome como string.
     */
    string nome() const;

    /**
     * @brief Define o nome do jogador.
     * @param nome Novo nome.
     */
    void nome(const string &nome);

    /**
     * @brief Retorna o mapeamento entre nomes de colunas SQL e nomes internos.
     * @return Mapa com nomes de colunas.
     */
    static unordered_map<string, string> get_sql_columns();

    /**
     * @brief Retorna uma lista com os nomes das colunas do jogador.
     * @return Vetor com os nomes das colunas.
     */
    static vector<string> get_colunas();

    /**
     * @brief Retorna o nome da chave primária do jogador.
     * @return Nome da chave primária.
     */
    static string get_primary_key();
};

#endif // DADOS_HPP
