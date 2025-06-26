//
// Created by leand on 25/05/2025.
//

#ifndef HITBOX_HPP
#define HITBOX_HPP

#include "../interface/figuras_basicas.hpp"
#include "../util.hpp"
#include "player_exception.hpp"
#include <cmath>
#include <vector>

/**
 * @brief Interface para elementos colidíveis no jogo.
 *
 * Define a estrutura comum para classes que possuem detecção de colisão com pontos.
 * Serve como base para diferentes tipos de hitboxes, como retângulos, polígonos, curvas, etc.
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
     * @brief Retorna os pontos que definem a forma geométrica da hitbox.
     * @return Vetor de coordenadas da forma.
     */
    virtual std::vector<coordenadas> get_pontos() const = 0;
};

/**
 * @brief Implementa uma hitbox retangular.
 *
 * A posição do retângulo é definida a partir de seu canto inferior esquerdo,
 * e ele possui uma largura (base) e altura fixas. Pode ser utilizado para colisões
 * com obstáculos, personagens ou áreas de interação.
 */
class RetanguloHitboxAbstract : public Colidivel {
  protected:
    coordenadas ponto_inferior_esquerdo; ///< Canto inferior esquerdo do retângulo.
    float base;                          ///< Largura do retângulo.
    float altura;                        ///< Altura do retângulo.

  public:
    /**
     * @brief Construtor que define a posição e dimensões do retângulo.
     * @param p1 Ponto inferior esquerdo.
     * @param b Largura (base).
     * @param a Altura.
     */
    RetanguloHitbox(coordenadas p1, float b, float a)
      : ponto_inferior_esquerdo(p1), base(b), altura(a) {}

    /**
     * @brief Retorna a coordenada X do ponto de referência da hitbox.
     * @return Valor da posição X.
     */
    float getX() const { return ponto_inferior_esquerdo.x; }

    /**
     * @brief Retorna a coordenada Y do ponto de referência da hitbox.
     * @return Valor da posição Y.

     */
    float getY() const { return ponto_inferior_esquerdo.y; }

    /**
     * @brief Retorna a altura da hitbox.
     * @return Valor da altura.
     */
    float getAltura() const { return altura; }

    /**
     * @brief Comportamento padrão ao ocorrer colisão.
     *
     * Pode ser sobrescrito em classes derivadas. Aqui, não faz nada por padrão.
     */
    void onCollision() override {
        // comportamento padrão ou vazio
    }

    /**
     * @brief Verifica se o ponto colide com a hitbox.
     * @param p Ponto a ser testado.
     * @return true se estiver no interior ou perímetro.
     */
    bool colisao(coordenadas p) override {
        return noInterior(p) || noPerimetro(p);
    }

    /**
     * @brief Verifica se o ponto está dentro da área do retângulo.
     * @param p Coordenadas a verificar.
     * @return true se estiver no interior.
     */
    bool noInterior(const coordenadas &p) const override;

    /**
     * @brief Verifica se o ponto está exatamente no contorno do retângulo.
     * @param p Coordenadas a verificar.
     * @return true se estiver no perímetro.
     */
    bool noPerimetro(const coordenadas &p) const override;

    /**
     * @brief Retorna os quatro cantos do retângulo.
     * @return Vetor com as coordenadas dos vértices.
     */
    std::vector<coordenadas> get_pontos() const override;
};
#endif
