//
// Created by leand on 24/05/2025.
//
#ifndef PONTUADOR_HPP
#define PONTUADOR_HPP
#include "estado_jogo.hpp"

class PontuadorStrategy {
public:
	int virtual getPontos(estado anterior, estado atual) = 0;
};


class Pontuador {
private:
	PontuadorStrategy *estrategia;
	estado atual;

public:
	Pontuador(PontuadorStrategy *estrategia);
	virtual int calcular_pontos(estado novo);
};

#endif //PONTUADOR_HPP
