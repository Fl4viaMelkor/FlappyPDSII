//
// Created by leand on 25/05/2025.
//

#include "../../include/fisica/hitbox.hpp"

#include <iostream>

bool RetanguloHitboxAbstract::noInterior(const coordenadas &p) const
{
    if (p.x < ponto_inferior_esquerdo.x || p.y > ponto_inferior_esquerdo.y + altura ||
        p.y < ponto_inferior_esquerdo.y || p.x > ponto_inferior_esquerdo.x + base)
        return false;
    return true;
}
bool RetanguloHitboxAbstract::noPerimetro(const coordenadas &p) const
{

    if (p.x == ponto_inferior_esquerdo.x && p.y >= ponto_inferior_esquerdo.y &&
        p.y <= ponto_inferior_esquerdo.y + altura)
        return true;

    if (p.y == ponto_inferior_esquerdo.y && p.x >= ponto_inferior_esquerdo.x && p.x <= ponto_inferior_esquerdo.x + base)
        return true;

    if (p.x == ponto_inferior_esquerdo.x + base && p.y >= ponto_inferior_esquerdo.y &&
        p.y <= ponto_inferior_esquerdo.y + altura)
        return true;

    if (p.y == ponto_inferior_esquerdo.y + altura && p.x >= ponto_inferior_esquerdo.x &&
        p.x <= ponto_inferior_esquerdo.x + base)
        return true;

    return false;
}
vector<coordenadas> RetanguloHitboxAbstract::get_pontos() const
{
    vector<coordenadas> ret;
    ret.push_back({ ponto_inferior_esquerdo.x, ponto_inferior_esquerdo.y });
    ret.push_back({ ponto_inferior_esquerdo.x + base, ponto_inferior_esquerdo.y });
    ret.push_back({ ponto_inferior_esquerdo.x, ponto_inferior_esquerdo.y + altura });
    ret.push_back({ ponto_inferior_esquerdo.x + base, ponto_inferior_esquerdo.y + altura });
    return ret;
}