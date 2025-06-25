/**
 * @file parallax_background.cpp
 * @brief Implementação da classe ParallaxBackground para gerenciar múltiplas camadas de fundo.
 */

#include "../../include/interface/parallax_background.hpp"
#include "../../include/interface/camada_background.hpp"

/**
 * @brief Construtor da classe ParallaxBackground.
 *
 * Inicializa o gerenciador de camadas de fundo com as dimensões da tela.
 *
 * @param larguraTela Largura da tela.
 * @param alturaTela Altura da tela.
 */
ParallaxBackground::ParallaxBackground(int larguraTela, int alturaTela)
  : larguraTela_(larguraTela)
  , alturaTela_(alturaTela)
{
}

/**
 * @brief Destrutor da classe ParallaxBackground.
 *
 * Libera a memória alocada para as camadas de fundo e limpa a lista de camadas.
 */
ParallaxBackground::~ParallaxBackground()
{
    for (CamadaBackground *camada : camadas_)
        delete camada;
    camadas_.clear();
}

/**
 * @brief Adiciona uma nova camada de fundo ao gerenciador.
 *
 * Cria uma nova instância de CamadaBackground e a adiciona à lista de camadas.
 *
 * @param caminhoTextura Caminho para o arquivo de textura da camada.
 * @param velocidade Velocidade de movimento horizontal da camada.
 */
void ParallaxBackground::adicionarCamada(const string &caminhoTextura, float velocidade)
{
    CamadaBackground *novaCamada = new CamadaBackground(caminhoTextura, velocidade, larguraTela_, alturaTela_);
    camadas_.push_back(novaCamada);
}

/**
 * @brief Atualiza todas as camadas de fundo com base no tempo decorrido.
 *
 * @param deltaTime Tempo decorrido desde a última atualização, em segundos.
 */
void ParallaxBackground::update(float deltaTime)
{
    for (CamadaBackground *camada : camadas_)
        camada->update(deltaTime);
}

/**
 * @brief Desenha todas as camadas de fundo na tela.
 */
void ParallaxBackground::draw()
{
    for (CamadaBackground *camada : camadas_)
        camada->draw();
}
