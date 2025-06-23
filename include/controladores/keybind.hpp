//
// Created by leand on 24/05/2025.
//

#ifndef KEYBIND_HPP
#define KEYBIND_HPP
#include "../dados/logger.hpp"
#include "../util.hpp"
#include <allegro5/allegro5.h>
#include <map>

// Classe que mapeia os botões para as ações. Ao estruturar o código desta forma, o processo de incluir (ou modificar)
// ações é simplifcado.
class Keybind {
  private:
    map<acao, ALLEGRO_EVENT> keymap;
    Keybinder *registrador;

  public:
    Keybind();
    Keybind(Keybinder *r);
    Keybind(map<acao, ALLEGRO_EVENT> k);
    bool adicionar_acao(ALLEGRO_EVENT al, acao a);
    bool existe(ALLEGRO_EVENT al);
    acao obter(ALLEGRO_EVENT al);
    ~Keybind();
};
#endif // KEYBIND_HPP
