// Created By Flávia

#ifndef PLAYER_EXCEPTION_HPP
#define PLAYER_EXCEPTION_HPP

#include <stdexcept>
#include <string>

/**
 * @brief Exceção personalizada para erros relacionados ao jogador.
 *
 * Essa classe permite lançar mensagens específicas quando ocorrem problemas com a lógica ou estado do jogador.
 */
class PlayerException : public std::runtime_error {
  public:
    /**
     * @brief Construtor que recebe uma mensagem de erro personalizada.
     * @param message Texto explicativo sobre o erro ocorrido.
     */
    PlayerException(const std::string &message)
      : std::runtime_error(message)
    {
    }
};

class ColisaoException : public PlayerException {
  public:
    /**
     * @brief Construtor que recebe uma mensagem de erro personalizada.
     * @param message Texto explicativo sobre o erro ocorrido.
     */
    ColisaoException(const std::string &message)
      : std::runtime_error(message)
    {
    }
};

#endif // PLAYER_EXCEPTION_HPP
