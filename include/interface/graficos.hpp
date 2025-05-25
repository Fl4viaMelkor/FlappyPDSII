//
// Created by leand on 25/05/2025.
//
#include "figuras_basicas.hpp"
#include "renderizador.hpp"

#ifndef FIGURAS_HPP
#define FIGURAS_HPP


class Desenho{

};

class Sprite{
  protected:
    string url;
  public:
    Sprite(string url);
    string getUrl(string url);
    void setUrl(string url);
};


#endif //FIGURAS_HPP
