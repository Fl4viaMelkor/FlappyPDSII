#ifndef TELAJOGO_HPP
#define TELAJOGO_HPP

#include "objeto_cano.hpp"
#include "tela_base.hpp"
#include "player.hpp"
#include <allegro5/allegro5.h>
//Caso algum item ou coisa seja adicionado na tela de JOGO deve criar um ponteiro para esse objeto no PRIVATE

class TelaJogo : public TelaBase {
private:
    Player* player;

    Cano* MySquare; //teste
    ALLEGRO_KEYBOARD_STATE keyState;
    bool end;   //Possível controlador para fim da tela

public:
    TelaJogo();
    ~TelaJogo();

    void update() override;
    void draw() override;
    void step(ALLEGRO_EVENT& evento) override;

    bool acabouJogo() const;
};

#endif
