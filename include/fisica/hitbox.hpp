//
// Created by leand on 25/05/2025.
//

#ifndef HITBOX_HPP
#define HITBOX_HPP

#include "../interface/figuras_basicas.hpp"
#include "../util.hpp"
#include <cmath>
#include <vector>

/**
 * @brief Interface para elementos colidíveis no jogo.
 * 
 * Define a estrutura comum para classes que possuem detecção de colisão.
 */
class Colidivel {
  public:
    virtual ~Colidivel() = default;

    /**
     * @brief Define o comportamento do objeto ao colidir.
     */
    virtual void onCollision() = 0;

    /**
     * @brief Verifica se há colisão com um ponto específico.
     * @param p Coordenada a ser verificada.
     * @return true se houver colisão com o ponto.
     */
    virtual bool colisao(coordenadas p) = 0;

    /**
     * @brief Verifica se um ponto está no interior da forma.
     * @param p Coordenada a ser verificada.
     * @return true se o ponto estiver dentro da hitbox.
     */
    virtual bool noInterior(const coordenadas &p) const = 0;

    /**
     * @brief Verifica se um ponto está no perímetro da forma.
     * @param p Coordenada a ser verificada.
     * @return true se o ponto estiver no contorno da hitbox.
     */
    virtual bool noPerimetro(const coordenadas &p) const = 0;

    /**
     * @brief Retorna os pontos que definem a forma geométrica.
     * @return Vetor de coordenadas da forma.
     */
    virtual vector<coordenadas> get_pontos() const = 0;
};

/**
 * @brief Implementa uma hitbox retangular.
 * 
 * Baseada em um ponto inferior esquerdo, com altura e base definidas.
 */
class RetanguloHitbox : public Colidivel {
  protected:
    coordenadas ponto_inferior_esquerdo; ///< Canto inferior esquerdo do retângulo.
    float base;   ///< Largura do retângulo.
    float altura; ///< Altura do retângulo.

  public:
    /**
     * @brief Construtor que define a posição e dimensões do retângulo.
     * @param p1 Ponto inferior esquerdo.
     * @param b Largura (base).
     * @param a Altura.
     */
    RetanguloHitbox(coordenadas p1, float b, float a)
      : ponto_inferior_esquerdo(p1), base(b), altura(a)
    {
    }

     /**
     * @brief Retorna a coordenada Y do ponto de referência do objeto.
     */
    float getY() const { return ponto_inferior_esquerdo.y; }

    /**
     * @brief Retorna a altura da hitbox.
     */
    float getAltura() const { return altura; }

    bool noInterior(const coordenadas &p) const override;
    bool noPerimetro(const coordenadas &p) const override;
    vector<coordenadas> get_pontos() const override;
};

// Comentários futuros para as classes abaixo, se forem implementadas:
//
// class PoligonoHitbox: public Hitbox { ... }
// class CurvaHitbox: public Hitbox { ... }
// class FiguraHitbox: public Hitbox { ... }

#endif