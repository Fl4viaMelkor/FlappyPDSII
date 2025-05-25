//
// Created by leand on 25/05/2025.
//
#include "figuras_basicas.hpp"
#include "renderizador.hpp"

#ifndef GRAFICOS_HPP
#define GRAFICOS_HPP

class Desenho {
  protected:
    Renderizador *renderizador;
  public:
    Desenho(Renderizador *renderizador);
    void virtual desenhar() = 0;
};

class Desenho2D: public Desenho{
  protected:
    Figura figura2d;
  public:
    Desenho2D(Renderizador *renderizador, Figura figura2d);
    void desenhar();
};


class Sprite: public Desenho{
  protected:
    string url;
   public:
     Sprite(Renderizador *renderizador, string url);
     void desenhar();
};
#endif //GRAFICOS_HPP
