#include "objeto_cano.hpp"

 

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_color.h> // Para al_map_rgb

using namespace std;

//Construtor
Cano::Cano(float x1, float x2, float y1, float y2, ALLEGRO_COLOR color, float espessura)
    : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_espessura(espessura), m_color(color) {
    // Não há necessidade de código adicional aqui, pois a inicialização já é feita na lista de inicialização
}


// Método para desenhar o quadrado
void Cano::draw() const {

    //desenhar retangulo na tela
    al_draw_rectangle(m_x1, m_x2, m_y1, m_y2 , m_color, m_espessura);
    
    
}