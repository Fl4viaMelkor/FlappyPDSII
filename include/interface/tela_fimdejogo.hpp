/**
 * @file tela_fimdejogo.hpp
 * @brief Define a interface da tela de "Fim de Jogo", que aparece quando o jogador perde sem bater um recorde.
 */
#ifndef TELAFIMDEJOGO_HPP
#define TELAFIMDEJOGO_HPP

#include "tela_base.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <string>

/**
 * @class tela_fimdejogo
 * @brief Representa a tela padrão de "Fim de Jogo".
 * @details Esta tela é exibida quando a partida termina e a pontuação do jogador não foi
 * alta o suficiente para entrar no ranking de recordes. Ela mostra a pontuação final e
 * oferece opções para reiniciar, voltar ao menu ou sair do jogo.
 */
class tela_fimdejogo : public TelaBase {
  private:
    ALLEGRO_FONT *fonte_titulo;       ///< Ponteiro para a fonte usada no título principal "FIM DE JOGO".
    ALLEGRO_FONT *fonte_texto;        ///< Ponteiro para a fonte usada para a pontuação e opções.
    ALLEGRO_BITMAP *imagem_fundo;     ///< Ponteiro para a imagem de fundo da tela.
    int pontuacao_final;              ///< A pontuação final do jogador a ser exibida.
    EstadoProximaTela proxima_tela_estado; ///< Armazena a escolha do jogador para a transição de tela.

    // Cores
    ALLEGRO_COLOR cor_titulo;         ///< Cor do texto do título.
    ALLEGRO_COLOR cor_texto_pontuacao;///< Cor do texto da pontuação.
    ALLEGRO_COLOR cor_texto_opcoes;   ///< Cor do texto das opções.

    // Dimensões
    int largura_tela;                 ///< Largura da tela virtual.
    int altura_tela;                  ///< Altura da tela virtual.

  public:
    /**
     * @brief Construtor da tela de fim de jogo.
     * @param pontuacao A pontuação final que o jogador alcançou.
     * @param largura_tela_jogo A largura da tela virtual onde a tela será desenhada.
     * @param altura_tela_jogo A altura da tela virtual onde a tela será desenhada.
     */
    tela_fimdejogo(int pontuacao, int largura_tela_jogo, int altura_tela_jogo);

    /**
     * @brief Destrutor da tela de fim de jogo. Libera os recursos alocados (fontes, imagem).
     */
    ~tela_fimdejogo() override;

    /**
     * @brief Atualiza a lógica da tela a cada frame. (Atualmente vazio).
     */
    void update() override;

    /**
     * @brief Desenha todos os elementos visuais da tela.
     */
    void draw() override;

    /**
     * @brief Processa os eventos de input do jogador.
     * @param evento O evento do Allegro a ser processado (ex: pressionamento de tecla).
     */
    void step(ALLEGRO_EVENT &evento) override;

    /**
     * @brief Retorna o estado da próxima tela escolhido pelo jogador.
     * @return EstadoProximaTela O estado para o qual o jogo deve transicionar.
     */
    EstadoProximaTela getProximaTelaEstado() const override;

    /**
     * @brief Implementação vazia da função virtual herdada. Não possui lógica nesta tela.
     */
    void resetEstado() override {}

    /**
     * @brief Reseta o estado de transição para NENHUM.
     * @details Essencial para que o gerenciador de telas não processe a mesma transição múltiplas vezes.
     */
    void resetProximaTelaEstado() override;
};

#endif
