//
// Created by leand on 25/05/2025.
//
#include "../util.hpp"
#include <vector>

#ifndef FIGURAS_BASICAS_HPP
#define FIGURAS_BASICAS_HPP

struct reta {
    coordenadas ponto_inicial, ponto_final;
};

struct curva {
    coordenadas centro;       // Posição (relativa ao objeto)
    float radius;
};

class Poligono{
    protected:
        vector<reta> perimetro;
    public:
        Poligono(vector<reta> perimetro);
        void set_perimetro(vector<reta> perimetro);
        vector<reta> get_poligono();
};

class Curva{
  protected:
    coordenadas centro;
    float radius;

  public:
    Curva(coordenadas centro, float radius);
    void set_curva(coordenadas centro, float radius);
    pair<coordenadas, float> get_curva();
};

class Figura{
  	protected:
        vector<Poligono> poligonos;
        vector<Curva> circulos;
	public:
          Figura(coordenadas centro_massa);
          ~Figura();
};

#endif //FIGURAS_BASICAS_HPP
