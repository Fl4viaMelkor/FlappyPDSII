//
// Created by leand on 24/05/2025.
//
#include "controlador.hpp"
#include "renderizador.hpp"
#include "fisica/hitbox.hpp"

#ifndef ATOR_HPP
#define ATOR_HPP



class Elemento {
    protected:
        Controlador *controller;

    public:
        Elemento(const Controlador *control, const Renderizador *render);
        void update(acao a);
        ~Elemento();
};

class Ator: public Elemento, public Colidivel {

};



#endif //ATOR_HPP
