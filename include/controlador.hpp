//
// Created by leand on 24/05/2025.
//
#include "fisica.hpp"
#include "util.hpp"
#include <set.h>

using namespace std;

#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP


// Um controlador é um elemento do jogo (pássaro, obstáculo ou mesmo outro jogador) descrito de forma independente de sua implementação visual
class Controlador {
    protected:
        momento_linear momentoLinear; // Momento linear com posição (referente ao centro de massa) e velocidade
		set<acao> disponiveis; // Lista de ações disponiveis para esse personagem

    public:
      Controlador();
      Controlador(momento_linear mom);
      Controlador(set<acao> disponiveis);
      Controlador(set<acao> disponiveis, momento_linear mom);

      bool temAcao(acao a); // Informa se o personagem é capaz de realizar aquela ação específica
      posicao getMomentoLinear(); // Retorna o momento linear
      void setMomentoLinear(momento_linear mom); // Define o momento linear como a passada por parâmetro
      void updatePos(coordenadas pos); // Adiciona a posição passada por parâmetro à posição atual
      void updateVel(coordenadas vel); // Adiciona a velocidade passada por parâmetro à posição atual

      ~Controlador();
};

#endif //CONTROLADOR_HPP
