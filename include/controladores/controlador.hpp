//
// Created by leand on 24/05/2025.
//


#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include "../util.hpp"
#include <set>

using namespace std;

// Um controlador é um elemento do jogo (pássaro, obstáculo ou mesmo outro jogador) descrito de forma independente de sua implementação visual
class Controlador {
    protected:
        momento_linear momentoLinear; // Momento linear com posição (referente ao centro de massa) e velocidade
        gravidade g;
        set<acao> disponiveis; // Lista de ações disponiveis para esse personagem
        Controlador();
    public:
        virtual ~Controlador() = default;

        virtual coordenadas executar(acao a) = 0;
};

#endif //CONTROLADOR_HPP
