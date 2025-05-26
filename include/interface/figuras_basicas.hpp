//
// Created by leand on 25/05/2025.
//
#include "../util.hpp"
#include <vector>

#ifndef FIGURAS_BASICAS_HPP
#define FIGURAS_BASICAS_HPP


struct curva {
    coordenadas centro;       // Posição (relativa ao objeto)
    float radius;
};

class Poligono{
    protected:
        vector<coordenadas> perimetro;
    public:
        Poligono(vector<coordenadas> perimetro);
        void set_perimetro(vector<coordenadas> perimetro);
        vector<coordenadas> get_poligono();
};

class Retangulo: public Poligono{
    public:
      Retangulo(coordenadas p1, coordenadas p2, coordenadas p3, coordenadas p4);
};

class Curva{
  protected:
    coordenadas centro;
    float radius;
    void set_curva(coordenadas centro, float radius);

  public:
    Curva(coordenadas centro, float radius);
};

class Figura{
  	protected:
        vector<Poligono> poligonos;
        vector<Curva> circulos;
        coordenadas centro_massa;
        void setFigura(coordenadas centro_massa);
	public:
          Figura(coordenadas centro_massa);
          ~Figura();
};

#endif //FIGURAS_BASICAS_HPP
