//
// Created by leand on 07/06/2025.
//
#include "../../include/fisica/fisica.hpp"
momento_linear Modulo_Fisico::pular(const gravidade &g, momento_linear pos) { return (momento_linear){}; }
momento_linear Modulo_Fisico::mover_esquerda(gravidade gravidade, momento_linear pos) { return (momento_linear){}; }
momento_linear Modulo_Fisico::atualizar_fisica(const gravidade &g, const acao &a, const momento_linear pos)
{
    switch (a.id) {
        case 0:
            return pular(g, pos);
        case 1:
            return mover_esquerda(g, pos);
    }
    return atualizar_fisica(g, pos);
}
momento_linear Modulo_Fisico::atualizar_fisica(const gravidade &g, momento_linear pos)
{
    const float current_speed_x = pos.velocidade.x;
    const float current_speed_y = pos.velocidade.y;
    pos.velocidade.x += g.acc_X;
    pos.velocidade.y += g.acc_Y;
    return (momento_linear){ { pos.posicao.x + (pos.velocidade.x + current_speed_x) / 2,
                               pos.posicao.y + (pos.velocidade.y + current_speed_y) / 2 },
                             { pos.velocidade.x, pos.velocidade.y } };
}
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
