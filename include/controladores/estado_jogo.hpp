//
// Created by leand on 24/05/2025.
//
#include "fisica.hpp"
#include "pontuador.hpp"
#include "jogador.hpp"


#ifndef estado_jogo_HPP
#define estado_jogo_HPP

struct estado{
    Ator jogador;
    vector<Ator> atores;
};

estado atualizar_estado(estado e, acao a);


#endif //estado_jogo_HPP
