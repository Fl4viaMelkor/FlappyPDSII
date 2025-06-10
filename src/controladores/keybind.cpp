//
// Created by leand on 24/05/2025.
//
#include "../../include/controladores/keybind.hpp"
#include "../../include/util.hpp"

Keybind::Keybind(map<acao, ALLEGRO_EVENT> k) { keymap = k; }

// Adiciona uma ação e retorna positivo se a ação foi inserida com sucesso
bool Keybind::adicionar_acao(ALLEGRO_EVENT al, acao a) { return true; }

bool Keybind::existe(ALLEGRO_EVENT al) { return true; }

// Retorna o nome e o tipo da ação
acao Keybind::obter(ALLEGRO_EVENT al) { return Acoes::inexistente; }
