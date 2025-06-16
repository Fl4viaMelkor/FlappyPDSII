#include "../../include/interface/parallax_background.hpp"

ParallaxBackground::ParallaxBackground(int larguraTela, int alturaTela)
: larguraTela_(larguraTela), alturaTela_(alturaTela){}

ParallaxBackground::~ParallaxBackground(){
    for (CamadaBackground* camada: camadas_){
        delete camada;
    }
    camadas_.clear();
}

void ParallaxBackground::adicionarCamada(const string& caminhoTextura, float velocidade){
    CamadaBackground* novaCamada = new CamadaBackground(caminhoTextura, velocidade, larguraTela_, alturaTela_);
    camadas_.push_back(novaCamada);
}

void ParallaxBackground::update(float deltaTime){
    for (CamadaBackground* camada : camadas_){
        camada->update(deltaTime);
    }
}

void ParallaxBackground::draw(){
    for (CamadaBackground* camada : camadas_){
        camada->draw();
    }
}
