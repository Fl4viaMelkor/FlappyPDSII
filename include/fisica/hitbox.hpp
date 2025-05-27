//
// Created by leand on 25/05/2025.
//
#include "fisica.hpp"
#include "util.hpp"
#include "info_colisao.hpp"
#include "../interface/figuras_basicas.hpp"

#ifndef HITBOX_HPP
#define HITBOX_HPP


class Hitbox{
    public:
        virtual bool colisao(coordenadas p) = 0;
};

//class PoligonoHitbox:public Hitbox{
//  protected:
//    Poligono poligono;
//  public:
//    PoligonoHitbox(Poligono p):poligono(p){}
//    bool noInterior(coordenadas p);
//    bool noPerimetro(coordenadas p);
//};
//
//class CurvaHitbox:public Hitbox{
//  protected:
//    Curva circulo;
//public:
//    CurvaHitbox(Curva c):circulo(c){}
//    bool noInterior(coordenadas p);
//    bool noPerimetro(coordenadas p);
//};
//
//class FiguraHitbox: public Hitbox{
//    protected:
//        Figura figura;
//    public:
//      	FiguraHitbox(Figura f):figura(f){}
//        virtual bool noInterior(coordenadas p) = 0;
//        virtual bool noPerimetro(coordenadas p) = 0;
//};


class Colidivel {
    public:
        virtual Hitbox getHitbox() const = 0;
        virtual CollisionInfo getInfo(Colidivel alvo) = 0;
        virtual void onCollision(CollisionInfo *info) = 0;
        virtual ~Colidivel() = default;
};


#endif //HITBOX_HPP
