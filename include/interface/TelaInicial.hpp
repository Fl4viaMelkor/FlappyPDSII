/**
 * @file TelaInicial.hpp
 * @brief Define a interface da tela de menu principal do jogo.
 */
#ifndef TELAINICIAL_HPP
#define TELAINICIAL_HPP

#include "tela_base.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <string>

/**
 * @class TelaInicial
 * @brief Representa o menu principal do jogo, servindo como ponto de partida.
 * @details Esta tela exibe o título do jogo e as opções de navegação (Iniciar Jogo, Recordes, Sair).
 * Ela gerencia a seleção do menu via teclado e exibe uma animação como cursor.
 */
class TelaInicial : public TelaBase {
private:
    // --- Recursos Gráficos ---
    ALLEGRO_FONT* fonte_titulo;         ///< Ponteiro para a fonte do título principal.
    ALLEGRO_FONT* fonte_opcoes;         ///< Ponteiro para a fonte das opções do menu.
    
    // --- Animação do Cursor ---
    ALLEGRO_BITMAP* animacao_spritesheet; ///< Ponteiro para o bitmap do sprite sheet da animação.
    int anim_frame_largura;             ///< A largura em pixels de um único frame da animação.
    int anim_frame_atual;               ///< O índice do frame atual a ser desenhado (ex: 0, 1, 2).
    float anim_contador_tempo;          ///< Acumulador de tempo para controlar a velocidade da animação.
    float anim_delay_frame;             ///< O tempo que cada frame deve permanecer na tela.

    // --- Lógica do Menu ---
    std::vector<std::string> opcoes_menu; ///< Vetor que armazena os textos das opções do menu.
    int opcao_selecionada_idx;          ///< Índice da opção de menu atualmente selecionada pelo jogador.
    EstadoProximaTela estado_escolhido; ///< Armazena a ação escolhida pelo jogador para a transição de tela.

    // --- Cores e Dimensões ---
    ALLEGRO_COLOR cor_titulo;             ///< Cor do texto do título.
    ALLEGRO_COLOR cor_opcao_normal;       ///< Cor do texto de uma opção não selecionada.
    ALLEGRO_COLOR cor_opcao_selecionada;  ///< Cor do texto da opção selecionada (destaque).
    int largura_tela;                     ///< Largura da tela virtual.
    int altura_tela;                      ///< Altura da tela virtual.

public:
    /**
     * @brief Construtor da TelaInicial.
     * @param largura A largura da tela virtual na qual a tela será desenhada.
     * @param altura A altura da tela virtual na qual a tela será desenhada.
     */
    TelaInicial(int largura, int altura);

    /**
     * @brief Destrutor da TelaInicial. Libera todos os recursos alocados (fontes e bitmaps).
     */
    ~TelaInicial() override;

    /**
     * @brief Atualiza a lógica da tela, como a animação do cursor.
     */
    void update() override;

    /**
     * @brief Desenha todos os elementos visuais do menu na tela.
     */
    void draw() override;

    /**
     * @brief Processa eventos de input do jogador para navegar no menu.
     * @param evento O evento do Allegro a ser processado.
     */
    void step(ALLEGRO_EVENT& evento) override;

    /**
     * @brief Retorna o estado da próxima tela escolhido pelo jogador.
     * @return EstadoProximaTela O estado para o qual o jogo deve transicionar.
     */
    EstadoProximaTela getProximaTelaEstado() const override;

    /**
     * @brief Reseta o estado de transição para NENHUM após a transição ser processada.
     */
    void resetProximaTelaEstado() override;

    int getOpcaoSelecionadaIdx() const { return opcao_selecionada_idx; }
};

#endif
