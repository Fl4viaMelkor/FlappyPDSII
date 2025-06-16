#ifndef PARALLAX_BACKGROUND_HPP
#define PARALLAX_BACKGROUND_HPP

#include "camada_background.hpp"
#include <vector>

class ParallaxBackground{
public:
    ParallaxBackground(int larguraTela, int alturaTela);
    ~ParallaxBackground();

    void adicionarCamada(const string& caminhoTextura, float velocidade);

    void update(float deltaTime);
    void draw();

private:
    int larguraTela_;
    int alturaTela_;
    vector<CamadaBackground*> camadas_;
};

#endif