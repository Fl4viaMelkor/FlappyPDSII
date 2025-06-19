//
// Created by leand on 24/05/2025.
//

#ifndef REGRAS_JOGO_HPP
#define REGRAS_JOGO_HPP
#include "../util.hpp"
#include "./hitbox.hpp"
#include <iostream>
#include <set>

class Detector_Colisao {
  private:
    set<Colidivel *> elementos;
    Colidivel &player;

  public:
    explicit Detector_Colisao(Colidivel &player)
      : player(player)
    {
    }
    void registrar(Colidivel *e); // Registra um elemento novo e retorna verdadeiro se ele foi inserido com
                                  // sucesso e falso caso ele já exista
    void remover(Colidivel *e);
    void detectar() const;
};

class GameEndException : public std::exception {
  public:
    explicit GameEndException(const std::string &message = "Aconteceu uma colisão")
      : m_message(message)
    {
    }

    const char *what() const noexcept override { return m_message.c_str(); }

  private:
    string m_message;
};

#endif // REGRAS_JOGO_HPP
