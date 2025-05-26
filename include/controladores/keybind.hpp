//
// Created by leand on 24/05/2025.
//
#include <map.h>
#include "../dados/logger.hpp"

#ifndef KEYBIND_HPP
#define KEYBIND_HPP

// Classe que mapeia os botões para as ações. Ao estruturar o código desta forma, o processo de incluir (ou modificar) ações é simplifcado.
class Keybind {
    private:
        map<ALLEGRO_EVENT, acao> keymap;
        Keybinder registrador;
    public:
      Keybind();
      Keybind(Keybinder r);
      Keybind(map<ALLEGRO_EVENT, acao> k);
      bool adicionar_acao(ALLEGRO_EVENT al, acao a);
      bool existe(ALLEGRO_EVENT al);
      acao obter(ALLEGRO_EVENT al);
      ~Keybind();
};
#endif //KEYBIND_HPP
