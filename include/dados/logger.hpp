//
// Created by leand on 25/05/2025.
//
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "dados.hpp"
#include "database.hpp"

/**
 * @brief Classe base para gerenciamento de logs com persistência em banco de dados.
 *
 * Controla operações de carregar, salvar, deletar e listar dados associados a uma entidade.
 */
class Logger {
  protected:
    Database *db_ = nullptr; ///< Ponteiro para o banco de dados utilizado.

    /**
     * @brief Construtor protegido que associa o Logger a um banco de dados.
     * @param db Ponteiro para a instância do banco de dados.
     */
    explicit Logger(Database *db);

    /**
     * @brief Construtor padrão protegido.
     */
    explicit Logger() = default;

  public:
    virtual ~Logger();

    /**
     * @brief Carrega os dados de uma entidade com base no identificador.
     * @param id Identificador da entidade.
     * @return true se o carregamento foi bem-sucedido.
     */
    virtual bool carregar(const string &id) = 0;

    /**
     * @brief Salva os dados atuais no banco.
     * @return true se a operação foi bem-sucedida.
     */
    virtual bool salvar() const = 0;

    /**
     * @brief Deleta os dados associados ao identificador informado.
     * @param id Identificador da entidade.
     * @return true se a operação foi bem-sucedida.
     */
    virtual bool deletar(string id) const;

    /**
     * @brief Reseta ou limpa os dados associados ao Logger.
     *
     * Método abstrato, deve ser implementado pelas classes derivadas.
     */
    virtual void resetar() = 0;

    /**
     * @brief Lista os dados atuais formatados como string.
     * @param sep_chave_valor Separador entre chave e valor (padrão ": ").
     * @param sep_dados Separador entre pares chave-valor (padrão "\t").
     * @param sep_entidade Separador entre entidades (padrão "\n").
     * @return String formatada contendo os dados.
     */
    string listar_dados(const string &sep_chave_valor = ": ", const string &sep_dados = "\t",
                        const string &sep_entidade = "\n");
};



/**
 * @brief Logger especializado para dados de jogadores.
 */
class PlayerLogger final : public Logger {
  private:
    Dado_Jogador *atual_ = nullptr;

  public:
    /**
     * @brief Construtor que associa o PlayerLogger a um banco de dados.
     * @param db Ponteiro para o banco de dados.
     */
    explicit PlayerLogger(Database *db);

 /**
     * @brief Verifica se uma nova pontuação é alta o suficiente para entrar no ranking.
     * @param novo_score A pontuação a ser verificada.
     * @return true se for um recorde, false caso contrário.
     */
    bool isHighScore(int novo_score);

    /**
     * @brief Construtor padrão.
     */
    explicit PlayerLogger();

    /**
     * @brief Lista os dados dos jogadores formatados e ordenados.
     * @param sep_chave_valor Separador entre chave e valor (padrão ": ").
     * @param sep_dados Separador entre pares chave-valor (padrão "\n").
     * @param sep_entidade Separador entre entidades (padrão "\n").
     * @return String formatada com dados ordenados.
     */
    string listar_dados_ordenados(const string &sep_chave_valor = ": ", const string &sep_dados = "\n",
                                  const string &sep_entidade = "\n") const;

    /**
     * @brief Reseta os dados do logger de jogador.
     */
    void resetar() override;
    bool salvar(int pontuacao_mais_recente);
    bool salvar() const override;
    ~PlayerLogger() override;
    bool carregar(const string &id) override;
};

/**
 * @brief Logger especializado para configuração de keybindings.
 */
class Keybinder : public Logger {
  public:
    /**
     * @brief Construtor que recebe uma referência para banco de dados.
     * @param db Referência para o banco de dados.
     */
    explicit Keybinder(const Database &db);

    /**
     * @brief Reseta os dados do keybinder.
     */
    void resetar() override;
};

#endif // LOGGER_HPP
