#ifndef SPRITE_ANIMADO_HPP
#define SPRITE_ANIMADO_HPP

#include <string>
#include <allegro5/allegro5.h>
#include "util.hpp"

// Created by Flávia

// No construtor, basta dizer o número de frames da sprite sheet como um todo,
// além da altura, largura de UM único frame e quantos frames por segundo deseja que a animação tenha.

class SpriteAnimado {
private:
    ALLEGRO_BITMAP* bitmap;     // Bitmap contendo a sprite sheet
    int frameLargura;           // Largura de um frame individual
    int frameAltura;            // Altura de um frame individual
    int numFrames;              // Número total de quadros (frames) da animação
    int frameAtual;             // Frame atual da animação
    float tempoPorFrame;        // Tempo que cada frame deve durar (em segundos)
    float tempoAcumulado;       // Tempo acumulado para decidir quando trocar de frame

public:
    // Construtor: inicializa o sprite animado com a imagem e configurações de animação
    SpriteAnimado(const std::string& filename, int numFrames, int frameLargura, int frameAltura, float fps);

    // Destrutor: libera o bitmap
    ~SpriteAnimado();

    // Atualiza o frame da animação de acordo com o tempo decorrido
    void update(float deltaTime);

    // Desenha o sprite na tela, com opção de espelhamento vertical
    void draw(const coordenadas& pos, bool flip_vertical = false) const;

    // Retorna a largura de um frame
    int getWidth() const;

    // Retorna a altura de um frame
    int getHeight() const;
};

#endif
