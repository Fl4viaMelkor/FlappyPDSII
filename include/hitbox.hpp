//
// Created by leand on 25/05/2025.
//
#include "fisica.h"

#ifndef HITBOX_HPP
#define HITBOX_HPP


#include "util.h"

struct RectHitbox {
    coordenadas extremidade;       // Posição (relativa ao objeto)
    float width, height;
};

struct CircleHitbox {
    coordenadas centro;       // Posição (relativa ao objeto)
    float radius;
};

struct PointHitbox {
    coordenadas ponto;
};


class Hitbox{
protected:
    vector<RectHitbox> rects;
    vector<CircleHitbox> circles;
    vector<PointHitbox> points;
public:
    Hitbox();
    void setRects(vector<RectHitbox> r);
    void setCircles(vector<CircleHitbox> c);
    void setPoints(vector<PointHitbox> p);

    void addRect(coordenadas p, float width, float height);
    void addCircle(coordenadas p, float radius);
    void addPoint(coordenadas p);

    vector<RectHitbox> getRects(vector<RectHitbox> &r);
    vector<CircleHitbox> getCircles(vector<CircleHitbox> &c);
    vector<PointHitbox> getPoints(vector<PointHitbox> &p);
}

class Colidivel {
public:
    virtual Hitbox getHitbox() const = 0;
    virtual void naColisao(CollisionInfo info) = 0;
    virtual ~Colidivel() = default;
};


#endif //HITBOX_HPP
