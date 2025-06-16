#include "../../include/interface/camada_background.hpp"
#include <iostream>
using namespace std;

CamadaBackground::CamadaBackground(const string& caminhoTextura, float velocidade, int larguraTela, int alturaTela)
    : textura_(nullptr), velocidade_(velocidade), posicaoX_(0.0f),
    larguraTextura_(0), alturaTextura_(0), yOffset_(0),
    larguraTela_(larguraTela), alturaTela_(alturaTela)
    {

    textura_ = al_load_bitmap(caminhoTextura.c_str());

    if(!textura_){
        cerr << "Erro ao carregar bitmap: " << caminhoTextura << endl;
    }
    larguraTextura_ = al_get_bitmap_width(textura_);
    alturaTextura_ = al_get_bitmap_height(textura_);
}

CamadaBackground::~CamadaBackground(){
    if (textura_ != nullptr){
        al_destroy_bitmap(textura_);
        textura_ = nullptr;
    }
}

void CamadaBackground::update(float deltaTime){
    if (!textura_) return;

    posicaoX_ -= velocidade_ * deltaTime;

    if(posicaoX_ <= -larguraTela_){
        posicaoX_ += larguraTela_;
    }

}

void CamadaBackground::definirAlturaTela(int altura){

}

void CamadaBackground::draw(){
    if (!textura_) return;

    al_draw_scaled_bitmap(
        textura_,
        0, 0, //Posição e dimensão da textura de origem
        larguraTextura_, alturaTextura_, //Largura e altura da textura de origem
        posicaoX_, 0, //Posição x e y na tela
        larguraTela_, alturaTela_,
        0
    );

    // Segunda imagem utilizada no loop
    al_draw_scaled_bitmap(
        textura_,
        0, 0, //Posição e dimensão da textura de origem
        larguraTextura_, alturaTextura_, //Largura e altura da textura de origem
        posicaoX_ + larguraTela_, 0, //Posição x e y na tela
        larguraTela_, alturaTela_,
        0
    );
}