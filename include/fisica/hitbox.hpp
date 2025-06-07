//
// Created by leand on 25/05/2025.
//

#ifndef HITBOX_HPP
#define HITBOX_HPP

#include "../interface/figuras_basicas.hpp"
#include "../util.hpp"
#include "info_colisao.hpp"

class Hitbox {
  public:
    virtual ~Hitbox() = default;
    virtual bool colisao(coordenadas p) = 0;
};

class Colidivel {
  public:
    virtual ~Colidivel() = default;
    virtual Hitbox *getHitbox() const = 0;
    virtual CollisionInfo getInfo() = 0;
    virtual void onCollision() = 0;
};

// class PoligonoHitbox:public Hitbox{
//   protected:
//     Poligono poligono;
//   public:
//     PoligonoHitbox(Poligono p):poligono(p){}
//     bool noInterior(coordenadas p);
//     bool noPerimetro(coordenadas p);
// };
//
// class CurvaHitbox:public Hitbox{
//   protected:
//     Curva circulo;
// public:
//     CurvaHitbox(Curva c):circulo(c){}
//     bool noInterior(coordenadas p);
//     bool noPerimetro(coordenadas p);
// };
//
// class FiguraHitbox: public Hitbox{
//     protected:
//         Figura figura;
//     public:
//       	FiguraHitbox(Figura f):figura(f){}
//         virtual bool noInterior(coordenadas p) = 0;
//         virtual bool noPerimetro(coordenadas p) = 0;
// };

#endif // HITBOX_HPP
