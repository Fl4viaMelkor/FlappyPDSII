#include "../../include/interface/tela_fimdejogo.hpp"

// Construtor
tela_fimdejogo::tela_fimdejogo(int pontuacao, int largura_tela_jogo, int altura_tela_jogo) : fonte_titulo(nullptr),
    fonte_texto(nullptr),
    imagem_fundo(nullptr),
    pontuacao_final(pontuacao),
    proxima_tela_estado(EstadoProximaTela::NENHUM),
    largura_tela(largura_tela_jogo),
    altura_tela(altura_tela_jogo) {
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();

    fonte_titulo = al_load_ttf_font("./assets/fonts/joystix/joystixmonospace.otf", 72, 0);

    if (!fonte_titulo) {
        std::cerr << "Erro ao carregar 'joystix monospace.otf' para fonte_titulo" << std::endl;
    }

    fonte_texto = al_load_font("./assets/fonts/joystix/joystixmonospace.otf", 24, 0);
    if (!fonte_texto) {
        std::cerr << "Erro ao carregar 'joystix monospace.otf' para fonte_texto ." << std::endl;
    }


    imagem_fundo = al_load_bitmap("./assets/imagens/deadlow.jpg");
    if (!imagem_fundo) {
        std::cerr << "Erro ao carregar imagem de fundo para tela_fimdejogo." << std::endl;
    }

    // Define as cores que serão usadas na função draw()
    cor_titulo = al_map_rgb(255, 255, 255); // Branco
    cor_texto_pontuacao = al_map_rgb(237, 195, 12); // Amarelo
    cor_texto_opcoes = al_map_rgb(255, 255, 255); // Branco
}

// Destrutor
tela_fimdejogo::~tela_fimdejogo() {
    if (fonte_titulo) {
        al_destroy_font(fonte_titulo);
    }
    if (fonte_texto) {
        al_destroy_font(fonte_texto);
    }
    if (imagem_fundo) {
        al_destroy_bitmap(imagem_fundo);
    }
}

// update: Lógica de atualização da tela
void tela_fimdejogo::update() {
    // Fazer uma animaçãozinha do personagem morrendo e vivendo aqui mais tarde
}

// Desenha os elementos da tela
void tela_fimdejogo::draw() {
    // Desenha o fundo
    al_draw_scaled_bitmap(imagem_fundo,
                          0, 0, al_get_bitmap_width(imagem_fundo), al_get_bitmap_height(imagem_fundo),
                          0, 0, largura_tela, altura_tela,
                          0);

    float centro_x = largura_tela / 2.0f;

    float painel_y = altura_tela * 0.15f;


    //  Desenha os textos diretamente na tela
    float pos_y_titulo = painel_y + 50;
    float pos_y_pontuacao = pos_y_titulo + 100;
    float pos_y_opcao_reiniciar = pos_y_pontuacao + 100;
    float pos_y_opcao_menu = pos_y_opcao_reiniciar + 50;
    float pos_y_opcao_sair = pos_y_opcao_menu + 50;


    if (fonte_titulo) {
        al_draw_text(fonte_titulo, cor_titulo, centro_x, pos_y_titulo, ALLEGRO_ALIGN_CENTER, "FIM DE JOGO");
    }

    if (fonte_texto) {
        std::string texto_pontuacao_str = "Sua pontuação: " + std::to_string(pontuacao_final);
        al_draw_text(fonte_texto, cor_texto_pontuacao, centro_x, pos_y_pontuacao, ALLEGRO_ALIGN_CENTER,
                     texto_pontuacao_str.c_str());


        al_draw_text(fonte_texto, cor_texto_opcoes, centro_x, pos_y_opcao_reiniciar, ALLEGRO_ALIGN_CENTER,
                     "Pressione R para Reiniciar");
        al_draw_text(fonte_texto, cor_texto_opcoes, centro_x, pos_y_opcao_menu, ALLEGRO_ALIGN_CENTER,
                     "Pressione M para Menu Principal");
        al_draw_text(fonte_texto, cor_texto_opcoes, centro_x, pos_y_opcao_sair, ALLEGRO_ALIGN_CENTER,
                     "Pressione ESC para Sair");
    }
}

// step: Processa inputs (eventos do Allegro)
void tela_fimdejogo::step(ALLEGRO_EVENT &evento) {
    // Só processa se nenhuma ação já foi selecionada para evitar múltiplas transições
    if (proxima_tela_estado != EstadoProximaTela::NENHUM) {
        return;
    }

    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento.keyboard.keycode) {
            case ALLEGRO_KEY_R:
                proxima_tela_estado = EstadoProximaTela::REINICIAR_JOGO;
                break;
            case ALLEGRO_KEY_M:
                proxima_tela_estado = EstadoProximaTela::MENU_PRINCIPAL;
                break;
            case ALLEGRO_KEY_ESCAPE: // Tecla ESC para sair
                proxima_tela_estado = EstadoProximaTela::SAIR_DO_JOGO;
                break;
            default:
                break;
        }
    }
}

// Retorna o estado da próxima tela decidido pelo jogador
EstadoProximaTela tela_fimdejogo::getProximaTelaEstado() const {
    return proxima_tela_estado;
}

// Reseta o estado para NENHUM.
void tela_fimdejogo::resetProximaTelaEstado() {
    proxima_tela_estado = EstadoProximaTela::NENHUM;
}
