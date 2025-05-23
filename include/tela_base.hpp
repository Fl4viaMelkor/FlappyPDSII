#ifndef TELA_BASE_HPP
#define TELA_BASE_HPP

#include <allegro5/allegro5.h>

//Classe abstrata, todas as telas devem herdar ela


class TelaBase {
public:
    virtual ~TelaBase() {} //

    // update (posição, lógica, etc)
    virtual void update() = 0;

    // Draw, pra tudo que for desenhado
    virtual void draw() = 0;

    // updates do ALLEGRP (teclado, mouse, etc)
    virtual void step(ALLEGRO_EVENT &evento) = 0;
};

#endif