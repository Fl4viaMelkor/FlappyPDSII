#include "sprite.hpp"
#include <allegro5/allegro_image.h>
#include <stdexcept>

Sprite::Sprite(const std::string& filename)
    : bitmap(nullptr), width(0), height(0)
{
    bitmap = al_load_bitmap(filename.c_str());
    if (!bitmap)
        throw std::runtime_error("Erro ao carregar imagem: " + filename);

    width = al_get_bitmap_width(bitmap);
    height = al_get_bitmap_height(bitmap);
}

Sprite::~Sprite() {
    if (bitmap)
        al_destroy_bitmap(bitmap);
}

void Sprite::draw(const coordenadas& pos) const {
    al_draw_bitmap(bitmap, pos.x, pos.y, 0);
}

int Sprite::getWidth() const {
    return width;
}

int Sprite::getHeight() const {
    return height;
}

ALLEGRO_BITMAP* Sprite::getBitmap() const {
    return bitmap;
}
