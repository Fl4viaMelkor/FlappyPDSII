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
    public:
      Logger(Database *db);
      bool cadastrar(string nome);
      bool remover(string nome);
      bool vitoria(string nome);
      bool derrota(string nome);
};



#endif //LOGGER_HPP
