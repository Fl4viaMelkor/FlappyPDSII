//
// Created by leand on 24/05/2025.
//
#include "fisica.hpp"
#include "acoes.hpp"
#include <set.h>

using namespace std;

#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP
struct posicao{
	int x, y;
};

// Um controlador é um elemento do jogo (pássaro, obstáculo ou mesmo outro jogador) descrito de forma independente de sua implementação visual
class Controlador {
    protected:
        posicao pos; // Posicoes do centro de massa
		set<acao> disponiveis; // Lista de ações disponiveis para esse personagem

    public:
      Controlador();
      Controlador(posicao pos);
      Controlador(set<acao> disponiveis);
      Controlador(set<acao> disponiveis, posicao pos);

      bool temAcao(acao a); // Informa se o personagem é capaz de realizar aquela ação específica
      posicao getPos(); // Retorna a posição
      void setPos(posicao pos); // Define a posição como a passada por parâmetro
      void updatePos(posicao pos); // Adiciona a posição passada por parâmetro à posição atual

      ~Controlador();
};

#endif //CONTROLADOR_HPP
