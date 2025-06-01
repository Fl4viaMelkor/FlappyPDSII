//
// Created by leand on 25/05/2025.
//
#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "database.hpp"
#include "dados.hpp"

class Logger {
	protected:
		Database *db_;
		Dado *atual_;
		explicit Logger(const Database &db);

	public:
		virtual ~Logger() = default;
		virtual bool carregar(string id) = 0;
		virtual bool salvar() = 0;
		virtual bool deletar(string id) = 0;

		virtual string listar_dados(string sep_chave_valor = ": ", string sep_dados = "\n") = 0;
		virtual string listar_dados_ordenados(string chave, bool crescente = true, string sep_chave_valor = ": ",
		                                      string sep_dados = "\n") = 0;
};

class PlayerLogger final : public Logger {
	public:
		explicit PlayerLogger(const Database &db);
		bool carregar(string id) override;
		bool salvar() override;
		bool deletar(string id) override;
		string listar_dados(string sep_chave_valor = ": ", string sep_dados = "\n") override;
		string listar_dados_ordenados(string chave, bool crescente, string sep_chave_valor = ": ",
		                              string sep_dados = "\n") override;
		~PlayerLogger() override;
};

class Keybinder : public Logger {
	public:
		explicit Keybinder(const Database &db);
		~Keybinder() override;
		bool carregar(string id) override;
		bool salvar() override;
		bool deletar(string id) override;
		string listar_dados(string sep_chave_valor = ": ", string sep_dados = "\n") override;
		string listar_dados_ordenados(string chave, bool crescente, string sep_chave_valor = ": ",
		                              string sep_dados = "\n") override;
};

#endif //LOGGER_HPP
