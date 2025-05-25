//
// Created by leand on 25/05/2025.
//

#include <string>
#include <unordered_map>

using namespace std;
#ifndef DATABASE_HPP
#define DATABASE_HPP



class Database {
    public:
        bool adicionar(objeto);
        bool excluir(objeto);
        vector<objeto> buscar(string chave, string valor);

};

#endif //DATABASE_HPP
