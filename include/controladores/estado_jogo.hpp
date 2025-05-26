//
// Created by leand on 24/05/2025.
//
#include "pontuador.hpp"
#include "ator.hpp"
#include "../util.hpp"

#include <unordered_map>

#ifndef estado_jogo_HPP
#define estado_jogo_HPP

struct estado{
    Ator jogador;
    vector<Ator> atores;
};

estado atualizar_estado(estado e, acao a);

class EstadoJogo{
  private:
    Ator *jogador;
    unordered_map<string, Ator*> atores;
  public:
       EstadoJogo(Ator *jogador);
       void adicionar_ator(string nome, Ator *elemento);
       void remover_ator(string nome);
       void atualizar_e_renderizar(acao a);
       estado get_estado();

};

#endif //estado_jogo_HPP
