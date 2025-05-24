//
// Created by leand on 24/05/2025.
//
#include <vector.h>

#ifndef KEYBIND_HPP
#define KEYBIND_HPP

#include <functional>
class Keybind {
    private:
        vector<ALLEGRO_EVENT> keys;
        vector<ALLEGRO_EVENT> keys_old;
};
#endif //KEYBIND_HPP
