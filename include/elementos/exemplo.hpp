#ifndef CANO_H
#define CANO_H

//biblioteca padrão
#include <allegro5/allegro.h>

//modulo para desenhar quadrado 
#include <allegro5/allegro_primitives.h> 

#include "../controladores/controlador.hpp"
#include "../fisica/hitbox.hpp"
#include "../controladores/ator.hpp"

using namespace std;
/*
Exemplo de implementação:

class ControladorCano:public Controlador{
    protected:
        momento_linear momentoLinear; // Momento linear com posição (referente ao centro de massa) e velocidade
        set<acao> disponiveis; // Lista de ações disponiveis para esse personagem

    public:
        ControladorCano(){
            momentoLinear.posicao = {200, 200};
            momentoLnear.velocidade = {-2, 0};
        }
        void executar(acao a, gravidade g){
            momentoLinear = Modulo_Fisico.atualizar_fisica(g, momentoLinear);
        };
};

class Poligono{
    protected:
        vector<reta> perimetro;
    public:
        Poligono(vector<reta> perimetro);
        void set_perimetro(vector<reta> perimetro);
        vector<reta> get_poligono();
};

class FiguraCano:public Figura{
    protected:
        vector<Poligono> poligonos;
        vector<Curva> circulos;
	public:
          Figura(coordenadas centro_massa){
            setFigura(centro_massa);
          }
          void setFigura(coordenadas centro_massa){
                    poligonos.push_back(new Retangulo(centro_massa));
           };
          ~Figura();
};

class HitboxCano:public Hitbox{
    protected:
        FiguraCano *cano;
        virtual bool noInterior(coordenadas p) = 0; // retorna verdadeiro se o ponto p está no interior (mas não na extremidade) da figura
        virtual bool noPerimetro(coordenadas p) = 0;// retorna verdadeiro se o ponto p está no perimetro (mas não na extremidade) da figura
    public:
        HitboxCano(FiguraCano
        virtual bool colisao(coordenadas p) = 0;
};

class Cano2D: public Desenho{
  protected:
    Figura figura2d;
    float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura;
  public:
    Desenho2D(Renderizador *renderizador, Figura figura2d);
    void desenhar();
};


class Cano:public Ator{
    Controlador *controller;
    gravidade g;
    Desenho grafico;
    public:
      Cano();

};

 */

class Cano2 {
    public:
    
    
        // Construtor recebe posição x, largura do cano, espaço entre os blocos, altura da tela, cor e espessura da linha
        Cano(float x, float largura, float abertura, float altura_tela, ALLEGRO_COLOR cor, float espessura);
    

        
        void draw() const;                  // Desenha os canos (superior e inferior)
        void move(float dx);                // Move os canos na horizontal (dx pode ser negativo para mover para esquerda)
    
    
        
        // Verifica se o cano saiu da tela e reseta a posição e altura aleatória
        void reset_if_out_of_screen(float limite_esquerdo, float posicao_ultimo_cano, float espacamento, float altura_tela);
    
    
        
        // Getters para pegar posição e largura (úteis para cálculo de reposicionamento)
        float getX() const { return m_x; }
        float getLargura() const { return m_largura; }
    
    
    
        // Verifica se um retângulo do jogador colide com o cano
        bool colisao_com_jogador(float jogador_x, float jogador_y, float jogador_largura, float jogador_altura) const;
    
    
    
        
    private:
        float m_x;            // Posição horizontal do cano (para ambos os blocos)
        float m_largura;      // Largura dos blocos (constante)
        float m_espaco;       // Espaço vertical entre os blocos (abertura)
        float m_y_topo;       // Altura do bloco superior (variável para fazer o espaço aleatório)
        ALLEGRO_COLOR m_color;  // Cor dos blocos
        float m_espessura;      // Espessura da linha (não usada para preenchidos, mas mantida)
    };
    
    
    
    #endif

 