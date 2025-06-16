#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <string>
#include <allegro5/allegro.h>

class Sprite {
private:
    ALLEGRO_BITMAP* bitmap;
    int width;
    int height;

public:
    Sprite(const std::string& filename);
    ~Sprite();

    void draw(float x, float y) const;

    int getWidth() const;
    int getHeight() const;
    ALLEGRO_BITMAP* getBitmap() const;
};

#endif
