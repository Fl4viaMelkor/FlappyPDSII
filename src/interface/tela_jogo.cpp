#include "../../include/interface/tela_jogo.hpp"
#include "../../include/dados/config.hpp"
#include "../../include/interface/parallax_background.hpp"
#include "../../include/interface/tela_jogo.hpp"

#include <random>

// Caso algo seja adicionado, lembrese de criar o objeto e de implementar seus métodos, bem como destructors

void TelaJogo::initializeRandomBackgroud() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> distrib(1, 3);
    int background = distrib(gen);

    switch (background) {
        case 1: // Desert Background
            parallaxBg->adicionarCamada("assets/background/desert/desert_sky.png", 0.0f);
            parallaxBg->adicionarCamada("assets/background/desert/desert_cloud.png", 15.0f);
            parallaxBg->adicionarCamada("assets/background/desert/desert_mountain.png", 30.0f);
            parallaxBg->adicionarCamada("assets/background/desert/desert_dunemid.png", 45.0f);
            parallaxBg->adicionarCamada("assets/background/desert/desert_dunefrontt.png", 60.0f);
            break;

        case 2: // Forest Background
            parallaxBg->adicionarCamada("assets/background/forest/forest_sky.png", 0.0f);
            parallaxBg->adicionarCamada("assets/background/forest/forest_mountain.png", 15.0f);
            parallaxBg->adicionarCamada("assets/background/forest/forest_back.png", 30.0f);
            parallaxBg->adicionarCamada("assets/background/forest/forest_mid.png", 45.0f);
            parallaxBg->adicionarCamada("assets/background/forest/forest_long.png", 65.0f);
            break;

        case 3: // Skies Background
            parallaxBg->adicionarCamada("assets/background/skies/Sky_sky.png", 0.0f);
            parallaxBg->adicionarCamada("assets/background/skies/sky_clouds.png", 30.0f);
            parallaxBg->adicionarCamada("assets/background/skies/Sky_cloud_single.png", 45.0f);
            parallaxBg->adicionarCamada("assets/background/skies/Sky_back_mountain.png", 60.0f);
            break;
    default:
        std::cerr << "Opção de Background Inválida: " << background << std::endl;
        break;
}
}


