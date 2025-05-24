//
// Created by leand on 24/05/2025.
//
#include "regras_jogo.hpp"
#include "pontuador.hpp"
#include "jogador.hpp"

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

class Estado_jogo{
    private:
        regras regras_gerais; // Regras gerais, oferecem um caso padrão para aspectos como velocidade e aceleração
        Pontuador *p; // Responsável por oferecer e calcular a pontuação
        jogador *j;
    public:
          Estado_jogo(regras regras_gerais, Pontuador p);
          Estado_jogo(float g, float velY, float velX, Pontuador *p);
};

#endif //GAME_STATE_HPP
