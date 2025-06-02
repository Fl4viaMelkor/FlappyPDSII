//
// Created by leand on 25/05/2025.
//


#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <unordered_map>

#include "sqlite3.h" // Inclua a biblioteca SQLite

#include <vector>
#include "dados.hpp"
using namespace std;


class Database {
	public:
		virtual ~Database() = default;
		virtual bool adicionar(objeto o) = 0;
		virtual bool excluir(const objeto &o) = 0;
		virtual vector<objeto> buscar(const string &chave, const string &valor) = 0;
		virtual vector<objeto> listar() = 0;
		virtual vector<objeto> listar_ordenado(string chave, bool crescente = true) = 0;
		virtual bool limpar() = 0;
		virtual bool atualizar(objeto o) = 0;
		virtual bool atualizar_ou_adicionar(objeto o) = 0;
};

class SQLDatabase : public Database {
	protected :
		sqlite3 *db_ = nullptr;
		string fileName_, table_name_, primary_key_name_, sql_query_str_;
		bool pkAutoIncrement_ = false;
		unordered_map<string, string> colunas_;
		char *zErrMsg_ = nullptr; // To store error messages
		int rc_ = 0; // Return code for SQLite operations
		string get_sql_type_from_schema(const string &col_name) const;
		SQLDatabase(const string &fileName, const string &tableName, const unordered_map<string, string> &columns,
		            const string &primaryKeyName, bool pkAutoIncrement);

	public:
		~SQLDatabase() override;
		bool adicionar(objeto o) final;
		bool excluir(const objeto &o) final;
		vector<objeto> buscar(const string &chave, const string &valor) final;
		vector<objeto> listar() final;
		vector<objeto> listar_ordenado(string chave, bool crescente = true) final;
		bool limpar() final;
		bool atualizar(objeto o) final;
		bool atualizar_ou_adicionar(objeto o) final;
		bool hasDb() const;
};

class JogadorSQLDatabase : public SQLDatabase {
	public:
		explicit JogadorSQLDatabase(const string &fileName = "jogadores.db");
		~JogadorSQLDatabase() override;
};


#endif //DATABASE_HPP
