//
// Created by leand on 25/05/2025.
//
#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "database.hpp"
#include "dados.hpp"

class Logger {
	protected:
		const Database *db_;
		Dado *atual_;
		explicit Logger(const Database &db);

	public:
		~Logger();
		bool carregar(const string &id);
		bool salvar() const;
		bool deletar(string id) const;
		virtual bool resetar() = 0;

		string listar_dados(string sep_chave_valor = ": ", string sep_dados = "\n");
		virtual string listar_dados_ordenados(string chave, bool crescente = true, string sep_chave_valor = ": ",
		                                      string sep_dados = "\n");
};

class PlayerLogger final : public Logger {
	public:
		explicit PlayerLogger(const Database &db);
		string listar_dados_ordenados(string chave, bool crescente, string sep_chave_valor = ": ",
		                              string sep_dados = "\n") override;
};

class Keybinder : public Logger {
	public:
		explicit Keybinder(const Database &db);
		string listar_dados_ordenados(string chave, bool crescente, string sep_chave_valor = ": ",
		                              string sep_dados = "\n") override;
};

#endif //LOGGER_HPP
