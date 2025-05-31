//
// Created by leand on 25/05/2025.
//


using namespace std;
#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "dados.hpp"
#include <vector>

class Database {
    public:
        virtual ~Database() = default;
        bool virtual adicionar(objeto o) = 0;
        bool virtual excluir(objeto o) = 0;
        vector<objeto> virtual buscar(string chave, string valor) = 0;
        vector<objeto> virtual listar() = 0;
        bool virtual limpar() = 0;
        bool virtual atualizar(objeto o) = 0;
        bool virtual atualizar_ou_adicionar(objeto o) = 0;
};

#endif //DATABASE_HPP
