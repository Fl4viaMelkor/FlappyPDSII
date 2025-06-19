#include "sprite.hpp"

#include <allegro5/allegro_image.h>
#include <bits/ostream.tcc>
#include <stdexcept>

using namespace std;

Sprite::Sprite(const std::string &filename, float &x, float &y)
  : bitmap(nullptr)
  , width(0)
  , height(0)
  , x(x)
  , y(y)
{
    bitmap = al_load_bitmap(filename.c_str());
    if (!bitmap)
        throw std::runtime_error("Erro ao carregar imagem: " + filename);
    width = al_get_bitmap_width(bitmap);
    height = al_get_bitmap_height(bitmap);
}

Sprite::~Sprite()
{
    if (bitmap)
        al_destroy_bitmap(bitmap);
}

void Sprite::draw() { al_draw_bitmap(bitmap, x, y, 0); }

int Sprite::getWidth() const { return width; }

int Sprite::getHeight() const { return height; }

ALLEGRO_BITMAP *Sprite::getBitmap() const { return bitmap; }
