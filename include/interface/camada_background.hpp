#ifndef CAMADA_BACKGROUND_HPP
#define CAMADA_BACKGROUND_HPP

#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

using namespace std;

class CamadaBackground{
public:
    CamadaBackground(const string& caminhoTextura, float velocidade, int larguraTela, int alturaTela);
    ~CamadaBackground();

    void update(float deltaTime);
    void draw();
    void definirAlturaTela(int altura);
private:
    float velocidade_;
    float posicaoX_;

    int larguraTextura_;
    int alturaTextura_;
    int yOffset_;

    int larguraTela_;
    int alturaTela_;

    ALLEGRO_BITMAP* textura_;
};

#endif