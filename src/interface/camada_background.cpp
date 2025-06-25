/**
 * @file camada_background.cpp
 * @brief Implementação da classe CamadaBackground para controle de camadas de fundo.
 */

#include "../../include/interface/camada_background.hpp"
#include <iostream>

using namespace std;

/**
 * @brief Construtor da classe CamadaBackground.
 *
 * Inicializa a camada de fundo com a textura fornecida, sua velocidade,
 * e as dimensões da tela.
 *
 * @param caminhoTextura Caminho para o arquivo de textura.
 * @param velocidade Velocidade de movimento horizontal da camada.
 * @param larguraTela Largura da tela.
 * @param alturaTela Altura da tela.
 */
CamadaBackground::CamadaBackground(const string &caminhoTextura,
                                   float velocidade,
                                   int larguraTela,
                                   int alturaTela)
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

/**
 * @brief Destrutor da classe CamadaBackground.
 *
 * Libera a memória alocada para a textura.
 */
CamadaBackground::~CamadaBackground()
{
    if (textura_ != nullptr)
    {
        al_destroy_bitmap(textura_);
        textura_ = nullptr;
    }
}

/**
 * @brief Atualiza a posição da camada de fundo com base no tempo decorrido.
 *
 * @param deltaTime Tempo decorrido desde a última atualização, em segundos.
 */
void CamadaBackground::update(float deltaTime)
{
    if (!textura_)
        return;

    posicaoX_ -= velocidade_ * deltaTime;

    if (posicaoX_ <= -larguraTela_)
    {
        posicaoX_ += larguraTela_;
    }
}

/**
 * @brief Define a altura da tela para ajuste da exibição.
 *
 * (Atualmente não implementado.)
 *
 * @param altura Nova altura da tela.
 */
void CamadaBackground::definirAlturaTela(int altura)
{
    // Método não implementado
}

/**
 * @brief Desenha a camada de fundo na tela.
 *
 * Realiza o desenho da textura duas vezes para criar o efeito de paralaxe.
 */
void CamadaBackground::draw()
{
    if (!textura_)
        return;

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
