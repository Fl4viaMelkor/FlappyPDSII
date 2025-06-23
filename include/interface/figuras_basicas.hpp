//
// Created by leand on 25/05/2025.
//

#ifndef FIGURAS_BASICAS_HPP
#define FIGURAS_BASICAS_HPP

#include "../util.hpp"
#include <vector>

/**
 * @brief Interface para formas geométricas fechadas.
 * 
 * Define métodos para verificar se um ponto está dentro ou sobre o perímetro da forma.
 */
class Forma_Geometrica_Fechada {
  public:
    /**
     * @brief Verifica se o ponto está no interior da figura (excluindo bordas).
     * @param p Coordenada do ponto.
     * @return true se estiver no interior.
     */
    virtual bool noInterior(coordenadas p) = 0;

    /**
     * @brief Verifica se o ponto está no perímetro da figura.
     * @param p Coordenada do ponto.
     * @return true se estiver no perímetro.
     */
    virtual bool noPerimetro(coordenadas p) = 0;
};

/**
 * @brief Representa um polígono definido por um vetor de coordenadas.
 */
class Poligono : public Forma_Geometrica_Fechada {
  protected:
    vector<coordenadas> perimetro; ///< Pontos que definem os lados do polígono.

  public:
    /**
     * @brief Construtor que define o polígono com base em pontos.
     * @param p Vetor de pontos que definem o contorno.
     */
    Poligono(vector<coordenadas> p);

    /**
     * @brief Define ou redefine o perímetro do polígono.
     * @param p Vetor de coordenadas.
     */
    void setPerimetro(vector<coordenadas> p);

    bool noInterior(coordenadas p) override;
    bool noPerimetro(coordenadas p) override;

    /**
     * @brief Retorna os pontos que formam o polígono.
     * @return Vetor de coordenadas.
     */
    vector<coordenadas> get_poligono();
};

/**
 * @brief Representa um retângulo como um tipo de polígono.
 */
class Retangulo : public Poligono {
  public:
    /**
     * @brief Construtor com os quatro vértices do retângulo.
     */
    Retangulo(coordenadas p1, coordenadas p2, coordenadas p3, coordenadas p4);

    bool noInterior(coordenadas p) override;
    bool noPerimetro(coordenadas p) override;
};

/**
 * @brief Representa um círculo com centro e raio.
 */
class Circulo : public Forma_Geometrica_Fechada {
  protected:
    coordenadas centro; ///< Centro do círculo.
    float radius;       ///< Raio do círculo.

  public:
    /**
     * @brief Construtor do círculo.
     * @param centro Centro do círculo.
     * @param radius Raio do círculo.
     */
    Circulo(coordenadas centro, float radius);

    /**
     * @brief Define um novo centro e raio para o círculo.
     */
    void set_circulo(coordenadas centro, float radius);

    bool noInterior(coordenadas p) override;
    bool noPerimetro(coordenadas p) override;
};

/**
 * @brief Representa uma figura composta por várias formas geométricas fechadas.
 */
class Figura : public Forma_Geometrica_Fechada {
  protected:
    vector<Forma_Geometrica_Fechada> elementos; ///< Lista de formas que compõem a figura.
    coordenadas ponto_referencia;               ///< Ponto de referência para a figura.

  public:
    /**
     * @brief Construtor com ponto de referência.
     * @param ponto_referencia Ponto central ou base da figura composta.
     */
    Figura(coordenadas ponto_referencia);

    /**
     * @brief Define ou redefine a figura com base em novo ponto de referência.
     */
    void setFigura(coordenadas ponto_referencia);

    bool noInterior(coordenadas p) override;
    bool noPerimetro(coordenadas p) override;
};

#endif // FIGURAS_BASICAS_HPP
