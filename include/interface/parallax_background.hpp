/**
 * @file parallax_background.hpp
 * @brief Declaração da classe ParallaxBackground para gerenciar múltiplas camadas de fundo com efeito de paralaxe.
 */

#ifndef PARALLAX_BACKGROUND_HPP
#define PARALLAX_BACKGROUND_HPP

#include "camada_background.hpp"
#include <vector>

/**
 * @brief Classe responsável por gerenciar um conjunto de camadas de fundo com efeito de paralaxe.
 *
 * Essa classe permite adicionar, atualizar e desenhar várias camadas de fundo que se movem em velocidades diferentes,
 * criando um efeito visual de profundidade.
 */
class ParallaxBackground {
public:
    /**
     * @brief Construtor da classe ParallaxBackground.
     *
     * Inicializa o gerenciador de camadas de fundo com as dimensões da tela.
     *
     * @param larguraTela Largura da tela.
     * @param alturaTela Altura da tela.
     */
    ParallaxBackground(int larguraTela, int alturaTela);

    /**
     * @brief Destrutor da classe ParallaxBackground.
     *
     * Libera a memória alocada para as camadas de fundo.
     */
    ~ParallaxBackground();

    /**
     * @brief Adiciona uma nova camada de fundo ao gerenciador.
     *
     * Cria uma nova instância de CamadaBackground e a adiciona à lista de camadas.
     *
     * @param caminhoTextura Caminho para o arquivo de textura da camada.
     * @param velocidade Velocidade de movimento horizontal da camada.
     */
    void adicionarCamada(const std::string& caminhoTextura, float velocidade);

    /**
     * @brief Atualiza todas as camadas de fundo com base no tempo decorrido.
     *
     * @param deltaTime Tempo decorrido desde a última atualização, em segundos.
     */
    void update(float deltaTime);

    /**
     * @brief Desenha todas as camadas de fundo na tela.
     */
    void draw();

private:
    int larguraTela_; ///< Largura da tela.
    int alturaTela_;  ///< Altura da tela.
    std::vector<CamadaBackground*> camadas_; ///< Lista de camadas de fundo.
};

#endif // PARALLAX_BACKGROUND_HPP
