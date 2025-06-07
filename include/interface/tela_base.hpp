#ifndef TELA_BASE_HPP
#define TELA_BASE_HPP

#include <allegro5/allegro5.h>

enum class EstadoProximaTela {
    NENHUM,
    REINICIAR_JOGO,
    FIM_DE_JOGO,
    MENU_PRINCIPAL,
    JOGO_PRINCIPAL,
    SAIR_DO_JOGO
};

// Classe abstrata, todas as telas devem herdar ela
class TelaBase {
  public:
    virtual ~TelaBase() = default; // Destrutor.

    // update (posição, lógica, etc)
    virtual void update() = 0;

    // Draw, pra tudo que for desenhado
    virtual void draw() = 0;

    // updates do ALLEGRO (teclado, mouse, etc)
    virtual void step(ALLEGRO_EVENT &evento) = 0;

    // Função para o gerenciador de telas saber o que fazer depois
    virtual EstadoProximaTela getProximaTelaEstado() const = 0;

    virtual void resetProximaTelaEstado() = 0; // Reseta o estado após ser lido

    virtual void resetEstado() {}
};

#endif
