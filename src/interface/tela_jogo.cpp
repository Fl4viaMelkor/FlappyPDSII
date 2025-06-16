#include "../../include/interface/tela_jogo.hpp"
#include "../../include/interface/camada_background.hpp"
#include "../../include/config.hpp"

// Caso algo seja adicionado, lembrese de criar o objeto e de implementar seus métodos, bem como destructors

TelaJogo::TelaJogo()
  : keyState()
{
    bg = new CamadaBackground("assets/background/glacial_mountains.png", 50.0f, LARGURA_TELA, ALTURA_TELA);
    // Criando objetos que vão estar no jogo
    player = new Player(100, 100, 4.0f, 32, 32);

    MySquare = new Cano(600.0, 635, -1, 200, al_map_rgb(255, 0, 0), 1.0f); // cano de teste
    end = false;
}

TelaJogo::~TelaJogo()
{
    // Destrutores
    delete player;
    delete MySquare;
    delete bg;
}

void TelaJogo::update()
{
    player->update(keyState);
    bg->update(1.0f / FPS);

    // Teste
    if (player->isDead() == true)
        end = true;
}

void TelaJogo::draw()
{
    bg->draw();
    player->draw();
    MySquare->draw();
}

void TelaJogo::step(ALLEGRO_EVENT &evento)
{
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN || evento.type == ALLEGRO_EVENT_KEY_UP)
        al_get_keyboard_state(&keyState);
}

bool TelaJogo::acabouJogo() const { return end; }

EstadoProximaTela TelaJogo::getProximaTelaEstado() const { return EstadoProximaTela::NENHUM; }

void TelaJogo::resetEstado() {}
void TelaJogo::resetProximaTelaEstado() {}
