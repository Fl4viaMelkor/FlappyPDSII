//
// Created by leand on 24/05/2025.
//
#include "regras_jogo.hpp"
#include "acoes.hpp"
#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP
struct posicao{
  int x;
  int y;
};

// Um controlador é um elemento do jogo (pássaro, obstáculo ou mesmo outro jogador) descrito de forma independente de sua implementação visual
class Controlador {
    private:
        posicao pos; // Posicoes do ator
    public:
          virtual void update(acao a, regras r) = 0;
          virtual posicao getPosicao() = 0;
};

#endif //CONTROLADOR_HPP