TelaJogo::TelaJogo()
  : RetanguloHitbox({ 0, 0 }, static_cast<float>(LARGURA_JANELA), static_cast<float>(ALTURA_JANELA))
  , keyState()
  , currentGameState(GameState::PAUSED_FOR_START),
  pontos(0)
{
    float	playerStartX;
	float	playerStartY;
	float	espacamentoCanos;
	float	larguraCanos;

    parallaxBg = new ParallaxBackground(LARGURA_JANELA, ALTURA_JANELA);
    initializeRandomBackgroud();
    // Criando objetos que vão estar no

    playerStartX = LARGURA_JANELA / 8.0f;
    playerStartY = ALTURA_JANELA / 2.0f - (32/ 2.0f);
    player = new Player("assets/player/galinha_spritesheet.png", playerStartX, playerStartY, 0.0f, 32, 32); //blue_bird_spritesheet.png  galinha_spritesheet.png

    font = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 20, 0);
    if (!font){
        cerr << "Falha ao carregar a fonte. Verifique o caminho";
    }
    //nova implementação: substituindo MySquare por vector de canos

	float posicaoIncialX = static_cast<float>(LARGURA_JANELA) + 100.0f;

	espacamentoCanos = 250.0f;
	larguraCanos = 52.0f;
    int quantidadeDeCanos = LARGURA_JANELA / espacamentoCanos + 1;

    for (int i = 0; i < quantidadeDeCanos; ++i) {
    float x = posicaoIncialX + i * espacamentoCanos;  // espaçamento horizontal entre os canos
    canos.push_back(new Cano(x, larguraCanos, 150, ALTURA_JANELA, al_map_rgb(35, 161, 49), 0.4f, "assets/obj/pipe-green.png"));
}
    //MySquare = new Cano(300.0f, 52, 150, ALTURA_TELA, al_map_rgb(35, 161, 49), 0.4f, "assets/obj/pipe-green.png");// cano de teste

    end = false;

    detector = new Detector_Colisao(*player);
    detector->registrar(this);
    // detector->registrar(MySquare);

    //inicializa pontuação
   // int pontos = 0;

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
    // O fundo sempre se move
    parallaxBg->update(1.0f / FPS);

    // A lógica principal do jogo só roda se não estiver pausado
    if (currentGameState == GameState::PLAYING){
        player->update(keyState);

        // Lógica para mover os canos e contar pontos
        for (size_t i = 0; i < canos.size(); ++i) {
            canos[i]->move(-2.0f);
            float limite_esquerdo = -canos[i]->getLargura();
            float espacamento_horizontal = 250.0f;
            float posicao_anterior = (i == 0) ? canos.back()->getX() : canos[i - 1]->getX();
            canos[i]->reset_if_out_of_screen(limite_esquerdo, posicao_anterior + espacamento_horizontal, espacamento_horizontal, ALTURA_NATIVA);

            if (!canos[i]->foiContado && player->getY() > canos[i]->getX() + canos[i]->getLargura())
            {
                pontos++;
                canos[i]->foiContado = true;
            }
        }

        // --- LÓGICA DE FIM DE JOGO ---
        // Verifica se o jogador caiu para fora da tela
        if (player->getY() >= ALTURA_JANELA - player->getAltura()) {
            if (!end) {
                std::cout << "!!! JOGADOR CAIU DA TELA !!!" << std::endl;
                player->onCollision();
                this->end = true;
            }
        }
        // else if (detector->detectar()) {
        //     if (!end) {
        //         std::cout << "!!! COLISÃO COM O CANO !!!" << std::endl;
        //         player->onCollision();
        //         this->end = true;
        //     }
        // }
    }
}

void TelaJogo::draw()
{
    parallaxBg->draw();
    player->draw();

    for (auto cano : canos) {
    cano->draw();
    }

    if (font) {
        std::string pontos_str = "Score: " + std::to_string(pontos);
        al_draw_text(font, al_map_rgb(0, 0, 0), LARGURA_JANELA / 2 + 2, 25 + 2,
        ALLEGRO_ALIGN_CENTER, pontos_str.c_str());
        al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_JANELA / 2, 25,
        ALLEGRO_ALIGN_CENTER, pontos_str.c_str());
    }

 if (currentGameState == GameState::PAUSED_FOR_START && font) {
        al_draw_text(font, al_map_rgb(0, 0, 0),
        LARGURA_JANELA / 2 + 2, ALTURA_JANELA / 2 - 50 + 2,
        ALLEGRO_ALIGN_CENTER, "Pressione ESPAÇO para comecar!"); // Uma pequena sombra preta
        al_draw_text(font, al_map_rgb(255, 255, 255),
        LARGURA_JANELA / 2, ALTURA_JANELA / 2 - 50,
        ALLEGRO_ALIGN_CENTER, "Pressione ESPAÇO para comecar!"); // Texto branco por cima
    }
}
    //MySquare->draw(); não precisa mais

void TelaJogo::step(ALLEGRO_EVENT &evento)
{
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        al_get_keyboard_state(&keyState);
        if (currentGameState == GameState::PAUSED_FOR_START) {
            if (al_key_down(&keyState, ALLEGRO_KEY_SPACE)) {
                currentGameState = GameState::PLAYING;
            }
        }
    } else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
        al_get_keyboard_state(&keyState);
    }
}

bool TelaJogo::acabouJogo() const { return end; }

EstadoProximaTela TelaJogo::getProximaTelaEstado() const { return EstadoProximaTela::NENHUM; }

void TelaJogo::resetEstado() {}
void TelaJogo::resetProximaTelaEstado() {}