//
// Created by leand on 25/05/2025.
//
#include "../util.hpp"
#include <vector>

#ifndef FIGURAS_BASICAS_HPP
#define FIGURAS_BASICAS_HPP

class Forma_Geometrica_Fechada{
    public:
      virtual bool noInterior(coordenadas p) = 0; // retorna verdadeiro se o ponto p está no interior (mas não na extremidade) da figura
      virtual bool noPerimetro(coordenadas p) = 0;// retorna verdadeiro se o ponto p está no perimetro (mas não na extremidade) da figura
};

class Poligono: Forma_Geometrica_Fechada{
    protected:
        vector<coordenadas> perimetro;
    public:
       Poligono(vector<coordenadas> p);
       void setPerimetro(vector<coordenadas> p);
       bool noInterior(coordenadas p);
       bool noPerimetro(coordenadas p);
       vector<coordenadas> get_poligono();
};

class Retangulo: public Poligono{
    public:
      Retangulo(coordenadas p1, coordenadas p2, coordenadas p3, coordenadas p4);
      bool noInterior(coordenadas p);
      bool noPerimetro(coordenadas p);
};

class Circulo: Forma_Geometrica_Fechada{
    protected:
        coordenadas centro;
        float radius;

    public:
        Circulo(coordenadas centro, float radius);
        void set_circulo(coordenadas centro, float radius);
        bool noInterior(coordenadas p);
        bool noPerimetro(coordenadas p);
};



class Figura: Forma_Geometrica_Fechada{
  	protected:
        vector<Forma_Geometrica_Fechada> elementos;
        coordenadas ponto_referencia;
	public:
        Figura(coordenadas ponto_referencia);
        void setFigura(coordenadas ponto_referencia);

        bool noInterior(coordenadas p);
        bool noPerimetro(coordenadas p);
};

#endif //FIGURAS_BASICAS_HPP
