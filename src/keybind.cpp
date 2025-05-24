//
// Created by leand on 24/05/2025.
//
#include "keybind.h"
#include "acoes.h"
Keybind::Keybind(map<ALLEGRO_EVENT, acao> k){
    keymap = k;
}

// Adiciona uma ação e retorna positivo se a ação foi inserida com sucesso
bool Keybind::adicionar_acao(ALLEGRO_EVENT al, acao a){
    auto result = keymap.insert({al, a});
    return result.second;
}

bool Keybind::existe(ALLEGRO_EVENT al){
    return keymap.find(al) != keymap.end();
};

// Retorna o nome e o tipo da ação
acao Keybind::obter(ALLEGRO_EVENT al){
    auto it = keymap.find(al);
    if (it != keymap.end())
        return it->second;
    else
        return Acoes.inexistente;
};
