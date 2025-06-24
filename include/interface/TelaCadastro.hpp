/**
 * @file TelaCadastro.hpp
 * @brief Define a interface da tela de cadastro de nome para novos recordes.
 */
#ifndef TELACADASTRO_HPP
#define TELACADASTRO_HPP

#include "tela_base.hpp"
#include "../dados/logger.hpp"
#include <allegro5/allegro_font.h>
#include <string>
#include <iostream>
#include <stdexcept>

/**
 * @class TelaCadastro
 * @brief Representa a tela que permite ao jogador digitar seu nome após atingir uma alta pontuação.
 * @details Esta tela é instanciada quando o jogo termina e a pontuação do jogador é alta o suficiente
 * para entrar no ranking. Ela captura a entrada do teclado para formar o nome do jogador e, ao
 * confirmar, usa o PlayerLogger para salvar o novo recorde.
 */
class TelaCadastro : public TelaBase {
private:
    ALLEGRO_FONT* fonte_titulo;      ///< Ponteiro para a fonte usada no título principal ("NOVO RECORDE!").
    ALLEGRO_FONT* fonte_input;       ///< Ponteiro para a fonte usada no campo de nome e instruções.
    ALLEGRO_FONT* fonte_subtext;     ///< Ponteiro para a fonte usada no texto de ajuda ("Pressione ENTER...").

    std::string nome_jogador;        ///< Armazena o nome que o jogador está digitando.
    int pontuacao_final;             ///< A pontuação que será salva junto com o nome.
    bool entrada_concluida;          ///< Flag para indicar se o jogador já confirmou o nome.
    EstadoProximaTela proxima_tela_estado; ///< Armazena o estado da próxima tela para a transição.
    
    // Variáveis para o efeito de cursor piscando
    float cursor_timer;              ///< Temporizador para controlar a velocidade do piscar.
    bool cursor_visivel;             ///< Flag que define se o cursor deve ser desenhado no frame atual.

    PlayerLogger& logger;            ///< Referência para o gerenciador de dados, usado para salvar o score.

    int largura_tela;                ///< Largura da tela virtual, usada para centralizar elementos.
    int altura_tela;                 ///< Altura da tela virtual, usada para posicionar elementos.

public:
    /**
     * @brief Construtor da TelaCadastro.
     * @param pontuacao A pontuação final que o jogador alcançou.
     * @param logger_ref Uma referência para o objeto PlayerLogger que gerencia o banco de dados.
     * @param largura A largura da tela virtual.
     * @param altura A altura da tela virtual.
     */
    TelaCadastro(int pontuacao, PlayerLogger& logger_ref, int largura, int altura);

    /**
     * @brief Destrutor da TelaCadastro.
     * @details Libera os recursos de fonte alocados.
     */
    ~TelaCadastro() override;

    /**
     * @brief Atualiza a lógica da tela.
     * @details Neste caso, controla a animação do cursor piscando.
     */
    void update() override;

    /**
     * @brief Desenha todos os elementos visuais da tela.
     * @details Desenha o título, instruções, o nome sendo digitado e o cursor.
     */
    void draw() override;

    /**
     * @brief Processa eventos do Allegro, como a entrada do teclado.
     * @param evento O evento do Allegro a ser processado.
     */
    void step(ALLEGRO_EVENT& evento) override;

    /**
     * @brief Retorna o estado da próxima tela para o gerenciador de telas.
     * @return EstadoProximaTela O estado para o qual o jogo deve transicionar.
     */
    EstadoProximaTela getProximaTelaEstado() const override;

    /**
     * @brief Reseta o estado de transição da tela para NENHUM.
     * @details Chamado pelo gerenciador de telas após a transição ser processada.
     */
    void resetProximaTelaEstado() override;
};
#endif
