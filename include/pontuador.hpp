//
// Created by leand on 24/05/2025.
//
#include "estado_jogo.hpp"
#ifndef PONTUADOR_HPP
#define PONTUADOR_HPP

// Classe responsável por calcular os pontos do jogo. Pode ser usada durante ou ao final.
// Implementada como classe abstrata e por isso permite diferentes formas de calcular
class Pontuador {
    public:
        virtual int calcular_pontos(Estado_jogo e) const = 0;
};

#endif //PONTUADOR_HPP
