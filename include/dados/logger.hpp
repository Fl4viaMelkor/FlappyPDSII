//
// Created by leand on 25/05/2025.
//
#include "database.hpp"
#include "dados.hpp"

#ifndef LOGGER_HPP
#define LOGGER_HPP

class Logger{
    protected:
      Database *db;
      Dado *atual;
    public:
      virtual bool carregar(string id) = 0;
      virtual bool salvar() = 0;
      bool deletar(string id);

      virtual bool atualizar_valor(string chave, string valor) = 0;
      virtual bool atualizar_valor(string chave, int valor) = 0;
      virtual bool atualizar_valor(string chave, double valor) = 0;

      virtual bool acrescer_valor(string chave, int valor) = 0;
      virtual bool acrescer_valor(string chave, double valor) = 0;

      virtual bool decrescer_valor(string chave, int valor) = 0;
      virtual bool decrescer_valor(string chave, double valor) = 0;

      virtual string listar_dados(string sep_chave_valor = ": ", string sep_dados = "\n") = 0;
      virtual string listar_dados_ordenados(string chave, bool crescente = true, string sep_chave_valor = ": ", string sep_dados = "\n") = 0;

};

class PlayerLogger : public Logger{};

class Keybinder: public Logger{
    protected:
        Database *db;
};

#endif //LOGGER_HPP
