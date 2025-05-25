//
// Created by leand on 25/05/2025.
//
#include "fisica.h"

#ifndef HITBOX_HPP
#define HITBOX_HPP




struct RectHitbox {
    posicao p;       // Posição (relativa ao objeto)
    float width, height;
};

struct CircleHitbox {
    posicao p;       // Posição (relativa ao objeto)
    float radius;
};

struct PointHitbox {
    posicao p;
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
    void addRect(posicao p, float width, float height);
    void addCircle(posicao p, float radius);
    void addPoint(posicao p);

    void getRects(vector<RectHitbox> &r);
    void getCircles(vector<CircleHitbox> &c);
    void getPoints(vector<PointHitbox> &p);
}

class Collidable {
public:
    virtual Hitbox getHitbox() const = 0;
    virtual void onCollision(CollisionInfo info) = 0;
    virtual ~Collidable() = default;
};


#endif //HITBOX_HPP
