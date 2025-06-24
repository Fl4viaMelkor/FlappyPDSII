/**
 * @file TelaHighScore.hpp
 * @brief Define a interface da tela que exibe os recordes do jogo.
 */
#ifndef TELAHIGHSCORE_HPP
#define TELAHIGHSCORE_HPP

#include "tela_base.hpp"
#include "../dados/logger.hpp"
#include <allegro5/allegro_font.h>
#include <string>
#include <vector>

/**
 * @class TelaHighScores
 * @brief Representa a tela que exibe a lista de maiores pontuações.
 * @details Esta tela é acessada a partir do menu principal. Ela utiliza o PlayerLogger
 * para buscar os dados dos jogadores do banco de dados e os exibe de forma ordenada.
 */
class TelaHighScores : public TelaBase {
private:
    ALLEGRO_FONT* fonte_titulo; ///< Ponteiro para a fonte do título "RECORDES".
    ALLEGRO_FONT* fonte_scores; ///< Ponteiro para a fonte usada para listar os scores.
    
    PlayerLogger& logger; ///< Referência para o gerenciador de dados.
    
    EstadoProximaTela proxima_tela_estado; ///< Armazena o estado de transição (voltar ao menu).
    
    int largura_tela; ///< Largura da tela virtual.
    int altura_tela;  ///< Altura da tela virtual.

public:
    /**
     * @brief Construtor da TelaHighScores.
     * @param logger_ref Uma referência para o objeto PlayerLogger que gerencia o banco de dados.
     * @param largura A largura da tela virtual.
     * @param altura A altura da tela virtual.
     */
    TelaHighScores(PlayerLogger& logger_ref, int largura, int altura);

    /**
     * @brief Destrutor da TelaHighScores.
     * @details Libera os recursos de fonte alocados.
     */
    ~TelaHighScores() override;

    /**
     * @brief Atualiza a lógica da tela (vazio para esta tela estática).
     */
    void update() override;

    /**
     * @brief Desenha a lista de recordes na tela.
     */
    void draw() override;

    /**
     * @brief Processa eventos de input, como a tecla ESC para voltar.
     * @param evento O evento do Allegro a ser processado.
     */
    void step(ALLEGRO_EVENT& evento) override;
    
    /**
     * @brief Retorna o estado da próxima tela (voltar ao menu).
     * @return EstadoProximaTela O estado para o qual o jogo deve transicionar.
     */
    EstadoProximaTela getProximaTelaEstado() const override;

    /**
     * @brief Reseta o estado de transição da tela.
     */
    void resetProximaTelaEstado() override;
};
#endif
