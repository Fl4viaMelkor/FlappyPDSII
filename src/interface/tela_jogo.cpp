#include "../../include/interface/tela_jogo.hpp"
#include "../../include/interface/parallax_background.hpp"
#include "../../include/config.hpp"

// Caso algo seja adicionado, lembrese de criar o objeto e de implementar seus métodos, bem como destructors

TelaJogo::TelaJogo()
  : keyState()
{
    parallaxBg = new ParallaxBackground(LARGURA_TELA, ALTURA_TELA);
    // Criando objetos que vão estar no jogo
    player = new Player("assets/player/bluebird-midflap.png",100, 100, 4.0f, 32, 32);

    MySquare = new Cano(300.0, 135, -1, 200, al_map_rgb(35, 161, 49), 0.4f); // cano de teste

    parallaxBg->adicionarCamada("assets/background/sky.png", 0.0f);
    parallaxBg->adicionarCamada("assets/background/clouds_bg.png", 32.0f);
    parallaxBg->adicionarCamada("assets/background/cloud_lonely.png", 40.0f);
    parallaxBg->adicionarCamada("assets/background/glacial_mountains.png", 50.0f);
    end = false;
}

TelaJogo::~TelaJogo()
{
    // Destrutores
    delete player;
    delete MySquare;
    delete parallaxBg;
}

void TelaJogo::update()
{
    player->update(keyState);
    parallaxBg->update(1.0f / FPS);

    // Teste
    if (player->isDead() == true)
        end = true;
}

void TelaJogo::draw()
{
    parallaxBg->draw();
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
