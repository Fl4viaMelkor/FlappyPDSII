//
// Created by leand on 07/06/2025.
//
#include "../../include/fisica/fisica.hpp"

bool Detector_Colisao::registrar(Colidivel *e)
{
    elementos.insert(e);
    return true;
}
bool Detector_Colisao::remover(Colidivel *e)
{
    elementos.erase(e);
    return true;
}
void Detector_Colisao::detectar() {}
