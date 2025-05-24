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
        regras r;
        Pontuador *p;
        jogador *j;
    public:
          Estado_jogo(regras r, Pontuador p);
          Estado_jogo(float g, float velY, float velX, Pontuador *p);
};

#endif //GAME_STATE_HPP
