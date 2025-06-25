#include "../../include/interface/TelaCadastro.hpp"
#include <allegro5/allegro_primitives.h>

TelaCadastro::TelaCadastro(int pontuacao, PlayerLogger &logger_ref, int largura, int altura)
  : fonte_titulo(nullptr)
  , fonte_input(nullptr)
  , pontuacao_final(pontuacao)
  , entrada_concluida(false)
  , proxima_tela_estado(EstadoProximaTela::NENHUM)
  , cursor_timer(0.0f)
  , cursor_visivel(true)
  , logger(logger_ref)
  , // Inicializa a referência
  largura_tela(largura)
  , altura_tela(altura)
{

    nome_jogador.reserve(15); // espaço pra colocar o nome do player

    try {

        fonte_titulo = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 48, 0);
        fonte_input = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 36, 0);
        fonte_subtext = al_load_font("assets/fonts/joystix/joystixmonospace.otf", 28, 0);
        if (!fonte_titulo || !fonte_input || !fonte_subtext)
            throw std::runtime_error("Falha ao carregar fontes para TelaCadastro");
    }
    catch (const std::runtime_error &e) {
        std::cerr << "Erro em TelaCadastro: " << e.what() << std::endl;
    }
}
// destrutor
TelaCadastro::~TelaCadastro()
{
    if (fonte_titulo)
        al_destroy_font(fonte_titulo);
    if (fonte_input)
        al_destroy_font(fonte_input);
    if (fonte_subtext)
        al_destroy_font(fonte_subtext);
}
// piscar o cursor
void TelaCadastro::update()
{
    cursor_timer += (1.0f / 60.0f);
    if (cursor_timer >= 0.5f) {
        cursor_visivel = !cursor_visivel;
        cursor_timer = 0;
    }
}

void TelaCadastro::draw()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    float centro_x = largura_tela / 2.0f;
    // texto
    al_draw_text(fonte_titulo, al_map_rgb(255, 215, 0), centro_x, altura_tela * 0.2f, ALLEGRO_ALIGN_CENTER,
                 "NOVO RECORDE!");
    al_draw_text(fonte_input, al_map_rgb(255, 255, 255), centro_x, altura_tela * 0.4f, ALLEGRO_ALIGN_CENTER,
                 "Digite seu nome:");
    al_draw_text(fonte_input, al_map_rgb(255, 255, 255), centro_x, altura_tela * 0.5f, ALLEGRO_ALIGN_CENTER,
                 nome_jogador.c_str());

    if (cursor_visivel && !entrada_concluida) {
        float largura_texto = al_get_text_width(fonte_input, nome_jogador.c_str());
        float pos_cursor_x = centro_x - (largura_texto / 2.0f) + largura_texto;
        float pos_cursor_y = altura_tela * 0.5f;
        al_draw_text(fonte_input, al_map_rgb(255, 255, 255), pos_cursor_x, pos_cursor_y, ALLEGRO_ALIGN_LEFT, "_");
    }

    al_draw_text(fonte_subtext, al_map_rgb(200, 200, 200), centro_x, altura_tela * 0.8f, ALLEGRO_ALIGN_CENTER,
                 "Pressione ENTER para confirmar");
}

void TelaCadastro::step(ALLEGRO_EVENT &evento)
{ // entrada do teclado
    if (entrada_concluida)
        return;

    if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
        if (evento.keyboard.unichar >= 32 && evento.keyboard.unichar <= 126) {
            if (nome_jogador.length() < 15)
                nome_jogador += evento.keyboard.unichar;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
            if (!nome_jogador.empty())
                nome_jogador.pop_back();
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER || evento.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
            if (nome_jogador.empty())
                nome_jogador = "JOGADOR";
            entrada_concluida = true;
            logger.carregar(nome_jogador);

            if (logger.salvar(pontuacao_final))
                std::cout << "Score salvo com sucesso via Logger para o jogador: " << nome_jogador << std::endl;
            else
                std::cerr << "Falha ao salvar o score via Logger." << std::endl;

            // Define o estado para voltar ao menu principal
            proxima_tela_estado = EstadoProximaTela::MENU_PRINCIPAL;
        }
    }
}

EstadoProximaTela TelaCadastro::getProximaTelaEstado() const { return proxima_tela_estado; }

void TelaCadastro::resetProximaTelaEstado() { proxima_tela_estado = EstadoProximaTela::NENHUM; }
