//
// Created by leand on 24/05/2025.
//
#include "posicao.hpp"
#include "graficos.hpp"

#ifndef RENDERIZADOR_HPP
#define RENDERIZADOR_HPP

class Renderizador {
    public:
        virtual void renderizar(Sprite s) = 0;
        virtual void renderizar(Grafico g) = 0;
};

class RenderizadorAllegro: public Renderizador {
  public:
    virtual void renderizar(Sprite s);
    virtual void renderizar(Grafico g);
}

#endif //RENDERIZADOR_HPP
