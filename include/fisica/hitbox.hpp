//
// Created by leand on 25/05/2025.
//

#ifndef HITBOX_HPP
#define HITBOX_HPP

#include "../interface/figuras_basicas.hpp"
#include "../util.hpp"
#include <cmath>

class Colidivel {
  public:
    virtual ~Colidivel() = default;
    virtual void onCollision() = 0;
    virtual bool colisao(coordenadas p) = 0;
    virtual bool noInterior(const coordenadas &p) const = 0;
    virtual bool noPerimetro(const coordenadas &p) const = 0;
};

class RetanguloHitbox : public Colidivel {

  protected:
    coordenadas &ponto_inferior_esquerdo;
    float &base, &altura;

  public:
    RetanguloHitbox(coordenadas &p1, float &b, float &a)
      : ponto_inferior_esquerdo(p1)
      , base(b)
      , altura(a)
    {
    }
    RetanguloHitbox(coordenadas p1, float b, float a)
      : ponto_inferior_esquerdo(p1)
      , base(b)
      , altura(a)
    {
    }
    bool noInterior(const coordenadas &p) const override;
    bool noPerimetro(const coordenadas &p) const override;
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
