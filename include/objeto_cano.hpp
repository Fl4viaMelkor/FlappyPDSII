#ifndef CANO_H
#define CANO_H


//
#include "sprite.hpp"

//biblioteca padrão
#include <allegro5/allegro.h>

//modulo para desenhar quadrado 
#include <allegro5/allegro_primitives.h> 

using namespace std;

#include <vector>

class Cano {
  public:
  // Construtor recebe posição x, largura do cano, espaço entre os blocos, altura da tela, cor e espessura da linha
    Cano(float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura, const std::string& sprite_path);

    void draw() const;                      // Desenha os canos (superior e inferior)
    void move(float dx);                    // Move os canos na horizontal (dx pode ser negativo para mover para esquerda)
    // Verifica se o cano saiu da tela e reseta a posição e altura aleatória
    void reset_if_out_of_screen(float limite_esquerdo, float posicao_ultimo_cano, float espacamento, float altura_tela);
    
    
        
    // Getters para pegar posição e largura (úteis para cálculo de reposicionamento)
    float getX() const { return m_x; }
    float getLargura() const { return m_largura; }
    
    
    
    // Verifica se um retângulo do jogador colide com o cano
    //bool colisao_com_jogador(float jogador_x, float jogador_y, float jogador_largura, float jogador_altura) const;
    
  private:
    float m_x;            // Posição horizontal do cano (para ambos os blocos)
    float m_largura;      // Largura dos blocos (constante)
    float m_espaco;       // Espaço vertical entre os blocos (abertura)
    float m_y_topo;       // Altura do bloco superior (variável para fazer o espaço aleatório)
    ALLEGRO_COLOR m_color;  // Cor dos blocos
    float m_espessura;      // Espessura da linha (não usada para preenchidos, mas mantida)

    SpriteAnimado sprite;  // NOVO
};
    
    
    #endif

 