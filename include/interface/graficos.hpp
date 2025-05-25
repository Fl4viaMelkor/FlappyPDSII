//
// Created by leand on 25/05/2025.
//
#include "figuras_basicas.hpp"
#include "renderizador.hpp"

#ifndef FIGURAS_HPP
#define FIGURAS_HPP

class Desenho {
  protected:
    Renderizador *renderizador;
  public:
    Desenho(Renderizador *renderizador);
    void desenhar();
}

class Desenho2D: public Desenho{
  protected:
    Figura figura2d;
  public:
    Desenho2D(Renderizador *renderizador);
};


class Sprite: public Desenho{
  protected:
    string url;
   public:

};

class DesenhoAllegro{
	public:
          virtual void desenhar() = 0;
};

class Sprite{
  protected:
    string url;
  public:
    Sprite(string url);
    string getUrl(string url);
    void setUrl(string url);
};


#endif //FIGURAS_HPP
