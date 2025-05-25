//
// Created by leand on 24/05/2025.
//
#include "posicao.hpp"
#include "graficos.hpp"

#ifndef RENDERIZADOR_HPP
#define RENDERIZADOR_HPP

class Renderizador {
    public:
        virtual void renderizar(Desenho2D d2d) = 0;
        virtual void renderizar(Desenho3D d3d) = 0;
        virtual void renderizar(Sprite s) = 0;
};

class RenderizadorAllegro: public Renderizador {
  public:
    virtual void renderizar(Desenho2D d2d);
    virtual void renderizar(Desenho3D d3d);
    virtual void renderizar(Sprite s);
}

#endif //RENDERIZADOR_HPP
