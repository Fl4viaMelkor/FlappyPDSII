//
// Created by leand on 24/05/2025.
//

#ifndef REGRAS_JOGO_HPP
#define REGRAS_JOGO_HPP
#include "ator.hpp"
#include <vector>

// Regras gerais do jogo devem ser implementadas nesse arquivo.
// O jogo possuirá suas regras padrões e cada personagem terá a sua. Desta forma, é possível implementar elementos com funcionamentos diferentes
struct fisica{
    float acc_Y, acc_X; // Gravidade do jogador
    float vel_Y, vel_X;
};

class Modulo_Fisico{
  	protected:
          fisica fis;
    public:
      Modulo_Fisico(fisica f);
      void update(acao a, posicao &pos);
};


#endif //REGRAS_JOGO_HPP
