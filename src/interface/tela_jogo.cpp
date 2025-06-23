#include "../../include/interface/tela_jogo.hpp"
#include "../../include/dados/config.hpp"
#include "../../include/interface/parallax_background.hpp"

// Caso algo seja adicionado, lembrese de criar o objeto e de implementar seus métodos, bem como destructors

TelaJogo::TelaJogo()
  : RetanguloHitbox({ 0, 0 }, static_cast<float>(LARGURA_JANELA), static_cast<float>(ALTURA_JANELA))
  , keyState()
{
    parallaxBg = new ParallaxBackground(LARGURA_JANELA, ALTURA_JANELA);
    // Criando objetos que vão estar no jogo
    player = new Player("assets/player/galinha_spritesheet.png", 100, 100, 4.0f, 32, 32); //blue_bird_spritesheet.png  galinha_spritesheet.png

    //nova implementação: substituindo MySquare por vector de canos
    for (int i = 0; i < 4; ++i) {
    float x = 400.0f + i * 250.0f;  // espaçamento horizontal entre os canos
    canos.push_back(new Cano(x, 52, 150, ALTURA_NATIVA, al_map_rgb(35, 161, 49), 0.4f, "assets/obj/pipe-green.png"));
}
    //MySquare = new Cano(300.0f, 52, 150, ALTURA_TELA, al_map_rgb(35, 161, 49), 0.4f, "assets/obj/pipe-green.png");// cano de teste



    parallaxBg->adicionarCamada("assets/background/sky.png", 0.0f);
    parallaxBg->adicionarCamada("assets/background/clouds_bg.png", 32.0f);
    parallaxBg->adicionarCamada("assets/background/cloud_lonely.png", 40.0f);
    parallaxBg->adicionarCamada("assets/background/glacial_mountains.png", 50.0f);
    end = false;

    detector = new Detector_Colisao(*player);
    detector->registrar(this);
    // detector->registrar(MySquare);
}

TelaJogo::~TelaJogo()
{
    // Destrutores
    delete player;

    //novo destrutor para canos: vector
    for (auto cano : canos) {
    delete cano;
}
canos.clear();

   // delete MySquare; não precisa

    delete parallaxBg;
}

void TelaJogo::update()
{
    player->update(keyState);
    parallaxBg->update(1.0f / FPS);

    //novo destrutor para canos
    for (size_t i = 0; i < canos.size(); ++i) {
    canos[i]->move(-2.0f);  // movimento para esquerda

    // reset se sair da tela
    float limite_esquerdo = 0.0f;
    float posicao_anterior = (i == 0) ? canos.back()->getX() : canos[i - 1]->getX();
    canos[i]->reset_if_out_of_screen(limite_esquerdo, posicao_anterior, 250.0f, ALTURA_JANELA);
}

    //MySquare->move(-2.0f); // move o cano 2 pixels
    //MySquare->reset_if_out_of_screen(0.0f, 300.0f, 135.0f,  ALTURA_TELA); // reposiciona o objeto cano se ele sair da tela
    
    detector->detectar();
}

void TelaJogo::draw()
{
    parallaxBg->draw();
    player->draw();

    for (auto cano : canos) {
    cano->draw();
}
    //MySquare->draw(); não precisa mais
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
