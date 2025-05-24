//
// Created by leand on 24/05/2025.
//
#include "controlador.hpp"
#include "renderizador.hpp"


#ifndef ATOR_HPP
#define ATOR_HPP



class Ator {
    private:
        Controlador *controller;
        Renderizador *renderer;

    public:
        Ator(const Controlador *control, const Renderizador *render);
        void update(acao a);
        ~Ator();
};



#endif //ATOR_HPP
