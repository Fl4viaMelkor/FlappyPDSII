//
// Created by leand on 07/06/2025.
//
#include "../../include/fisica/fisica.hpp"

void Detector_Colisao::registrar(Colidivel *e) { elementos.insert(e); }
void Detector_Colisao::remover(Colidivel *e) { elementos.erase(e); }
void Detector_Colisao::detectar() const
{
    vector<coordenadas> pontos_player = player.get_pontos();
    for (const auto colidivel : elementos) {
        vector<coordenadas> pontos = colidivel->get_pontos();
        for (const auto p : pontos)
            if (player.colisao(p)) {
                player.onCollision();
                colidivel->onCollision();
            }
        for (const auto p : pontos_player)
            if (colidivel->colisao(p)) {
                colidivel->onCollision();
                player.onCollision();
            }
        cout << endl;
    }
    cout << endl;
}
