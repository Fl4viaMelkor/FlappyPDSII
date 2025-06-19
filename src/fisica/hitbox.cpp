//
// Created by leand on 25/05/2025.
//

#include "../../include/fisica/hitbox.hpp"

bool RetanguloHitbox::noInterior(const coordenadas &p) const
{
    if (p.x < ponto_inferior_esquerdo.x || p.y > ponto_inferior_esquerdo.y + altura ||
        p.y < ponto_inferior_esquerdo.y || p.x > ponto_inferior_esquerdo.x + base)
        return false;
    return true;
}
bool RetanguloHitbox::noPerimetro(const coordenadas &p) const
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