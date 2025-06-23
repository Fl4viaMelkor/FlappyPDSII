// Created by Flávia
#ifndef SPRITE_ANIMADO_HPP
#define SPRITE_ANIMADO_HPP

#include <string>
#include <allegro5/allegro5.h>
#include "util.hpp"

/**
 * @brief Classe responsável por animar sprites usando uma sprite sheet.
 *
 * Esta classe gerencia a exibição de sprites animados, manipulando os quadros (frames) individuais de uma imagem
 * que contém múltiplos quadros em sequência. Ideal para animações simples de personagens ou objetos.
 *
 * A imagem é dividida em `numFrames` de tamanho igual, com `frameLargura` x `frameAltura`, e a animação
 * é exibida a uma taxa de quadros definida por `fps`.
 */
class SpriteAnimado {
private:
    ALLEGRO_BITMAP* bitmap;     ///< Bitmap contendo a sprite sheet.
    int frameLargura;           ///< Largura de um único quadro.
    int frameAltura;            ///< Altura de um único quadro.
    int numFrames;              ///< Número total de quadros na animação.
    int frameAtual;             ///< Índice do quadro atual a ser exibido.
    float tempoPorFrame;        ///< Tempo que cada quadro deve durar (em segundos).
    float tempoAcumulado;       ///< Tempo acumulado desde o último avanço de quadro.

public:
    /**
     * @brief Construtor da animação.
     * 
     * @param filename Caminho do arquivo da sprite sheet.
     * @param numFrames Número total de quadros.
     * @param frameLargura Largura de cada quadro individual.
     * @param frameAltura Altura de cada quadro individual.
     * @param fps Quadros por segundo da animação.
     */
    SpriteAnimado(const std::string& filename, int numFrames, int frameLargura, int frameAltura, float fps);

    /**
     * @brief Destrutor que libera a memória da imagem carregada.
     */
    ~SpriteAnimado();

    /**
     * @brief Atualiza a animação com base no tempo decorrido.
     * 
     * @param deltaTime Tempo decorrido desde a última atualização (em segundos).
     */
    void update(float deltaTime);

    /**
     * @brief Desenha o quadro atual da animação na posição indicada.
     * 
     * @param pos Posição na tela onde o sprite será desenhado.
     * @param flip_vertical Se verdadeiro, espelha o sprite verticalmente.
     */
    void draw(const coordenadas& pos, bool flip_vertical = false) const;

    /**
     * @brief Retorna a largura de um quadro individual.
     * @return Largura do quadro.
     */
    int getWidth() const;

    /**
     * @brief Retorna a altura de um quadro individual.
     * @return Altura do quadro.
     */
    int getHeight() const;
};

#endif // SPRITE_ANIMADO_HPP
