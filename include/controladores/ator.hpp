//
// Created by leand on 24/05/2025.
//

#ifndef ATOR_HPP
#define ATOR_HPP

#include "controlador.hpp"
#include "../fisica/hitbox.hpp"
#include "../interface/graficos.hpp"

class Elemento {
    protected:
        Controlador *controller;
        Desenho grafico;
    public:
        Elemento(const Controlador &control);
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
