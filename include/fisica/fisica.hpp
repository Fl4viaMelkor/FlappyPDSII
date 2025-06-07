//
// Created by leand on 24/05/2025.
//

#ifndef REGRAS_JOGO_HPP
#define REGRAS_JOGO_HPP
#include "../util.hpp"
#include "./hitbox.hpp"
#include "./info_colisao.hpp"
#include <set>
#include <vector>

// Regras gerais do jogo devem ser implementadas nesse arquivo.
// O jogo possuirá as suas regras padrões e cada personagem terá a sua. Desta forma, é possível implementar elementos
// com funcionamentos diferentes

class Modulo_Fisico {
  private:
    static momento_linear pular(const gravidade &g, momento_linear pos);
    static momento_linear mover_esquerda(gravidade gravidade, momento_linear pos);

  public:
    static momento_linear atualizar_fisica(const gravidade &g, const acao &a, momento_linear pos);
    static momento_linear atualizar_fisica(const gravidade &g, momento_linear pos);
};

class Detector_Colisao {
  private:
    set<Colidivel *> elementos;

  public:
    Detector_Colisao() = default;
    bool registrar(Colidivel *e); // Registra um elemento novo e retorna verdadeiro se ele foi inserido com
                                  // sucesso e falso caso ele já exista
    bool remover(Colidivel *e);
    static pair<CollisionInfo, CollisionInfo> detectar();
};

#endif // REGRAS_JOGO_HPP
