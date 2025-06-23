#ifndef PLAYER_EXCEPTION_HPP
#define PLAYER_EXCEPTION_HPP

#include <stdexcept>
#include <string>

//Created By Flávia

// Classe de exceção personalizada para erros relacionados ao jogador (Player)
class PlayerException : public std::runtime_error {
public:
    // Construtor que recebe uma mensagem de erro
    PlayerException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif
