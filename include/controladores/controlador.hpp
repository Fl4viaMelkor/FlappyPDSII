//
// Created by leand on 24/05/2025.
//
#include "../fisica/fisica.hpp"
#include "../util.hpp"
#include <set>

using namespace std;

#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP


// Um controlador é um elemento do jogo (pássaro, obstáculo ou mesmo outro jogador) descrito de forma independente de sua implementação visual
class Controlador {
    protected:
        momento_linear momentoLinear; // Momento linear com posição (referente ao centro de massa) e velocidade
        Modulo_Fisico fisica;
		set<acao> disponiveis; // Lista de ações disponiveis para esse personagem

    public:
      Controlador();
      void executar(acao a);
     ~Controlador();
};

#endif //CONTROLADOR_HPP
