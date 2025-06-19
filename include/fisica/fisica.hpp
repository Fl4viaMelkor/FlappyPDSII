//
// Created by leand on 24/05/2025.
//

#ifndef REGRAS_JOGO_HPP
#define REGRAS_JOGO_HPP
#include "../util.hpp"
#include "./hitbox.hpp"
#include <set>

class Detector_Colisao {
  private:
    set<Colidivel *> elementos;

  public:
    Detector_Colisao() = default;
    bool registrar(Colidivel *e); // Registra um elemento novo e retorna verdadeiro se ele foi inserido com
                                  // sucesso e falso caso ele já exista
    bool remover(Colidivel *e);
    void detectar();
};

#endif // REGRAS_JOGO_HPP
