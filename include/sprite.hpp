#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "./interface/desenho.hpp"
#include "util.hpp"

#include <allegro5/allegro.h>
#include <iostream>
#include <string>

class Sprite : public Desenho {
  private:
    ALLEGRO_BITMAP *bitmap;
    int width;
    int height;
    coordenadas &posicao;

  public:
    Sprite(const std::string &filename, coordenadas &p);
    ~Sprite();

    void draw() override;

    int getWidth() const;
    int getHeight() const;
    ALLEGRO_BITMAP *getBitmap() const;
};

#endif
