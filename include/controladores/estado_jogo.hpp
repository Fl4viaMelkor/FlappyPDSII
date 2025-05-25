//
// Created by leand on 24/05/2025.
//
#include "fisica.hpp"
#include "pontuador.hpp"
#include "jogador.hpp"


#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

class Estado_jogo{
    private:

        jogador *j;
    public:
          Estado_jogo(regras regras_gerais, Pontuador p);
          Estado_jogo(float g, float velY, float velX, Pontuador *p);
};

#endif //GAME_STATE_HPP
