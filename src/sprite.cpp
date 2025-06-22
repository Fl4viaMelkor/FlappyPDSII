#include "sprite.hpp"
#include <allegro5/allegro_image.h>
#include <stdexcept>


//Created by Flávia


SpriteAnimado::SpriteAnimado(const std::string& filename, int numFrames, int frameLargura, int frameAltura, float fps)
    : bitmap(nullptr),
      frameLargura(frameLargura),
      frameAltura(frameAltura),
      numFrames(numFrames),
      frameAtual(0),
      tempoPorFrame(1.0f / fps),
      tempoAcumulado(0.0f)
{
    bitmap = al_load_bitmap(filename.c_str());
    if (!bitmap)
        throw std::runtime_error("Erro ao carregar spritsheet: " + filename);
}

SpriteAnimado::~SpriteAnimado() {
    if (bitmap)
        al_destroy_bitmap(bitmap);
}

void SpriteAnimado::update(float deltaTime) {
    tempoAcumulado += deltaTime;

    if (tempoAcumulado >= tempoPorFrame) {
        tempoAcumulado -= tempoPorFrame;
        frameAtual = (frameAtual + 1) % numFrames;
    }
}

void SpriteAnimado::draw(const coordenadas& pos, bool flip_vertical) const {
    int flags = 0;
    if (flip_vertical)
        flags |= ALLEGRO_FLIP_VERTICAL;

    al_draw_bitmap_region(
        bitmap,
        frameAtual * frameLargura, 0,
        frameLargura, frameAltura,
        pos.x, pos.y,
        flags
    );
}


int SpriteAnimado::getWidth() const {
    return frameLargura;
}

int SpriteAnimado::getHeight() const {
    return frameAltura;
}
