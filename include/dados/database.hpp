//
// Created by leand on 25/05/2025.
//


#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "dados.hpp"
#include <vector>
using namespace std;

class Database {
	protected:
		string id_;

	public:
		virtual ~Database() = default;
		virtual bool adicionar(objeto o) const = 0;
		virtual bool excluir(objeto o) const = 0;
		virtual vector<objeto> buscar(string chave, string valor) const = 0;
		virtual vector<objeto> listar() const = 0;
		virtual vector<objeto> listar_ordenado(string chave, bool crescente = true) const = 0;
		virtual bool limpar() const = 0;
		virtual bool atualizar(objeto o) const = 0;
		virtual bool atualizar_ou_adicionar(objeto o) const = 0;
};

#endif //DATABASE_HPP
