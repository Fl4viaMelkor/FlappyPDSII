//
// Created by leand on 24/05/2025.
//

#ifndef ATOR_HPP
#define ATOR_HPP

#include "../fisica/hitbox.hpp"
#include "../interface/desenho.hpp"
#include "controlador.hpp"

class Ator : public Desenho, public Colidivel {
  protected:
    Controlador *controller;
    Hitbox *hitbox;

    Ator(const Controlador &control);
    void atualizar(acao a);
    void renderizar();
    void atualizar_e_renderizar(acao a);
    ~Ator();

  public:
    Hitbox *getHitbox() const override;
    CollisionInfo getInfo() override;
    void onCollision() override;
    void draw() override;

  protected:
};

#endif // ATOR_HPP
