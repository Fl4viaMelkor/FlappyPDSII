#ifndef CANO_H
#define CANO_H

//biblioteca padrão
#include <allegro5/allegro.h>

//modulo para desenhar quadrado 
#include <allegro5/allegro_primitives.h> 

using namespace std;

/*-------------------------------------------------------*/

//colisão -> o objeto_passaro vai ter que identificar
//largura, altura
//destrutor para canos
//gerar tres tipos de cano
//gerar canos a partir de uma dada quantidade de frames

//uma classe cano e tem um loop

class Cano {


    public:
        // Construtor: x, y são as coordenadas do canto superior esquerdo
        // side é o comprimento do lado do quadrado
        // color é a cor do quadrado
        Cano(float x1, float x2,  float y1, float y2, ALLEGRO_COLOR color , float espessura);
    
        // Desenha o quadrado na tela
        void draw() const;
    

    private:
        float m_x1;
        float m_x2;
        float m_y1;
        float m_y2;
        float m_espessura;
        ALLEGRO_COLOR m_color;
    };
    
    
#endif


 