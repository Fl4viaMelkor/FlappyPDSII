/**
 * @file camada_background.hpp
 * @brief Declaração da classe CamadaBackground para gerenciar camadas de fundo animadas.
 */

#ifndef CAMADA_BACKGROUND_HPP
#define CAMADA_BACKGROUND_HPP

#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

using namespace std;

/**
 * @brief Classe responsável por representar uma camada de fundo animada.
 *
 * A camada de fundo pode se mover horizontalmente para criar efeitos visuais,
 * como paralaxe, em um jogo ou aplicativo gráfico.
 */
class CamadaBackground {
public:
    /**
     * @brief Construtor da classe CamadaBackground.
     *
     * Inicializa uma camada de fundo com a textura especificada e propriedades de movimento.
     *
     * @param caminhoTextura Caminho para o arquivo de textura.
     * @param velocidade Velocidade de movimento horizontal da camada.
     * @param larguraTela Largura da tela.
     * @param alturaTela Altura da tela.
     */
    CamadaBackground(const string& caminhoTextura, float velocidade, int larguraTela, int alturaTela);

    /**
     * @brief Destrutor da classe CamadaBackground.
     *
     * Libera a memória alocada para a textura da camada.
     */
    ~CamadaBackground();

    /**
     * @brief Atualiza a posição da camada com base no tempo decorrido.
     *
     * @param deltaTime Tempo decorrido desde a última atualização, em segundos.
     */
    void update(float deltaTime);

    /**
     * @brief Desenha a camada de fundo na tela.
     */
    void draw();

    /**
     * @brief Define a altura da tela para ajustar a exibição da camada.
     *
     * (Atualmente não implementado.)
     *
     * @param altura Nova altura da tela.
     */
    void definirAlturaTela(int altura);

    /**
    * @brief Retorna a posição X atual da camada.
     * @return A posição X.
     */
    float getPosicaoX() const {
        return posicaoX_;
    }

private:
    float velocidade_;      ///< Velocidade de movimento horizontal da camada.
    float posicaoX_;        ///< Posição horizontal atual da camada.

    int larguraTextura_;    ///< Largura da textura da camada.
    int alturaTextura_;     ///< Altura da textura da camada.
    int yOffset_;           ///< Deslocamento vertical da camada.

    int larguraTela_;       ///< Largura da tela onde a camada será exibida.
    int alturaTela_;        ///< Altura da tela onde a camada será exibida.

    ALLEGRO_BITMAP* textura_; ///< Ponteiro para a textura da camada.
};

#endif // CAMADA_BACKGROUND_HPP
