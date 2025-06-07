//
// Created by leand on 25/05/2025.
//
#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "dados.hpp"
#include "database.hpp"

class Logger {
  protected:
    Database *db_ = nullptr;
    Dado *atual_ = nullptr;
    explicit Logger(Database *db);
    explicit Logger() = default;

  public:
    virtual ~Logger();

    bool carregar(const string &id);
    virtual bool salvar() const;
    virtual bool deletar(string id) const;
    virtual void resetar() = 0;

    string listar_dados(const string &sep_chave_valor = ": ", const string &sep_dados = "\t",
                        const string &sep_entidade = "\n");
};

class PlayerLogger final : public Logger {
  public:
    explicit PlayerLogger(Database *db);
    explicit PlayerLogger();
    string listar_dados_ordenados(const string &sep_chave_valor = ": ", const string &sep_dados = "\n",
                                  const string &sep_entidade = "\n") const;
    void resetar() override;
};

class Keybinder : public Logger {
  public:
    explicit Keybinder(const Database &db);
    void resetar() override;
};

#endif // LOGGER_HPP
