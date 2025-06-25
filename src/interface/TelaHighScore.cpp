#include "../../include/interface/TelaHighScore.hpp"
#include <allegro5/allegro_primitives.h>
#include <stdexcept>

TelaHighScores::TelaHighScores(PlayerLogger& logger_ref, int largura, int altura) :
    fonte_titulo(nullptr), fonte_scores(nullptr),
    logger(logger_ref),
    proxima_tela_estado(EstadoProximaTela::NENHUM),
    largura_tela(largura), altura_tela(altura) {

    fonte_titulo = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 48, 0);
    fonte_scores = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 16, 0);
    if (!fonte_titulo || !fonte_scores) throw std::runtime_error("Falha ao carregar fontes para TelaHighScore");
}

TelaHighScores::~TelaHighScores() {
    if (fonte_titulo) al_destroy_font(fonte_titulo);
    if (fonte_scores) al_destroy_font(fonte_scores);
}

void TelaHighScores::update() { }
//fundo da tela
void TelaHighScores::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    float centro_x = largura_tela / 2.0f;
    //texto
    al_draw_text(fonte_titulo, al_map_rgb(255, 215, 0), centro_x, altura_tela * 0.05f, ALLEGRO_ALIGN_CENTER, "RECORDES");

    std::string scores_formatados = logger.listar_dados_ordenados(": ", "\n", "\t");

    if (scores_formatados.empty()) {
        al_draw_text(fonte_scores, al_map_rgb(255,255,255), centro_x, altura_tela * 0.5f, ALLEGRO_ALIGN_CENTER, "Nenhum recorde ainda!");
    } else {

        al_draw_multiline_text(
            fonte_scores,
            al_map_rgb(255, 255, 255),
            centro_x, altura_tela * 0.25f,  // Posição inicial do texto
            largura_tela * 0.9, 40.0f,       // Largura máxima da caixa de texto e altura da linha
            ALLEGRO_ALIGN_CENTER,            // Alinhamento do texto
            scores_formatados.c_str()
        );
    }

    al_draw_text(fonte_scores, al_map_rgb(200, 200, 200), centro_x, altura_tela * 0.9f, ALLEGRO_ALIGN_CENTER, "Pressione ESC para voltar");
}
//vai pra proxima tela
void TelaHighScores::step(ALLEGRO_EVENT& evento) {
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            proxima_tela_estado = EstadoProximaTela::MENU_PRINCIPAL;
        }
    }
}

EstadoProximaTela TelaHighScores::getProximaTelaEstado() const {
    return proxima_tela_estado;
}
void TelaHighScores::resetProximaTelaEstado() {
    proxima_tela_estado = EstadoProximaTela::NENHUM;
}
