#ifndef PLAYER_EXCEPTION_HPP
#define PLAYER_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class PlayerException : public std::runtime_error {
public:
    PlayerException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif
