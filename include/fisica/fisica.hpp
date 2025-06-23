//
// Created by leand on 24/05/2025.
//

#ifndef REGRAS_JOGO_HPP
#define REGRAS_JOGO_HPP

#include "../util.hpp"
#include "./hitbox.hpp"
#include <iostream>
#include <set>

/**
 * @brief Responsável por detectar colisões entre o jogador e outros objetos colidíveis no jogo.
 */
class Detector_Colisao {
  private:
    set<Colidivel *> elementos; ///< Conjunto de elementos colidíveis registrados.
    Colidivel &player;          ///< Referência ao objeto do jogador (também colidível).

  public:
    /**
     * @brief Construtor que recebe uma referência ao jogador.
     * @param player Referência ao objeto do jogador.
     */
    explicit Detector_Colisao(Colidivel &player)
      : player(player)
    {
    }

    /**
     * @brief Registra um novo elemento colidível.
     * 
     * @param e Ponteiro para o elemento colidível.
     * @note Elementos repetidos não são adicionados novamente.
     */
    void registrar(Colidivel *e);

    /**
     * @brief Remove um elemento colidível do conjunto.
     * @param e Ponteiro para o elemento a ser removido.
     */
    void remover(Colidivel *e);

    /**
     * @brief Verifica se o jogador colidiu com algum dos elementos registrados.
     * 
     * Se houver colisão, a exceção `GameEndException` pode ser lançada.
     */
    void detectar() const;
};

/**
 * @brief Exceção lançada quando o jogo deve terminar (por exemplo, ao ocorrer uma colisão fatal).
 */
class GameEndException : public std::exception {
  public:
    /**
     * @brief Construtor com mensagem personalizada.
     * @param message Mensagem explicando o motivo do encerramento do jogo.
     */
    explicit GameEndException(const std::string &message = "Aconteceu uma colisão")
      : m_message(message)
    {
    }

    /**
     * @brief Retorna a mensagem de erro.
     * @return Mensagem C-style (const char*).
     */
    const char *what() const noexcept override { return m_message.c_str(); }

  private:
    string m_message; ///< Mensagem descritiva do erro.
};

#endif // REGRAS_JOGO_HPP

