//
// Created by leand on 24/05/2025.
//
#include "controlador.hpp"
#include "fisica/hitbox.hpp"
#include "interface/graficos.hpp"

#ifndef ATOR_HPP
#define ATOR_HPP

class Elemento {
    protected:
        Controlador controller;
        Desenho grafico;
    public:
        Elemento(Controlador control);
        void atualizar(acao a);
        void renderizar();
        void atualizar_e_renderizar(acao a);
        ~Elemento();
};

class Ator: public Elemento, public Colidivel {
    protected:
      Hitbox *hitbox;
};



#endif //ATOR_HPP
