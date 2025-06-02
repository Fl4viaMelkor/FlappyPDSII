//
// Created by leand on 24/05/2025.
//
#include "../include/util.hpp"
#include <algorithm>
#include <cctype>
#include <string>

// Instanciamento das ações
const acao Acoes::pular = {1, "Pular"};
const acao Acoes::mover_esquerda_devagar = {2, "Mover Esquerda Devagar"};

const acao Acoes::inexistente = {0, ""};

string toUpper(const string &input) {
	std::string result = input;
	std::transform(result.begin(), result.end(), result.begin(),
	               [](unsigned char c) { return toupper(c); });
	return result;
}

string toLower(const string &input) {
	std::string result = input;
	std::transform(result.begin(), result.end(), result.begin(),
	               [](unsigned char c) { return tolower(c); });
	return result;
}
