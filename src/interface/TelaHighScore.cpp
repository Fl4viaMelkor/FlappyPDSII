#include "../../include/interface/TelaHighScore.hpp"
#include <allegro5/allegro_primitives.h>
#include <stdexcept>

TelaHighScores::TelaHighScores(GerenciadorHighScores& manager, int largura, int altura) :
    fonte_titulo(nullptr), fonte_scores(nullptr),
    gerenciador_scores(manager),
    proxima_tela_estado(EstadoProximaTela::NENHUM),
    largura_tela(largura), altura_tela(altura) {
        
    fonte_titulo = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 48, 0);
    fonte_scores = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 32, 0);
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
    al_draw_text(fonte_titulo, al_map_rgb(255, 215, 0), centro_x, altura_tela * 0.1f, ALLEGRO_ALIGN_CENTER, "RECORDES");

    const auto& scores = gerenciador_scores.getScores();
    float pos_y_inicial = altura_tela * 0.25f;
    float espacamento = 40.0f;
//texto
    if (scores.empty()) {
        al_draw_text(fonte_scores, al_map_rgb(255,255,255), centro_x, altura_tela * 0.5f, ALLEGRO_ALIGN_CENTER, "Nenhum recorde ainda!");
    } else {
        for (size_t i = 0; i < scores.size(); ++i) {
            std::string texto_score = std::to_string(i + 1) + ". " + scores[i].nome + " - " + std::to_string(scores[i].score);
            al_draw_text(fonte_scores, al_map_rgb(255,255,255), centro_x, pos_y_inicial + i * espacamento, ALLEGRO_ALIGN_CENTER, texto_score.c_str());
        }
    }
    
    al_draw_text(fonte_scores, al_map_rgb(200,200,200), centro_x, altura_tela * 0.9f, ALLEGRO_ALIGN_CENTER, "Pressione ESC para voltar");
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
