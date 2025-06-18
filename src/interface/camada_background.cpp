#include "../../include/interface/camada_background.hpp"
#include <iostream>

using namespace	std;

CamadaBackground::CamadaBackground(const string &caminhoTextura,
		float velocidade, int larguraTela, int alturaTela)
	: velocidade_(velocidade),
		posicaoX_(0.0f),
		larguraTextura_(0),
		alturaTextura_(0),
		yOffset_(0),
		larguraTela_(larguraTela),
		alturaTela_(alturaTela),
		textura_(al_load_bitmap(caminhoTextura.c_str()))
{
	if (!textura_)
	{
		cerr << "Erro ao carregar bitmap: " << caminhoTextura << endl;
	}
	larguraTextura_ = al_get_bitmap_width(textura_);
	alturaTextura_ = al_get_bitmap_height(textura_);
}

CamadaBackground::~CamadaBackground()
{
	if (textura_ != nullptr)
	{
		al_destroy_bitmap(textura_);
		textura_ = nullptr;
	}
}

void CamadaBackground::update(float deltaTime)
{
	if (!textura_)
		return ;
	posicaoX_ -= velocidade_ * deltaTime;
	if (posicaoX_ <= -larguraTela_)
	{
		posicaoX_ += larguraTela_;
	}
}

void CamadaBackground::definirAlturaTela(int altura)
{
}

void CamadaBackground::draw()
{
	if (!textura_)
		return ;

	al_draw_scaled_bitmap(
		textura_,
		0,
		0,
		larguraTextura_,
		alturaTextura_,
		posicaoX_,
		0,
		larguraTela_,
		alturaTela_,
		0);

	al_draw_scaled_bitmap(
		textura_,
		0,
		0,
		larguraTextura_,
		alturaTextura_,
		posicaoX_ + larguraTela_,
		0,
		larguraTela_,
		alturaTela_,
		0);
}