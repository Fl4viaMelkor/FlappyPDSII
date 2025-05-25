//
// Created by leand on 25/05/2025.
//

#ifndef INFO_COLISAO_HPP
#define INFO_COLISAO_HPP


#pragma once
#include <glm/glm.hpp> // Para matemática vetorial
#include <memory>
#include <vector>

// Estrutura contendo informações detalhadas da colisão
class CollisionInfo {
    public:
        // Construtor básico
        CollisionInfo(Collidable A, Collidable B);

};

#endif //INFO_COLISAO_HPP
