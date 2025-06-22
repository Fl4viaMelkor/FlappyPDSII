#ifndef SPRITE_ANIMADO_HPP
#define SPRITE_ANIMADO_HPP

#include <string>
#include <allegro5/allegro5.h>
#include "util.hpp"

//Created by Flávia

class SpriteAnimado {
private:
    ALLEGRO_BITMAP* bitmap;
    int frameLargura;
    int frameAltura;
    int numFrames;
    int frameAtual;
    float tempoPorFrame;
    float tempoAcumulado;

public:
    SpriteAnimado(const std::string& filename, int numFrames, int frameLargura, int frameAltura, float fps);
    ~SpriteAnimado();

    void update(float deltaTime);
    void draw(const coordenadas& pos) const;
};

#endif
